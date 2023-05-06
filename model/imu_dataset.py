from pathlib import Path
from torch.utils.data import Dataset
import torch
import pandas as pd

class IMUDataset(Dataset):

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

    def read_sample(self, csv_path):
        """
        Read a sample from a csv file and return it as a tensor
        """
        data = pd.read_csv(csv_path, header=0)
        tensor = torch.tensor(data.values).type(torch.float32)
        tensor = tensor.T # from (1000, 6) to (6, 1000)
        return tensor

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        data = self.data[idx]
        label = self.labels[idx]
        data = self.__normalize(data)
        
        return data, label
    
    def __normalize(self, tensor):
        return tensor
    
        """samples = [d for d,l in list(dataset)]

        # compute mean and std
        mean = torch.stack(samples).mean(dim=(0,2)) # [  0.1352,   0.9354,  -0.0871, -18.0559,  32.6679,  -7.0598]
        std = torch.stack(samples).std(dim=(0,2)) # [  1.4614,   3.0519,   1.3878, 241.3652, 228.0653, 239.8968]
        mean, std
        
        min = torch.min(data)
        max = torch.max(data)
        data = (data - min) / (max - min)
        return data"""