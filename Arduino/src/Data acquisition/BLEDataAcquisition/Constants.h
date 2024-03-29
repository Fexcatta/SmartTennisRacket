#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_





//DEFINE PARAMETERS ----------------------------------

#define DATA_INDEX 1 //1- send debug id; 0- only data
#define DATA_INTERVAL_S 10 //how many seconds to acquire data for
#define DATA_RATE_MS 3 //how many milliseconds between reads of the sensors
#define TIMER_PRESCALER 6 //prescaler value for the timer used for data acquisition (16MHz / 2^(TIMER_PRESCALER)) [MAX 8]

//CONSTANTS ------------------------------------------

constexpr unsigned int STRUCT_SIZE = (DATA_INDEX) ? 28 : 24; //size in bytes of the data struct
constexpr unsigned int STRUCT_NUM = (DATA_INDEX) ? 8 : 10; //how many structs to send in a sigle packet (max 10, max 8 when sending id)
constexpr unsigned int CHARACTERISTIC_SIZE = STRUCT_SIZE*STRUCT_NUM; //BLE buffer size
constexpr unsigned int TIMER_CLOCK = 16000000 / (1<<(TIMER_PRESCALER)); //Timer clock value (16Mhz / 2^(TIMER_PRESCALER))
constexpr unsigned int TIMER_COMPARE_VALUE = (DATA_RATE_MS/1000.0) * TIMER_CLOCK; //Timer Compare register value to have the desired time delay

constexpr unsigned int DATA1 = (unsigned int)( ( (float)DATA_INTERVAL_S/DATA_RATE_MS) * 1000);
constexpr unsigned int DATA2 = (unsigned int)( ( (float)DATA_INTERVAL_S/DATA_RATE_MS) * 1000) % STRUCT_NUM;
constexpr unsigned int DATA_SIZE = (DATA2 != 0) ? DATA1 + (STRUCT_NUM - DATA2) : DATA1; //How many sensor acquisition to take before sending them


//MACROS ---------------------------------------------

#define BLE_SUBSCRIBED_AND_DATA_LEFT_TO_SEND ((packetIndex < DATA_SIZE) && (bufferIndex >= DATA_SIZE) && sensorCharacteristic.subscribed())
#define FINISHED_SENDING_DATA (packetIndex >= DATA_SIZE)
#define TRIGGER_IS_FALSE (!trigger)
#define TRIGGER_IS_TRUE_AND_DATA_BUFFER_NOT_FULL (updateSensors && trigger && bufferIndex < DATA_SIZE)









#endif