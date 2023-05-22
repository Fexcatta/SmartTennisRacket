from reader import Reader
from threading import Thread
from utils import to_float, to_int
from bleak import BleakClient, BleakScanner
from tqdm import tqdm
import asyncio
from rich import print
from aioconsole import ainput
from config import config
from timer_bar import TimerBar

class BluetoothReader(Reader):

    def __init__(self, device_address=None, sample_received_listener=None, connection_listener=None, new_thread=True):
        super().__init__(sample_received_listener, connection_listener)
        
        self.sample = []
        self.device_address = device_address
        self.device_name = None

        if new_thread:
            Thread(target=self.__start_listening).start()
        else:
            self.__start_listening()
    
    async def __discover_and_select_device(self):
        print("Scanning for devices...")
        devices = await BleakScanner.discover()
        
        if len(devices) == 0:
            print("No devices found, please try again.")
            exit()

        for i, d in enumerate(devices):
            print(f"[{i}] {d.name or d.address}")

        try:
            choice = int(await ainput("Which device to use? "))
            self.device_address = devices[choice].address
            self.device_name = devices[choice].name
        except:
            print("Invalid choice")


    def __start_listening(self):
        try:
            self.loop = asyncio.get_event_loop()
        except RuntimeError:
            self.loop = asyncio.new_event_loop()
            asyncio.set_event_loop(self.loop)

        if self.device_address:
            self.loop.run_until_complete(self.__connect())
        else:
            self.loop.run_until_complete(self.__discover_and_select_device())
            self.loop.run_until_complete(self.__connect())
        self.loop.run_forever()

    def __packet_received(self, sender, data):
        if len(self.sample) == 0:
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.RECEIVING)
            self.sample_progress_bar = tqdm(range(config.get("MEASUREMENTS_PER_SAMPLE")), desc="Receiving sample", unit="m")
        
        measurements = self.__parse_packet(data)
        self.sample.extend(measurements)
        self.sample_progress_bar.update(len(measurements))
        
        # check if sample is complete
        if len(self.sample) == config.get("MEASUREMENTS_PER_SAMPLE"):
            self.sample_progress_bar.close()
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.CONNECTED)

            #TODO: check if packets arrive in order, otherwise reassemble them using id
            if self.sample_received_listener:
                self.sample_received_listener(self.sample)
            
            self.sample = []

    def __parse_packet(self, data):
        measurements = []
        
        try:
            if config.get("ID_PRESENT"):
                chunk_size = 28
            else:
                chunk_size = 24

            surplus = len(data) % chunk_size
            if surplus > 0:
                print(f"[bold red]Received packet with {len(data)} bytes which is not divisible by {chunk_size}, discarding last {surplus} bytes[/red bold]")
                data = data[:-surplus]

            # split data into chunks of chunk_size bytes
            chunks = [data[i:i+chunk_size] for i in range(0, len(data), chunk_size)]

            for chunk in chunks:
                # split chunk into 6 floats and optionally 1 int (the id) 
                vals = []
                for i in range(0, 6*4, 4):
                    vals.append(to_float(chunk[i:i+4]))

                if config.get("ID_PRESENT"):
                    vals.append(to_int(chunk[-4:])) # last 4 bytes are the id
                
                labels = ["accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ", "id"]
            
                measurements.append(dict(zip(labels, vals)))
        except Exception as e:
            print("Cannot parse packet:", e)

        return measurements

    def __disconnect_callback(self, sender):
        self.sample = []

        if self.connection_listener:
            self.connection_listener(Reader.ConnectionState.DISCONNECTED)

        print("Connection lost, trying to reconnect...")

        self.loop.create_task(self.__connect())

    async def __send_trigger(self):
        if self.__client.is_connected:
            await self.__client.write_gatt_char(config.get("TRIGGER_CHARACTERISTIC_UUID"), bytearray(b'\x01'), response=True)
            print("Sent trigger")

    def send_trigger(self):
        asyncio.run_coroutine_threadsafe(self.__send_trigger(), self.loop)

    async def __connect(self):    
        self.__client = BleakClient(self.device_address, disconnected_callback=self.__disconnect_callback, timeout=3)
        
        try:
            await self.__client.connect()
            print("Connected to", self.device_name or self.device_address)
            
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.CONNECTED)
            
            if not config.get("IMU_CHARACTERISTIC_UUID"):
                print("IMU_CHARACTERISTIC_UUID not set, cannot receive samples.")
            else:
                try:
                    await self.__client.start_notify(config.get("IMU_CHARACTERISTIC_UUID"), self.__packet_received)
                except:
                    pass

            
            if not config.get("INFERENCE_CHARACTERISTIC_UUID"):
                print("INFERENCE_CHARACTERISTIC_UUID not set, cannot receive inferences.")
            else:
                try:
                    await self.__client.start_notify(config.get("INFERENCE_CHARACTERISTIC_UUID"), self.__inference_packet_received)
                except:
                    pass

        except Exception as e:
            print(e)
            print("Cannot connect, retrying...")
            await asyncio.sleep(0.5)
            await self.__connect()

    async def __inference_packet_received(self, sender, data):
        try:
            self.inference_progress_bar.stop()
        except:
            pass

        data = data.decode("utf-8")

        # parse inference result
        result = {}
        for l in data.splitlines():
            cls, logit = l.split(":")
            cls = cls.strip()
            logit = float(logit.strip())
            result[cls] = logit

        # pretty print
        print("", end="\n")
        
        m = max(result.values())
        for k, v in result.items():
            start_max = "✅[dark_orange]" if v == m else "  "
            end_max = "[/dark_orange]" if v == m else ""

            print(f"{start_max}{k:<8} {v*100:>5.1f}% {end_max}")

        print("", end="\n")

        if self.inference_received_listener:
            self.inference_received_listener(data)

        self.inference_progress_bar = TimerBar()
        self.inference_progress_bar.start()