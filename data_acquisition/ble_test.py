import asyncio
from bleak import BleakClient, BleakScanner
import struct

DEVICE_ADDRESS = "54A17D1F-A9C9-FD1E-6E48-6B8BF4F6FB5B"

async def discover_bluetooth_device():
    devices = await BleakScanner.discover()
    for d in devices:
        if d.address == DEVICE_ADDRESS:
            print(f"Found device: {d.name} ({d.address})")
            return d.address
    
    raise Exception(f"Could not find device with address {DEVICE_ADDRESS}")

def to_float(b):
    return struct.unpack('f', b)[0]

def to_int(b):
    return struct.unpack('i', b)[0]

def print_sample(sample):
    print(f"{sample['id']:03d}", end=": ")
    print(*sample.values()[:-1], sep=";")

def callback(sender, data):
    sample = []

    # split data into chunks of 28 bytes
    chunks = [data[i:i+28] for i in range(0, len(data), 28)]

    for chunk in chunks:

        # split chunk into 7 floats
        vals = [to_float(chunk[i:i+4]) for i in range(0, len(chunk) - 4, 4)]
        vals.append(to_int(chunk[-4:])) # last 4 bytes are the id
        labels = ["accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ", "id"]
    
        sample.append(dict(zip(labels, vals)))
        print_sample(sample[-1])

async def connect():
    address = DEVICE_ADDRESS #await discover_bluetooth_device()
    
    client = BleakClient(address, disconnected_callback=lambda x: print("Disconnected"))
    try:
        await client.connect()
        print("Connected")
        await client.start_notify("19B10001-E8F2-537E-4F6C-D104768A1214", callback)
    except Exception as e:
        print(e)

loop = asyncio.get_event_loop()
loop.run_until_complete(connect())
loop.run_forever()