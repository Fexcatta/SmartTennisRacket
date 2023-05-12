#ifndef _VARIABLES_H_
#define _VARIABLES_H_


#if DATA_INDEX
struct dataStruct {
  float accX;
  float accY;
  float accZ;
  float gyrX;
  float gyrY;
  float gyrZ;
  unsigned int id;
};

unsigned int indexTest = 0;

#else
struct dataStruct {
  float accX;
  float accY;
  float accZ;
  float gyrX;
  float gyrY;
  float gyrZ;
};
#endif


dataStruct sensorData[DATA_SIZE]; //Data acquisition buffer
dataStruct checkData; //Data check buffer
unsigned int bufferIndex = 0; //Index for the data acquisition buffer
unsigned int bufferStart; //Start of the buffer
unsigned int bufferEnd; //End of the buffer
unsigned int samplesSaved = 0;


struct packetStruct { 
  dataStruct dataBuffer[STRUCT_NUM];
};
packetStruct packetBuffer; //BLE packet buffer (used for sending chunk of data)
int packetIndex = 0; //Index for the packet buffer

volatile bool updateSensors = true; //Flag: used for timing acc&gyro data acquisition (changed by Timer3 callback)
volatile bool checkSensors = true; //FLAG: used for timing acc&gyro data acquisition to wake up the data acquisition (changed by the Timer3 callback)
bool trigger = false; //Flag: used to signal the start of data acquisition (set by the Central over BLE); cleared when data transmission has ended
bool resetFlag = false;
bool sendFlag = false;
bool inferenceFlag = false;









#endif