# Model
This is the deep learning model of the project made in tensorflow

## Model Architecture
```
_________________________________________________________________
 Layer (type)                Output Shape              Param #   
=================================================================
 normalize_4 (Normalize)     multiple                  0         
                                                                 
 conv1d_8 (Conv1D)           multiple                  152       
                                                                 
 dropout_12 (Dropout)        multiple                  0         
                                                                 
 conv1d_9 (Conv1D)           multiple                  100       
                                                                 
 dropout_13 (Dropout)        multiple                  0         
                                                                 
 max_pooling1d_4 (MaxPooling  multiple                 0         
 1D)                                                             
                                                                 
 flatten_4 (Flatten)         multiple                  0         
                                                                 
 dropout_14 (Dropout)        multiple                  0         
                                                                 
 dense_4 (Dense)             multiple                  1204      
                                                                 
=================================================================
Total params: 1,456
Trainable params: 1,456
Non-trainable params: 0
_________________________________________________________________
```