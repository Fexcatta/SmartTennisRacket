#ifndef _VARIABLES_H_
#define _VARIABLES_H_

struct dataStruct {
  float accX;
  float accY;
  float accZ;
  float gyrX;
  float gyrY;
  float gyrZ;
};


dataStruct sensorData[DATA_SIZE]; //Data acquisition buffer
dataStruct checkData; //Data check buffer
unsigned int bufferIndex = 0; //Index for the data acquisition buffer
unsigned int bufferStart; //Start of the buffer

unsigned int samplesSaved = 0;

float lastInferenceOutput[4];


volatile bool updateSensors = true; //Flag: used for timing acc&gyro data acquisition (changed by Timer3 callback)
volatile bool checkSensors = true; //FLAG: used for timing acc&gyro data acquisition to wake up the data acquisition (changed by the Timer3 callback)
bool trigger = false;

enum statusFlagType : uint8_t {
  DATA_ACQUISITION,
  INFERENCE,
  DATA_OUTPUT,
  RESET
};

statusFlagType STATUS_FLAG;

struct outputStruct {
  unsigned int forehand;
  unsigned int backhand;
  unsigned int nothing;
  unsigned int serve;
};

outputStruct outputData;



#endif