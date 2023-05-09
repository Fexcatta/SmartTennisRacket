import time
import onnxruntime
import pandas as pd
import numpy as np
from pathlib import Path
from config import config

class InferenceEngine():

    IDX_TO_CLASS = {
        0: "forehand",
        1: "backhand",
        2: "nothing",
        3: "serve"
    }

    def __init__(self):
        self.ort_session = None
        self.__prepare_model()

    def __prepare_model(self):
        if not Path(config.get("ONNX_PATH")).exists():
            print("model.onnx file not found")
            return
        
        self.ort_session = onnxruntime.InferenceSession(config.get("ONNX_PATH"))

    def __preprocess(self, sample):
        df = pd.DataFrame(sample)
        df = df.drop('id', axis=1, errors='ignore')
        tensor = df.to_numpy()
        
        window = config.get("MEASUREMENTS_FOR_INFERENCE")

        if window > 666:
            tensor = tensor[:window, :]
        else:
            # trim data to be just m*2 measurements, data centered in 333
            m = window // 2
            tensor = tensor[333-m:333+m, :]

        tensor = np.expand_dims(tensor, axis=0)
        tensor = tensor.astype(np.float32)

        return tensor

    def predict(self, sample):   
        if not self.ort_session:
            return "error", 0, 0
             
        start_time = time.time()
        input_tensor = self.__preprocess(sample)
        print(input_tensor.shape)

        input_name = self.ort_session.get_inputs()[0].name

        print(input_name)

        ort_outs = self.ort_session.run(None, {input_name: input_tensor})
        output = ort_outs[0]

        prob = np.max(output, 1) 
        idx = np.argmax(output, 1)
        
        idx = idx[0].item() # item 0 of the batch
        prob = prob[0].item() # item 0 of the batch

        return self.IDX_TO_CLASS[idx], prob, time.time() - start_time