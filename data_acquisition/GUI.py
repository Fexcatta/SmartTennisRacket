from tkinter import Tk, Label, Button, Frame, Image
from PIL import ImageTk, Image
from threading import Thread

class MainWindow:
    def __init__(self, root, dataset_manager, serial_reader):
        self.root = root
        self.dataset_manager = dataset_manager
        self.serial_reader = serial_reader
        
        root.title("Data Acquisition")
        root.geometry("400x450")
        root.resizable(False, False)

        labels = Frame(root,  padx=20, pady=20)
        labels.place(relx=0.5, rely=0.4, anchor="center")
 
        labels.columnconfigure(0, weight=8)
        labels.columnconfigure(1, weight=2)

        self.total_label = Label(labels, text="Total samples:", font=("Helvetica", 24), padx=20, pady=20)
        self.total_label.grid(row=0, column=0, sticky="w")

        self.total_count_label = Label(labels, text="0", font=("Helvetica", 24), padx=20, pady=20)
        self.total_count_label.grid(row=0, column=1, sticky="e")

        self.forehand_label = Label(labels, text="Forehand:", font=("Helvetica", 24), padx=20, pady=20)
        self.forehand_label.grid(row=1, column=0, sticky="w")

        self.forehand_count_label = Label(labels, text="0", font=("Helvetica", 24), padx=20, pady=20)
        self.forehand_count_label.grid(row=1, column=1, sticky="e")

        self.backhand_label = Label(labels, text="Backhand:", font=("Helvetica", 24), padx=20, pady=20)
        self.backhand_label.grid(row=2, column=0, sticky="w")

        self.backhand_count_label = Label(labels, text="0", font=("Helvetica", 24), padx=20, pady=20)
        self.backhand_count_label.grid(row=2, column=1, sticky="e")

        self.nothing_label = Label(labels, text="Nothing:", font=("Helvetica", 24), padx=20, pady=20)
        self.nothing_label.grid(row=3, column=0, sticky="w")

        self.nothing_count_label = Label(labels, text="0", font=("Helvetica", 24), padx=20, pady=20)
        self.nothing_count_label.grid(row=3, column=1, sticky="e")

        self.read_button = Button(root, text="Read", command=self.process_sample, font=("Helvetica", 24))
        self.read_button.place(relx=0.5, rely=0.9, anchor="center")

        serial_reader.set_listener(self.process_sample)

    def process_sample(self, sample=None):
        root = Tk()
        SampleWindow(root, sample)
        Thread(target=root.mainloop).start()

    def update_counts(self):
        self.total_count_label.config(text=self.forehand + self.backhand + self.nothing)
        self.forehand_count_label.config(text=self.dataset_manager.forehand_count)
        self.backhand_count_label.config(text=self.dataset_manager.backhand_count)
        self.nothing_count_label.config(text=self.dataset_manager.nothing_count)


class SampleWindow:
    def __init__(self, root, sample):
        self.sample = sample
        self.root = root

        root.title(f"New sample")
        root.resizable(False, False)

        self.acc_image = Image.open("acc.png")
        self.acc_image = self.acc_image.resize((1000, 360), Image.Resampling.LANCZOS)
        self.acc_image = ImageTk.PhotoImage(master=root, image=self.acc_image)
        self.acc_chart = Label(root, image=self.acc_image)
        self.acc_chart.pack(side="top", padx=10, pady=10)

        self.gyr_image = Image.open("gyr.png")
        self.gyr_image = self.gyr_image.resize((1000, 360), Image.Resampling.LANCZOS)
        self.gyr_image = ImageTk.PhotoImage(master=root, image=self.gyr_image)
        self.gyr_chart = Label(root, image=self.gyr_image)
        self.gyr_chart.pack(side="top", padx=10, pady=10)

        buttons = Frame(root)
        buttons.pack(side="bottom", pady=10)

        self.forehand_button = Button(buttons, text="Forehand", command=self.forehand, font=("Helvetica", 28))
        self.forehand_button.pack(side="left", padx=10)

        self.backhand_button = Button(buttons, text="Backhand", command=self.backhand, font=("Helvetica", 28))
        self.backhand_button.pack(side="left", padx=10)

        self.nothing_button = Button(buttons, text="Nothing", command=self.nothing, font=("Helvetica", 28))
        self.nothing_button.pack(side="left", padx=10)

    def forehand(self):
        print("forehand")
        self.root.quit()
    
    def backhand(self):
        print("backhand")
        self.root.quit()

    def nothing(self):
        print("nothing")
        self.root.quit()

def run_gui(dataset_manager, serial_reader):
    root = Tk()
    main_window = MainWindow(root, dataset_manager, serial_reader)
    root.mainloop()