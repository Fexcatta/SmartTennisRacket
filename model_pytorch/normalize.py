import torch.nn as nn

class Normalize(nn.Module):

    def __init__(self, means, stds):
        super(Normalize, self).__init__()

        self.means = means
        self.stds = stds
    
    def forward(self, x):
        x -= self.means.to(x.device)
        x /= self.stds.to(x.device)

        return x