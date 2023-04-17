from reader import Reader
from threading import Thread

class BluetoothReader(Reader):

    def __init__(self, listener=None, new_thread=True):
        super.__init__(listener)

        if new_thread:
            Thread(target=self.__connect).start()
        else:
            self.__connect()

    def __connect(self):
        # TODO: connect to bluetooth device
        pass
