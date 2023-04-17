from abc import ABC, abstractmethod

class Reader(ABC):
    
    def __init__(self, listener=None):
        self.listener = listener

    def set_listener(self, listener):
        """
        Set the callback function that will be called when a sample is received
        """
        self.listener = listener