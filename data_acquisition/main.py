from GUI import run_gui
from serial_reader import SerialReader
from bluetooth_reader import BluetoothReader
from dataset_manager import DatasetManager
from rich import print
from config import config

if __name__ == "__main__":
    dataset_manager = DatasetManager()
    reader = BluetoothReader(device_address=config.get("DEVICE_ADDRESS"))
    run_gui(dataset_manager, reader)