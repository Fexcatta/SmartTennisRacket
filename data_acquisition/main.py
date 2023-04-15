from GUI import run_gui
from serial_reader import SerialReader
from dataset_manager import DatasetManager
import matplotlib
import matplotlib.pyplot as plt
from pandas import DataFrame
from rich import print

PORT = "/dev/cu.usbmodem1101"

matplotlib.use("SVG")

def save_chart(sample):
    df = DataFrame(sample)
    plt.plot(df[["accX", "accY", "accZ"]])
    plt.savefig("curr_sample.png")

def process_sample(sample):
    print(f"Received sample {dataset_manager.total_count} with {len(sample)} values\n")
    save_chart(sample)
    print("[red]Which label? (f/b/n)[/red]", end=" ")
    l = input()
    if l == "f":
        label = DatasetManager.Label.FOREHAND
    elif l == "b":
        label = DatasetManager.Label.BACKHAND
    elif l == "n":
        label = DatasetManager.Label.NOTHING
    
    dataset_manager.save_sample(sample, label)


dataset_manager = DatasetManager()
serial_reader = SerialReader(port=PORT, listener=process_sample, new_thread=False)
#run_gui(dataset_manager, serial_reader)