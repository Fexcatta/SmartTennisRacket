#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_



//DEFINE PARAMETERS ----------------------------------

#define DATA_SIZE 300 //how many IMU acquisitions in total
#define DATA_SIZE_PRE_TRIGGER 150 //how many acquisition to save prior to the trigger
#define DATA_RATE_MS 3 //how many milliseconds between reads of the sensors
#define TRIGGER_THRESHOLD_G 13 //how many gs to detect before saving and sending data
#define INFERENCE_THRESHOLD_PROBABILITY 0.8 //minimum inference output certainty in order to save a shot
#define TIMER_PRESCALER 6 //prescaler value for the timer used for data acquisition (16MHz / 2^(TIMER_PRESCALER)) [MAX 8]


//CONSTANTS ------------------------------------------

constexpr unsigned int TIMER_CLOCK = 16000000 / (1<<(TIMER_PRESCALER)); //Timer clock value (16Mhz / 2^(TIMER_PRESCALER))
constexpr unsigned int TIMER_COMPARE_VALUE = (DATA_RATE_MS/1000.0) * TIMER_CLOCK; //Timer Compare register value to have the desired time delay

constexpr unsigned int DATA_SIZE_POST_TRIGGER = DATA_SIZE - DATA_SIZE_PRE_TRIGGER; //Buffer size for data taken after trigger


//MACROS ---------------------------------------------

#define FINISHED_SENDING_DATA (packetIndex >= DATA_SIZE)
#define TRIGGER_IS_FALSE (!trigger)
#define FINISHED_ACQUIRING_DATA (samplesSaved >= DATA_SIZE_POST_TRIGGER-1)



#endif