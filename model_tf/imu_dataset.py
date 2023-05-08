from pathlib import Path
import pandas as pd
import numpy as np

class IMUDataset():

    IDX_TO_CLASS = {
        0: "forehand",
        1: "backhand",
        2: "nothing",
        3: "serve"
    }

    CLASS_TO_IDX = {
        "forehand": 0,
        "backhand": 1,
        "nothing": 2,
        "serve": 3
    }

    def __init__(self, base_path):
        self.base_path = Path(base_path)
        self.data = []
        self.labels = []

        for d in self.base_path.glob("*"):
            if d.is_dir():
                for csv_path in d.glob("*.csv"):
                    # load csv as tensor
                    self.data.append(self.read_sample(csv_path))
                    self.labels.append(self.CLASS_TO_IDX[d.name])

    def compute_means_stds(self):
        # compute mean and std for normalization
        means = np.stack(self.data).mean(axis=(0,2)).T
        means = np.expand_dims(means, axis=1)
        
        stds = np.stack(self.data).std(axis=(0,2)).T
        stds = np.expand_dims(stds, axis=1)

        return means, stds


    def read_sample(self, csv_path):
        """
        Read a sample from a csv file and return it as a tensor
        """
        data = pd.read_csv(csv_path, header=0)
        
        tensor = data.to_numpy()

        # trim last 333 measurements, data centered in 333
        tensor = tensor[:667, :]

        # trim data to be just 300 measurements
        m = 300 // 2
        tensor = tensor[333-m:333+m, :]

        tensor.astype(np.float32)

        return tensor

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        data = self.data[idx]
        label = self.labels[idx]

        return data, label
