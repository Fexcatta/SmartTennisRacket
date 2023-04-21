# BLEDataAcquisition

Program that waits a trigger signal through BLE to acquire data for a set amount of time. Once the time expires the data acquired is sent using BLE. Data is sent as a collection of floats.

Data is sent in this format:

```[accX][accY][accZ][gyrX][gyrY][gyrZ]([id])```

---

In Constants.h it is possible to modify #define values in order to customize the program.

```c++
#define  DATA_INDEX  1 //1- send debug id; 0- only data
```
- If set to (1) the data is sent with an integer identifier.
---
```c++
#define  DATA_INTERVAL_S  10 //how many seconds to acquire data for
```
- This value sets the amount of seconds during which data from sensors is taken.

---
```c++
#define  DATA_RATE_MS  3 //how many milliseconds between reads of the sensors
```
- This value sets the number of milliseconds between each consecutive acquisition of sensor data. 

---
```c++
#define  TIMER_PRESCALER  6 //prescaler value for the timer used for data acquisition (16MHz / 2^(TIMER_PRESCALER)) [MAX 8]
```
- This value changes the timer clock used to calculate time for data acquisitions. It follows the following formula:
	$ TCLK = 16MHz / 2 ^ {PRESCALER} $
