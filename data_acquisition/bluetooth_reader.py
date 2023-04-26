from reader import Reader
from threading import Thread
from utils import to_float, to_int
from bleak import BleakClient
from tqdm import tqdm
import asyncio
from rich import print

class BluetoothReader(Reader):
    DEVICE_ADDRESS = "00C22CA1-4E9A-0DCA-D713-67BE50AB9603"
    #DEVICE_ADDRESS = "54A17D1F-A9C9-FD1E-6E48-6B8BF4F6FB5B"
    IMU_CHARACTERISTIC_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"
    TRIGGER_CHARACTERISTIC_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214"
    MEASUREMENTS_PER_SAMPLE = 1000
    ID_PRESENT = True

    def __init__(self, sample_received_listener=None, connection_listener=None, new_thread=True):
        super().__init__(sample_received_listener, connection_listener)
        
        self.sample = []

        if new_thread:
            Thread(target=self.__start_listening).start()
        else:
            self.__start_listening()
    
    def __start_listening(self):
        try:
            self.loop = asyncio.get_event_loop()
        except RuntimeError:
            self.loop = asyncio.new_event_loop()
            asyncio.set_event_loop(self.loop)

        self.loop.run_until_complete(self.__connect())
        self.loop.run_forever()

    def __packet_received(self, sender, data):
        if len(self.sample) == 0:
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.RECEIVING)
            self.progress_bar = tqdm(range(self.MEASUREMENTS_PER_SAMPLE), desc="Receiving sample", unit="m")
        
        measurements = self.__parse_packet(data)
        self.sample.extend(measurements)
        self.progress_bar.update(len(measurements))
        
        # check if sample is complete
        if len(self.sample) == self.MEASUREMENTS_PER_SAMPLE:
            self.progress_bar.close()
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.CONNECTED)

            #TODO: check if packets arrive in order, otherwise reassemble them using id
            if self.sample_received_listener:
                self.sample_received_listener(self.sample)
            
            self.sample = []

    def __parse_packet(self, data):
        measurements = []

        if self.ID_PRESENT:
            chunk_size = 28
        else:
            chunk_size = 24

        # split data into chunks of chunk_size bytes
        chunks = [data[i:i+chunk_size] for i in range(0, len(data), chunk_size)]

        for chunk in chunks:
            # split chunk into 6 floats and optionally 1 int (the id)
            if self.ID_PRESENT:
                vals = [to_int(chunk[-4:])] # last 4 bytes are the id
            
            vals.extend([to_float(chunk[i:i+4]) for i in range(0, chunk_size, 4)])

            labels = ["id", "accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ", ]
        
            measurements.append(dict(zip(labels, vals)))
        
        return measurements

    def __disconnect_callback(self, sender):
        self.sample = []

        if self.connection_listener:
            self.connection_listener(Reader.ConnectionState.DISCONNECTED)

        print("Connection lost, trying to reconnect...")

        self.loop.create_task(self.__connect())

    async def __connect(self):    
        self.__client = BleakClient(self.DEVICE_ADDRESS, disconnected_callback=self.__disconnect_callback, timeout=5)
        
        try:
            await self.__client.connect()
            print("Connected to", self.DEVICE_ADDRESS)
            
            if self.connection_listener:
                self.connection_listener(Reader.ConnectionState.CONNECTED)
            
            await self.__client.start_notify(self.IMU_CHARACTERISTIC_UUID, self.__packet_received)

            await asyncio.sleep(10)

            await self.__client.write_gatt_char(TRIGGER_CHARACTERISTIC_UUID, bytearray(int(1)))
            print("Sent trigger: ", bytearray(int(1)))

        except Exception as e:
            print("Cannot connect, retrying in 5 seconds...")
            await asyncio.sleep(5)
            await self.__connect()