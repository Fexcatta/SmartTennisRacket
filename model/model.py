import torch.nn as nn
from torchvision.models import resnet18

class IMUModelBig(nn.Module):

    def __init__(self):
        super(IMUModelBig, self).__init__()
        self.resnet18 = resnet18(weights="IMAGENET1K_V1")

        for p in self.resnet18.parameters():
            p.requires_grad = False

        # change input channels to 1
        self.resnet18.conv1 = nn.Conv2d(1, 64, kernel_size=7, stride=2, padding=3, bias=False)
        
        # change output classes to 4
        self.resnet18.fc = nn.Sequential(
            nn.Linear(512, 256),
            nn.ReLU(),
            nn.Dropout(0.4),
            nn.Linear(256, 4), 
            nn.LogSoftmax(dim=1)
        )
    
    def forward(self, x):
        return self.resnet18(x)

class IMUModelSmall(nn.Module):

    def __init__(self):
        super(IMUModelSmall, self).__init__()

        self.net = nn.Sequential(
            nn.Conv1d(6, 32, kernel_size=3, stride=3, padding=1),
            nn.ReLU(),
            nn.Dropout(0.3),
            nn.Conv1d(32, 16, kernel_size=3, stride=1, padding=1),
            nn.ReLU(),
            nn.Dropout(0.3),
            nn.MaxPool1d(kernel_size=2, stride=2),
            nn.Flatten(),
            nn.Linear(-1, 50),
            nn.ReLU(),
            nn.Linear(50, 4),
            nn.LogSoftmax(dim=1)
        )
    
    def forward(self, x):
        return self.net(x)
