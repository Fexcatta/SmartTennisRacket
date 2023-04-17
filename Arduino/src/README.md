# BLEliveData

Program that acquires data and sends it over BLE instantly.


# BLEbufferData

Program that waits a trigger signal through BLE to acquire data for a set amount of time. Once the time expires the data acquired is sent using BLE.

The strings are formatted as:

```
%+06.2f;%+06.2f;%+06.2f;%+08.2f;%+08.2f;%+08.2f;%07d

+00.25;+01.02;-00.65;+0000.38;-1235.73;+0049.38;0000234
accX  ; accY ; accZ ;  gyrX  ;  gyrY  ;  gyrZ  ;   id
```

The trigger for the start of data acquisition is a boolean BLECharacteristic that can be written and read. When set ('1') the data acquisition starts. It is then cleared once all data has been sent through BLE. It can be read to check if the transmission has ended and the MCU is able to start a new acquisition.
