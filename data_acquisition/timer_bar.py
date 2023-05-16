from tqdm import tqdm
from threading import Thread
import time

class TimerBar:
    def __init__(self, seconds=10, update_interval=0.1):
        self.seconds = seconds
        self.update_interval = update_interval
        self.total_intervals = int(self.seconds / self.update_interval)
        self.stopped = False

        # start timer of 10 seconds
        self.progress_bar = tqdm(range(self.total_intervals), bar_format='{bar}')
    
    def start(self):
        Thread(target=self.__start).start()

    def __start(self):
        # update progress bar every second
        for i in range(self.total_intervals):
            if self.stopped:
                break

            self.progress_bar.n = self.total_intervals - i - 1
            self.progress_bar.refresh()
            time.sleep(self.update_interval)

    def stop(self):
        try:
            self.progress_bar.close()
            self.stopped = True
        except:
            pass