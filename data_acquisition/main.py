from GUI import run_gui
from serial_reader import SerialReader
from bluetooth_reader import BluetoothReader
from dataset_manager import DatasetManager
from rich import print

PORT = "/dev/cu.usbmodem1101"

DEVICE_ADDRESS = None # discover devices
#DEVICE_ADDRESS = "00C22CA1-4E9A-0DCA-D713-67BE50AB9603" # federico
#DEVICE_ADDRESS = "54A17D1F-A9C9-FD1E-6E48-6B8BF4F6FB5B" # francesco

if __name__ == "__main__":
    dataset_manager = DatasetManager()
    reader = BluetoothReader(device_address=DEVICE_ADDRESS, new_thread=True)
    run_gui(dataset_manager, reader)