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

all = set(range(1000))
found = set()

async def connect():
    address = await discover_bluetooth_device()
    
    async with BleakClient(address) as client:
        print("Connected")
        
        while True:
            accX = to_float(await client.read_gatt_char('19B10001-E8F2-537E-4F6C-D104768A1214'))
            accY = to_float(await client.read_gatt_char('19B10002-E8F2-537E-4F6C-D104768A1214'))
            accZ = to_float(await client.read_gatt_char('19B10003-E8F2-537E-4F6C-D104768A1214'))

            gyrX = to_float(await client.read_gatt_char('19B1F001-E8F2-537E-4F6C-D104768A1214'))
            gyrY = to_float(await client.read_gatt_char('19B1F002-E8F2-537E-4F6C-D104768A1214'))
            gyrZ = to_float(await client.read_gatt_char('19B1F003-E8F2-537E-4F6C-D104768A1214'))

            print(accX, accY, accZ, gyrX, gyrY, gyrZ)


loop = asyncio.get_event_loop()
loop.run_until_complete(connect())
loop.run_forever()