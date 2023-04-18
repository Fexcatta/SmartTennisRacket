from GUI import run_gui
from serial_reader import SerialReader
from bluetooth_reader import BluetoothReader
from dataset_manager import DatasetManager
from rich import print

PORT = "/dev/cu.usbmodem1101"

if __name__ == "__main__":
    dataset_manager = DatasetManager()
    reader = BluetoothReader(new_thread=True)
    run_gui(dataset_manager, reader)