# Install dependencies

`pip install -r requirements.txt`

# Run

`python main.py`

# Dataset
The dataset is created in the same folder, and it is composed by 3 directories:
- `forehand`
- `backhand`
- `nothing`

Each sample is saved in CSV format, with the following columns:
- `accX`
- `accY`
- `accZ`
- `gyrX`
- `gyrY`
- `gyrZ`

# Data transmission

## Serial transmission
Data must be transmitted as:
```
%
accX;accY;accZ;gyrX;gyrY;gyrZ;
...
%
```

For example:
```
%
0.125244;0.220703;0.933838;-6.591796;-12.573241;0.854492;
0.118532;0.242432;0.952637;-5.554199;-12.512206;-0.366211;
0.232134;0.271973;0.974854;-4.394531;-10.803222;-3.540039;
0.114624;0.276245;0.986816;-3.845215;-8.361816;-6.958007;
0.116089;0.253662;0.987793;-3.295898;-5.737304;-8.239746;
0.117676;0.210937;0.999146;-0.671387;-2.746582;-6.713867;
...
%
```

# Bluetooth transmission
Data must be transmitted as BLOB packets with number of bytes multiple of 28 (24 without id).
Where each chunk of 28 (24) bytes is composed as follow:
```
[accX][accY][accZ][gyrX][gyrY][gyrZ]([id])
```
First 6 values are in fp32 format.
Id is in int32 format.

All values are transmitted in little endian.