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
unsigned int bufferIndex = 0; //Index for the data acquisition buffer
unsigned int bufferStart; //Start of the buffer

unsigned int samplesSaved = 0; //Keep count of the samples that have been already saved

float lastInferenceOutput[4];


volatile bool updateSensors = true; //Flag: used for timing acc&gyro data acquisition (changed by Timer3 callback)
bool trigger = false; //FLAG: used to signal that data must be saved (acceleration threshold has been reached)

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