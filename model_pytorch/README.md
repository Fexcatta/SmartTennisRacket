# Old model in pytorch
This is the deep learning model of the project made in pytorch

## Model Architecture
```
=================================================================
Layer (type:depth-idx)                   Param #
=================================================================
├─Sequential: 1-1                        --
|    └─Normalize: 2-1                    --
|    └─AvgPool1d: 2-2                    --
|    └─Dropout: 2-3                      --
|    └─Conv1d: 2-4                       76
|    └─ReLU: 2-5                         --
|    └─Dropout: 2-6                      --
|    └─MaxPool1d: 2-7                    --
|    └─Flatten: 2-8                      --
|    └─Linear: 2-9                       5,940
|    └─ReLU: 2-10                        --
|    └─Linear: 2-11                      84
|    └─Softmax: 2-12                     --
=================================================================
Total params: 6,100
Trainable params: 6,100
Non-trainable params: 0
=================================================================
```