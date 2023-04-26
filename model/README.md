# Model
This is the deep learning model of the project.

## Model Architecture
Currently it is a ResNet-18 model (used as a testbench) with a custom head and tail.

In particular `conv1` layer is replaced with a similar 7x7 conv layer with only 1 channel.
The fully connected layer is replaced with a Sequential block as follows:
 - `Linear(512, 256)`
 - `ReLU()`
 - `Dropout(0.4)`
 - `Linear(256, 3)`
 - `LogSoftmax(dim=1)`