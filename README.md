# Smart Tennis Racket

This repository contains all the source code and material for an University project made for Hardware Architectures for Embedded and Edge AI (prof. Roveri Manuel) course at Politecnico di Milano (Milan, Italy).

The project consists in developing a TinyML model deployed on an Arduino Nano BLE sense rev.2 board to detect different tennis shots through the use of accelerometer and gyroscope data.

---
## Repository structure

### Arduino

This folder contains all the different firmwares used throughout the project, including the one for data acquisition and the final inference firmware. Also, some custom libraries are included.

### data_acquisition

This folder contains the data acquisition software that connects to the racket via BLE and receives the accelerometer data. It has a GUI to visualize the data, label it and save it to a file.
This is also useful to debug the inference firmware.

### dataset

This folder contains the dataset collected using the data acquisition software and used to train the neural network. It is divided into subfolders, one for each type of shot.

### model_pytorch

This folder contains the Pytorch code used to train the neural network.

### model_tf

This folder contains the Tensorflow code used to train the neural network and to convert it to the Tensorflow Lite format to be used on the Arduino.

### raw_datasets

This folder contains the raw data collected from 3 different people.

---

## Demo

<a href="https://www.youtube.com/watch?v=3WyLoOx5gWo">
    <img src="https://img.youtube.com/vi/3WyLoOx5gWo/maxresdefault.jpg" alt="Project demo" width="500"/>
</a>


---

## Authors

Federico Cattaneo - federico9.cattaneo@mail.polimi.it

Francesco Re - [francescore.dev](https://francescore.dev)

---

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.

