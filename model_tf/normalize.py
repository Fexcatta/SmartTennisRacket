import tensorflow as tf

class Normalize(tf.keras.layers.Layer):

    def __init__(self, means, stds):
        super(Normalize, self).__init__()
        self.means = means
        self.stds = stds

    def call(self, inputs):
        m = inputs - self.means
        return m / self.stds