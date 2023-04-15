import torch.nn as nn
from torchvision.models import resnet18

class IMUModel(nn.Module):

    def __init__(self):
        super(IMUModel, self).__init__()
        self.resnet18 = resnet18(weights="IMAGENET1K_V1")

        for p in self.resnet18.parameters():
            p.requires_grad = False

        # change input channels to 1
        self.resnet18.conv1 = nn.Conv2d(1, 64, kernel_size=7, stride=2, padding=3, bias=False)
        
        # change output classes to 3
        self.resnet18.fc = nn.Sequential(
            nn.Linear(512, 256),
            nn.ReLU(),
            nn.Dropout(0.4),
            nn.Linear(256, 3), 
            nn.LogSoftmax(dim=1)
        )
    
    def forward(self, x):
        return self.resnet18(x)