from abc import ABC, abstractmethod

class Reader(ABC):
    
    class ConnectionState:
        CONNECTED = 0
        DISCONNECTED = 1
        RECEIVING = 2

    def __init__(self, sample_received_listener=None, connection_listener=None):
        self.sample_received_listener = sample_received_listener
        self.connection_listener = connection_listener

    def set_sample_received_listener(self, listener):
        """
        Set the callback function that will be called when a sample is received
        """
        self.sample_received_listener = listener

    def set_connection_listener(self, listener):
        """
        Set the callback function that will be called when the connection state changes
        """
        self.connection_listener = listener
