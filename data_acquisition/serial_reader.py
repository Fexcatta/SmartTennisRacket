import serial
import time
from threading import Thread
import serial.tools.list_ports as list_ports
from rich import print

class SerialReader:

    def __init__(self, port=None, listener=None, new_thread=True):
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
            Thread(target=self.connect).start()
        else:
            self.connect()

    def connect(self):
        self.conn = None
        try:
            self.conn = serial.Serial(self.port, 115200, timeout=1)
            time.sleep(5)
            print(f"Connected to {self.port}")
            self.start_listening()
        except:
            print("Could not open serial port, retrying in 5 seconds")
            time.sleep(5)
            self.connect()
    
    def start_listening(self):
        try:
            while True:
                if "%" in self.read():
                    self.sample = []
                    l = self.read()
                    while "%" not in l:
                        self.sample.append(self.parse_line(l))
                        l = self.read()
                    
                    if self.listener:
                        self.listener(self.sample)
        except Exception as e:
            print(e)
            print("Serial connection lost, reconnecting")
            self.connect()

    def read(self):
        line = self.conn.readline()
        if line:
            return line.decode('utf-8').strip()
        
        return ""

    def parse_line(self, line):
        vals = [float(v) for v in line.split(";")[:-1]]
        labels = ["accX", "accY", "accZ", "gyrX", "gyrY", "gyrZ"]
        return dict(zip(labels, vals))

    def set_listener(self, listener):
        self.listener = listener

def process_sample(sample):
    print("Received sample with %d values" % len(sample))

if __name__ == "__main__":
    SerialReader(listener=process_sample)