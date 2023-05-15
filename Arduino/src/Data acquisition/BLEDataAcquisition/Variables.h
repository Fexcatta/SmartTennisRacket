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


dataStruct data[DATA_SIZE]; //Data acquisition buffer
unsigned int bufferIndex = 0; //Index for the data acquisition buffer

struct packetStruct { 
  dataStruct dataBuffer[STRUCT_NUM];
};
packetStruct packetBuffer; //BLE packet buffer (used for sending chunk of data)
unsigned int packetIndex = 0; //Index for the packet buffer

volatile bool updateSensors = true; //Flag: used for timing acc&gyro data acquisition (changed by Timer3 callback)
bool trigger = false; //Flag: used to signal the start of data acquisition (set by the Central over BLE); cleared when data transmission has ended









#endif