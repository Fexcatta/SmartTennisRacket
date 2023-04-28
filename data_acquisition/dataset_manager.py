from pathlib import Path
import csv
from rich import print
from config import config

class DatasetManager:

    class Label:
        FOREHAND = "forehand"
        BACKHAND = "backhand"
        NOTHING = "nothing"

    def __init__(self):
        if config.get("DATASET_PATH"):
            self.base_path = Path(config.get("DATASET_PATH")) / "dataset"
        else:
            self.base_path = Path(__file__).parent.absolute() / "dataset"
        self.base_path.mkdir(parents=True, exist_ok=True)

        self.forehands_path = self.base_path / self.Label.FOREHAND
        self.forehands_path.mkdir(parents=True, exist_ok=True)

        self.backhands_path = self.base_path / self.Label.BACKHAND
        self.backhands_path.mkdir(parents=True, exist_ok=True)

        self.nothing_path = self.base_path / self.Label.NOTHING
        self.nothing_path.mkdir(parents=True, exist_ok=True)

        self.read_counts()

    def read_counts(self):
        self.forehand_count = len(list(self.forehands_path.glob("*.csv")))
        self.backhand_count = len(list(self.backhands_path.glob("*.csv")))
        self.nothing_count = len(list(self.nothing_path.glob("*.csv")))
        self.total_count = self.forehand_count + self.backhand_count + self.nothing_count

    def save_sample(self, sample, label):
        path = self.base_path / label

        if label == self.Label.FOREHAND:
            filename = f"{self.forehand_count}.csv"
            self.forehand_count += 1
        elif label == self.Label.BACKHAND:
            filename = f"{self.backhand_count}.csv"
            self.backhand_count += 1
        elif label == self.Label.NOTHING:
            filename = f"{self.nothing_count}.csv"
            self.nothing_count += 1
        
        self.total_count += 1
        path = path / filename

        #save file
        try:
            with open(path, 'w', newline='') as csvfile:
                header = ["accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ"]
                if config.get("ID_PRESENT"):
                    header.insert(0, "id")
                writer = csv.DictWriter(csvfile, fieldnames=header)
                writer.writeheader()

                for row in sample:
                    writer.writerow(row)

            print(f"Saved sample to {path}")
        except IOError:
            print("Error saving sample")
        
        if self.listener:
            self.listener()
    
    def set_listener(self, listener):
        self.listener = listener