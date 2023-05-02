import tkthread; tkthread.patch()
from tkinter import Tk, Label, Button, Frame, Image, Toplevel
from PIL import ImageTk, Image
from threading import Thread
from dataset_manager import DatasetManager
import matplotlib
import matplotlib.pyplot as plt
from pandas import DataFrame
from inference import InferenceEngine
from rich import print
from reader import Reader

plt.style.use('seaborn')
matplotlib.use("SVG")

def save_charts(sample):
    df = DataFrame(sample)
    
    fig, ax = plt.subplots(figsize=(16, 5))
    ax.plot(df[["accX", "accY", "accZ"]])
    ax.set_title("Accelerometer")
    fig.savefig("acc.png", bbox_inches='tight')

    fig, ax = plt.subplots(figsize=(16, 5))
    ax.set_title("Gyroscope")
    ax.plot(df[["gyrX", "gyrY", "gyrZ"]])
    fig.savefig("gyr.png", bbox_inches='tight')

    plt.close("all")

class MainWindow:
    def __init__(self, root, dataset_manager, reader: Reader):
        self.root = root
        self.dataset_manager = dataset_manager
        self.dataset_manager.set_listener(self.update_counts)
        self.reader = reader
        
        root.title("Data Acquisition")
        root.geometry("350x500")
        root.resizable(False, False)

        labels = Frame(root,  padx=20, pady=20)
        labels.place(relx=0.5, rely=0.4, anchor="center")
 
        labels.columnconfigure(0, weight=8)
        labels.columnconfigure(1, weight=2)

        self.total_label = Label(labels, text="Total samples:", font=("Helvetica", 24), padx=20, pady=20)
        self.total_label.grid(row=0, column=0, sticky="w")

        self.total_count_label = Label(labels, text=self.dataset_manager.total_count, font=("Helvetica", 24), padx=20, pady=20)
        self.total_count_label.grid(row=0, column=1, sticky="e")

        self.forehand_label = Label(labels, text="Forehand:", font=("Helvetica", 24), padx=20, pady=20)
        self.forehand_label.grid(row=1, column=0, sticky="w")

        self.forehand_count_label = Label(labels, text=self.dataset_manager.forehand_count, font=("Helvetica", 24), padx=20, pady=20)
        self.forehand_count_label.grid(row=1, column=1, sticky="e")

        self.backhand_label = Label(labels, text="Backhand:", font=("Helvetica", 24), padx=20, pady=20)
        self.backhand_label.grid(row=2, column=0, sticky="w")

        self.backhand_count_label = Label(labels, text=self.dataset_manager.backhand_count, font=("Helvetica", 24), padx=20, pady=20)
        self.backhand_count_label.grid(row=2, column=1, sticky="e")

        self.serve_label = Label(labels, text="Serve:", font=("Helvetica", 24), padx=20, pady=20)
        self.serve_label.grid(row=3, column=0, sticky="w")

        self.serve_count_label = Label(labels, text=self.dataset_manager.serve_count, font=("Helvetica", 24), padx=20, pady=20)
        self.serve_count_label.grid(row=3, column=1, sticky="e")

        self.nothing_label = Label(labels, text="Nothing:", font=("Helvetica", 24), padx=20, pady=20)
        self.nothing_label.grid(row=4, column=0, sticky="w")

        self.nothing_count_label = Label(labels, text=self.dataset_manager.nothing_count, font=("Helvetica", 24), padx=20, pady=20)
        self.nothing_count_label.grid(row=4, column=1, sticky="e")

        self.connection_label = Label(root, text="Not connected", font=("Helvetica", 32), padx=20, pady=20, foreground="red")
        self.connection_label.place(relx=0.5, rely=0.8, anchor="center")

        self.read_button = Button(root, text="Send trigger", command=lambda: self.reader.send_trigger(), font=("Helvetica", 24))
        self.read_button.place(relx=0.5, rely=0.9, anchor="center")

        reader.set_sample_received_listener(self.process_sample)
        reader.set_connection_listener(self.update_connection_state)
    
    def update_connection_state(self, state):
        if state == Reader.ConnectionState.CONNECTED:
            self.connection_label.config(text="Connected", foreground="#00dd22")
        elif state == Reader.ConnectionState.DISCONNECTED:
            self.connection_label.config(text="Not connected", foreground="red")
        elif state == Reader.ConnectionState.RECEIVING:
            self.connection_label.config(text="Receiving", foreground="#00d0ff")

    def process_sample(self, sample):
        root = Toplevel(self.root)
        SampleWindow(root, sample, self.dataset_manager)

    def update_counts(self):
        self.total_count_label.config(text=self.dataset_manager.total_count)
        self.forehand_count_label.config(text=self.dataset_manager.forehand_count)
        self.backhand_count_label.config(text=self.dataset_manager.backhand_count)
        self.serve_count_label.config(text=self.dataset_manager.serve_count)
        self.nothing_count_label.config(text=self.dataset_manager.nothing_count)


