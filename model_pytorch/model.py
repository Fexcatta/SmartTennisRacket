import torch
import torch.nn as nn
from normalize import Normalize

class IMUModelBig(nn.Module):

    def __init__(self, means, stds):
        super(IMUModelBig, self).__init__()

        self.norm = Normalize(means=means, stds=stds)
        self.conv1 = nn.Conv1d(in_channels=6, out_channels=32, kernel_size=3, stride=2, padding=1)
        self.conv2 = nn.Conv1d(in_channels=32, out_channels=16, kernel_size=3, stride=1, padding=1)
        self.dropout1 = nn.Dropout(0.5)
        self.max_pool = nn.MaxPool1d(kernel_size=2, stride=2)
        self.flatten = nn.Flatten()
        self.dense1 = nn.LazyLinear(30)
        self.dense2 = nn.Linear(30, 4)
        self.relu = nn.ReLU()
        self.softmax = nn.Softmax(dim=1)

    def forward(self, inputs):
        x = self.norm(inputs)
        x = self.conv1(x)
        x = self.relu(x)
        x = self.conv2(x)
        x = self.relu(x)
        x = self.dropout1(x)
        x = self.max_pool(x)
        x = self.flatten(x)
        x = self.dense1(x)
        x = self.relu(x)
        x = self.dense2(x)
        x = self.softmax(x)
        return x

class IMUModelSmall(nn.Module):

    def __init__(self, means, stds):
        super(IMUModelSmall, self).__init__()

        self.norm = Normalize(means=means, stds=stds)
        self.conv1 = nn.Conv1d(in_channels=6, out_channels=8, kernel_size=3, stride=2, padding=1)
        self.dropout1 = nn.Dropout(0.5)
        self.conv2 = nn.Conv1d(in_channels=8, out_channels=4, kernel_size=3, stride=1, padding=1)
        self.dropout2 = nn.Dropout(0.5)
        self.max_pool = nn.MaxPool1d(kernel_size=2, stride=2)
        self.flatten = nn.Flatten()
        self.dropout3 = nn.Dropout(0.5)
        self.dense = nn.LazyLinear(4)
        self.relu = nn.ReLU()
        self.softmax = nn.Softmax(dim=1)

    def forward(self, inputs):
        x = self.norm(inputs)
        x = self.conv1(x)
        x = self.relu(x)
        x = self.dropout1(x)
        x = self.conv2(x)
        x = self.relu(x)
        x = self.dropout2(x)
        x = self.max_pool(x)
        x = self.flatten(x)
        x = self.dropout3(x)
        x = self.dense(x)
        x = self.softmax(x)
        return x
