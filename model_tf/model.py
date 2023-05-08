import tensorflow as tf
from normalize import Normalize
import tensorflow.keras.layers as l

class IMUModelSmall(tf.keras.Model):

    def __init__(self, means, stds):
        super(IMUModelSmall, self).__init__()

        self.norm = Normalize(means=means, stds=stds)
        self.avg_pool = l.AvgPool1D(pool_size=3, strides=2)
        self.dropout1 = l.Dropout(0.4)
        self.conv1 = l.Conv1D(filters=4, kernel_size=3, strides=1, padding='same', activation='relu')
        self.max_pool = l.MaxPool1D(pool_size=2, strides=2)
        self.flatten = l.Flatten()
        self.dropout2 = l.Dropout(0.4)
        self.dense1 = l.Dense(units=20, activation='relu')
        self.dense2 = l.Dense(units=4, activation='softmax')
    
    def call(self, inputs):
        x = self.norm(inputs)
        x = self.avg_pool(x)
        x = self.dropout1(x)
        x = self.conv1(x)
        x = self.max_pool(x)
        x = self.flatten(x)
        x = self.dropout2(x)
        x = self.dense1(x)
        x = self.dense2(x)
        return x
