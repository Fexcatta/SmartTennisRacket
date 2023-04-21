# BLEliveData

Program that acquires data and sends it over BLE instantly.


# BLEbufferData

Program that waits a trigger signal through BLE to acquire data for a set amount of time. Once the time expires the data acquired is sent using BLE. Data is sent through a string.

The strings are formatted as:

```
%+06.2f;%+06.2f;%+06.2f;%+08.2f;%+08.2f;%+08.2f;%07d

+00.25;+01.02;-00.65;+0000.38;-1235.73;+0049.38;0000234
accX  ; accY ; accZ ;  gyrX  ;  gyrY  ;  gyrZ  ;   id
```

The trigger for the start of data acquisition is a boolean BLECharacteristic that can be written and read. When set ('1') the data acquisition starts. It is then cleared once all data has been sent through BLE. It can be read to check if the transmission has ended and the MCU is able to start a new acquisition.


# BLEbufferDataFloat

Program that waits a trigger signal through BLE to acquire data for a set amount of time. Once the time expires the data acquired is sent using BLE. Data is sent as a collection of floats.

The define values change how the program runs.

```c++
#define DEBUG_INDEX 1 //1- send debug id, 0- only data
```
If set to (1) the data is sent with an integer identifier.

```c++
#define STRUCT_NUM 8 //how many structs to send in a sigle packet (MAX 10 with no identifier or MAX 8 with identifier)
```
This value allows to edit how many consecutive data aquisitions to send using a single BLE packet. Each float struct is 28 or 24 bytes long, which means there's a maximum of 8 or 10 structs per packet (BLE specification allows max 248 bytes of data).

```c++
#define DATA_SIZE 1000 //how many acquisition to take before sending them
```
This value sets the amount of acquisitions after which the data is sent.
