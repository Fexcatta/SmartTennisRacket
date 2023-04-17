import serial
import time
from threading import Thread
import serial.tools.list_ports as list_ports
from rich import print
from reader import Reader

class SerialReader(Reader):

    def __init__(self, port=None, listener=None, new_thread=True):
        super.__init__(listener)
        if not port:
            print(*[f"[{i}] {p}" for i,(p,_,_) in enumerate(serial.tools.list_ports.comports())], sep="\n")
            try:
                p = int(input("Which port to use? "))
                port = list_ports.comports()[p][0]
            except:
                raise Exception("Invalid port")
            
        self.port = port
        self.listener = listener

        if new_thread:
            Thread(target=self.__connect).start()
        else:
            self.__connect()

    def __connect(self):
        self.conn = None
        try:
            self.conn = serial.Serial(self.port, 115200, timeout=1)
            time.sleep(5)
            print(f"Connected to {self.port}")
            self.__start_listening()
        except:
            print("Could not open serial port, retrying in 5 seconds")
            time.sleep(5)
            self.__connect()
    
    def __start_listening(self):
        try:
            while True:
                if "%" in self.__read():
                    self.sample = []
                    l = self.__read()
                    while "%" not in l:
                        self.sample.append(self.__parse_line(l))
                        l = self.__read()
                    
                    if self.listener:
                        self.listener(self.sample)
        except Exception as e:
            print(e)
            print("Serial connection lost, reconnecting")
            self.__connect()

    def __read(self):
        line = self.conn.readline()
        if line:
            return line.decode('utf-8').strip()
        
        return ""

    def __parse_line(self, line):
        vals = [float(v) for v in line.split(";")[:-1]]
        labels = ["accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ"]
        return dict(zip(labels, vals))

def process_sample(sample):
    print("Received sample with %d values" % len(sample))

if __name__ == "__main__":
    SerialReader(listener=process_sample)