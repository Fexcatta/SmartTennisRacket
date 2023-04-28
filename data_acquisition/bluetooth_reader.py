from reader import Reader
from threading import Thread
from utils import to_float, to_int
from bleak import BleakClient, BleakScanner
from tqdm import tqdm
import asyncio
from rich import print
from aioconsole import ainput
from config import config

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
            self.progress_bar = tqdm(range(config.get("MEASUREMENTS_PER_SAMPLE")), desc="Receiving sample", unit="m")
        
        measurements = self.__parse_packet(data)
        self.sample.extend(measurements)
        self.progress_bar.update(len(measurements))
        
        # check if sample is complete
        if len(self.sample) == config.get("MEASUREMENTS_PER_SAMPLE"):
            self.progress_bar.close()
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
                if config.get("ID_PRESENT"):
                    vals = [to_int(chunk[-4:])] # last 4 bytes are the id
                
                vals.extend([to_float(chunk[i:i+4]) for i in range(0, chunk_size, 4)])

                labels = ["id", "accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ", ]
            
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
        self.__client = BleakClient(self.device_address, disconnected_callback=self.__disconnect_callback, timeout=5)
        
        try:
            await self.__client.connect()
            print("Connected to", self.device_name or self.device_address)
            
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.CONNECTED)
            
            await self.__client.start_notify(config.get("IMU_CHARACTERISTIC_UUID"), self.__packet_received)

        except Exception as e:
            print("Cannot connect, retrying in 5 seconds...")
            await asyncio.sleep(5)
            await self.__connect()