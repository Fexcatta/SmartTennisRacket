import tensorflow as tf
from normalize import Normalize
import tensorflow.keras.layers as l

class IMUModelBig(tf.keras.Model):

    def __init__(self, means, stds):
        super(IMUModelBig, self).__init__()

        self.norm = Normalize(means=means, stds=stds)
        self.conv1 = l.Conv1D(filters=32, kernel_size=3, strides=2, padding='same', activation='relu')
        self.conv2 = l.Conv1D(filters=16, kernel_size=3, strides=1, padding='same', activation='relu')
        self.dropout1 = l.Dropout(0.5)
        self.max_pool = l.MaxPool1D(pool_size=2, strides=2)
        self.flatten = l.Flatten()
        self.dense1 = l.Dense(units=30, activation='relu')
        self.dense2 = l.Dense(units=4, activation='softmax')
    
    def call(self, inputs):
        x = self.norm(inputs)
        x = self.conv1(x)
        x = self.conv2(x)
        x = self.dropout1(x)
        x = self.max_pool(x)
        x = self.flatten(x)
        x = self.dropout1(x)
        x = self.dense1(x)
        x = self.dense2(x)
        return x

class IMUModelSmall(tf.keras.Model):

    def __init__(self, means, stds):
        super(IMUModelSmall, self).__init__()

        self.norm = Normalize(means=means, stds=stds)
        self.conv1 = l.Conv1D(filters=8, kernel_size=3, strides=2, padding='same', activation='relu')
        self.dropout1 = l.Dropout(0.5)
        self.conv2 = l.Conv1D(filters=4, kernel_size=3, strides=1, padding='same', activation='relu')
        self.dropout2 = l.Dropout(0.5)
        self.max_pool = l.MaxPool1D(pool_size=2, strides=2)
        self.flatten = l.Flatten()
        self.dropout3 = l.Dropout(0.5)
        self.dense = l.Dense(units=4, activation='softmax')
    
    def call(self, inputs):
        x = self.norm(inputs)
        x = self.conv1(x)
        x = self.dropout1(x)
        x = self.conv2(x)
        x = self.dropout2(x)
        x = self.max_pool(x)
        x = self.flatten(x)
        x = self.dropout3(x)
        x = self.dense(x)
        return x
