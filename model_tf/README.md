# Model
This is the deep learning model of the project made in tensorflow

## Model Architecture
```
_________________________________________________________________
 Layer (type)                Output Shape              Param #   
=================================================================
 normalize (Normalize)       multiple                  0         
                                                                 
 average_pooling1d (AverageP  multiple                 0         
 ooling1D)                                                       
                                                                 
 dropout (Dropout)           multiple                  0         
                                                                 
 conv1d (Conv1D)             multiple                  76        
                                                                 
 max_pooling1d (MaxPooling1D  multiple                 0         
 )                                                               
                                                                 
 flatten (Flatten)           multiple                  0         
                                                                 
 dropout_1 (Dropout)         multiple                  0         
                                                                 
 dense (Dense)               multiple                  5940      
                                                                 
 dense_1 (Dense)             multiple                  84        
                                                                 
=================================================================
Total params: 6,100
Trainable params: 6,100
Non-trainable params: 0
```