from GUI import run_gui
from serial_reader import SerialReader
from dataset_manager import DatasetManager
from rich import print

PORT = "/dev/cu.usbmodem1101"

if __name__ == "__main__":
    dataset_manager = DatasetManager()
    serial_reader = SerialReader(new_thread=True)
    run_gui(dataset_manager, serial_reader)