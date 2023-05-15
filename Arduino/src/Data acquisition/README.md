# DataAcquisition

Program that continously acquires data and once an acceleration threshold has been reached, it sends set amount of data (user defined). Data is sent through BLE as a collection of floats and an optional identifier integer.

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
#define DATA_INTERVAL_PRE_TRIGGER_S 1 //how many seconds to save prior to the trigger
```
- This value sets the amount of seconds of data saved before the threshold trigger.

---
```c++
#define  DATA_RATE_MS  3 //how many milliseconds between reads of the sensors
```
- This value sets the number of milliseconds between each consecutive acquisition of sensor data. 

---
```c++
#define TRIGGER_THRESHOLD_G 10 //how many gs to detect before saving and sending data
```
- This value sets the threshold (in acceleration gs) in order to save data and send it.

---
```c++
#define  TIMER_PRESCALER  6 //prescaler value for the timer used for data acquisition (16MHz / 2^(TIMER_PRESCALER)) [MAX 8]
```
- This value changes the timer clock used to calculate time for data acquisitions. It follows the following formula:
	*TIMER_CLOCK = 16MHz / 2^(PRESCALER)*



---

# BLEDataAcquisition

Program that waits a trigger signal through BLE to acquire data for a set amount of time. Once the time expires the data acquired is sent using BLE. Data is sent as a collection of floats and an optional identifier integer.

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
	*TIMER_CLOCK = 16MHz / 2^(PRESCALER)*