class SampleWindow:
    def __init__(self, root, sample, dataset_manager):
        self.sample = sample
        self.root = root
        root.geometry("+400+0")
        self.dataset_manager = dataset_manager

        root.title(f"Sample n. {self.dataset_manager.total_count}")
        root.resizable(False, False)

        save_charts(sample)
        self.acc_image = Image.open("acc.png")
        self.acc_image = self.acc_image.resize((1000, 312), Image.Resampling.LANCZOS)
        self.acc_image = ImageTk.PhotoImage(master=root, image=self.acc_image)
        self.acc_chart = Label(root, image=self.acc_image)
        self.acc_chart.pack(side="top", padx=10, pady=10)

        self.gyr_image = Image.open("gyr.png")
        self.gyr_image = self.gyr_image.resize((1000, 312), Image.Resampling.LANCZOS)
        self.gyr_image = ImageTk.PhotoImage(master=root, image=self.gyr_image)
        self.gyr_chart = Label(root, image=self.gyr_image)
        self.gyr_chart.pack(side="top", padx=10, pady=10)

        buttons = Frame(root)
        buttons.pack(side="bottom", pady=10)

        self.forehand_button = Button(buttons, text="Forehand", command=self.forehand, font=("Helvetica", 28))
        self.forehand_button.pack(side="left", padx=10)

        self.backhand_button = Button(buttons, text="Backhand", command=self.backhand, font=("Helvetica", 28))
        self.backhand_button.pack(side="left", padx=10)

        self.serve_button = Button(buttons, text="Serve", command=self.serve, font=("Helvetica", 28))
        self.serve_button.pack(side="left", padx=10)
        
        self.nothing_button = Button(buttons, text="Nothing", command=self.nothing, font=("Helvetica", 28))
        self.nothing_button.pack(side="left", padx=10)

        self.delete_button = Button(buttons, text="Delete", command=self.delete, font=("Helvetica", 28), fg="#ff0000")
        self.delete_button.pack(side="left", padx=10)

        self.inference_button = Button(buttons, text="Inference", command=self.inference, font=("Helvetica", 28), fg="#ff8700")
        self.inference_button.pack(side="left", padx=10)

    def forehand(self):
        print("Selected forehand")
        self.dataset_manager.save_sample(self.sample, DatasetManager.Label.FOREHAND)
        self.root.destroy()
    
    def backhand(self):
        print("Selected backhand")
        self.dataset_manager.save_sample(self.sample, DatasetManager.Label.BACKHAND)
        self.root.destroy()

    def serve(self):
        print("Selected serve")
        self.dataset_manager.save_sample(self.sample, DatasetManager.Label.SERVE)
        self.root.destroy()

    def nothing(self):
        print("Selected nothing")
        self.dataset_manager.save_sample(self.sample, DatasetManager.Label.NOTHING)
        self.root.destroy()

    def delete(self):
        print("Deleted sample")
        self.root.destroy()

    def inference(self):
        engine = InferenceEngine()
        c, p, t = engine.predict(self.sample)
        print(f"Predicted: [dark_orange]{c} ({p*100:.1f}%)[/dark_orange], inference time: [green]{t*1000:.2f}ms[/green]")
        self.root.destroy()

def run_gui(dataset_manager, reader):
    root = Tk()
    main_window = MainWindow(root, dataset_manager, reader)
    root.mainloop()