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
        tensor = tensor.T
        
        window = config.get("MEASUREMENTS_FOR_INFERENCE")

        if window > 666:
            tensor = tensor[:, :window]
        else:
            # trim data to be just m*2 measurements, data centered in 333
            m = window // 2
            tensor = tensor[:, 333-m:333+m]

        # normalize
        means = np.array([  0.2193,   3.6914,  -0.2452, -46.9606,  68.9107, -14.5513])
        means = np.expand_dims(means, axis=1)
        stds = np.array([  2.3339,   4.2687,   2.2848, 410.9049, 330.7948, 386.3784])  
        stds = np.expand_dims(stds, axis=1)

        tensor = (tensor - means) / stds
        
        tensor = np.expand_dims(tensor, axis=0)
        tensor = tensor.astype(np.float32)

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