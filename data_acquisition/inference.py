import time
import onnxruntime
import pandas as pd
import numpy as np
from pathlib import Path

class InferenceEngine():

    IDX_TO_CLASS = {
        0: "forehand",
        1: "backhand",
        2: "nothing"
    }

    def __init__(self):
        self.ort_session = None
        self.__prepare_model()

    def __prepare_model(self):
        if not Path("model.onnx").exists():
            print("model.onnx file not found")
            return
        
        self.ort_session = onnxruntime.InferenceSession("model.onnx")

    def __preprocess(self, sample):
        df = pd.DataFrame(sample)
        tensor = df.to_numpy().astype(np.float32)
        tensor = np.expand_dims(tensor, axis=0)
        tensor = np.expand_dims(tensor, axis=0)

        return tensor

    def predict(self, sample):   
        if not self.ort_session:
            return "error", 0, 0
             
        start_time = time.time()
        input_tensor = self.__preprocess(sample)

        ort_outs = self.ort_session.run(None, {'input': input_tensor})
        output = ort_outs[0]

        # log probabilities
        ps = np.exp(output)

        prob = np.max(ps, 1) 
        idx = np.argmax(ps, 1)
        
        idx = idx[0].item() # item 0 of the batch
        prob = prob[0].item() # item 0 of the batch

        return self.IDX_TO_CLASS[idx], prob, time.time() - start_time