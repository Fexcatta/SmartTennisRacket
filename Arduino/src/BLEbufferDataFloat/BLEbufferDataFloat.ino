#define ARDUINO_BLE_SENSE

#ifdef ARDUINO_BLE_SENSE
  #include <Arduino_BMI270_BMM150.h>
#endif

#include "IMUfun.h"
#include <ArduinoBLE.h>
#include <nRF52840_Timers.h>

#define DEBUG_INDEX 1 //1- send debug id, 0- only data

#if DEBUG_INDEX
  #define STRUCT_SIZE 28
#else
  #define STRUCT_SIZE 24
#endif

#define STRUCT_NUM 8 //how many structs to send in a sigle packet (MAX 10 with no identifier or MAX 8 with identifier)
#define CHARACTERISTIC_SIZE STRUCT_SIZE*STRUCT_NUM  //BLE buffer size
#define DATA_SIZE 1000 //how many acquisition to take before sending them




const char *uuidService = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *uuidSensorCharacteristic = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *uuidTriggerCharacteristic = "19B10002-E8F2-537E-4F6C-D104768A1214";
const char *namePeripheral = "Arduino Nano 33 BLE";

BLEService sensorService(uuidService);

BLECharacteristic sensorCharacteristic(uuidSensorCharacteristic, BLERead | BLEIndicate, CHARACTERISTIC_SIZE, 0);
BLEBoolCharacteristic triggerCharacteristic(uuidTriggerCharacteristic, BLERead | BLEWrite);

Timer tmr3(3, TIMER_MODE, bitMode_16);


#if DEBUG_INDEX
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

dataStruct data[DATA_SIZE];

struct packetStruct {
  dataStruct dataBuffer[STRUCT_NUM];
};

packetStruct packetBuffer;


volatile bool updateSensors = true;
bool trigger = false;
unsigned int bufferIndex = 0;
unsigned int i = 0;


unsigned long time1, time2, timeX;



void setup() {

  //SERIAL init
  Serial.begin(115200);

  //IMU init
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //TIMER setup
  /*<milliseconds> * 125000 = <compareValue> */
  tmr3.begin(7); //125 KHz clock (16MHz / 2 ^ (PRESCALER))
  tmr3.setCompareWithInterrupt(0, 1000); //3ms - 375 | 8ms - 1000 | 10ms - 1250 | 12ms - 1500 | 15ms - 1875 | 20ms - 2500 | 22ms - 2750 | 30ms - 3750
  tmr3.autoClear(0);
  tmr3.attachInterrupt(TIMER3_Callback);
  
  
  //BLE setup
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName(namePeripheral);
  BLE.setAdvertisedService(sensorService);

  sensorService.addCharacteristic(sensorCharacteristic);
  sensorService.addCharacteristic(triggerCharacteristic);
  BLE.addService(sensorService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  sensorCharacteristic.setValue("test");
  triggerCharacteristic.setValue(trigger);

  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));


  tmr3.startTimer();
}

void loop() {

  BLE.poll();


  //if there's a BLE connection and we haven't received the trigger event, poll for it
  if (!trigger && BLE.connected()) {

    if(triggerCharacteristic.written()) { //if the trigger has been written

      if(triggerCharacteristic.value()){ //if the trigger is true
        trigger = true;
      } else { //the trigger is false
        trigger = false;
      }
    }
  }

  if (updateSensors && trigger && bufferIndex < DATA_SIZE) {
/*
    time1 = micros();
*/

    updateSensors = false;
    
    getIMUdata(data[bufferIndex].accX, data[bufferIndex].accY, data[bufferIndex].accZ, data[bufferIndex].gyrX, data[bufferIndex].gyrY, data[bufferIndex].gyrZ);
    #if DEBUG_INDEX
      data[bufferIndex].id = indexTest;
      indexTest += 1;
    #endif
    bufferIndex += 1;
    
    
    
/*
    time2 = micros() - time1;
    Serial.print(bufferIndex);
    Serial.print("\t");
    Serial.println(time2);
*/
  }


  if (trigger && bufferIndex >= DATA_SIZE && BLE.connected()) {  //buffer is full, start sending data

    tmr3.stopTimer(); //stop timer

    for (uint8_t j = 0; j < STRUCT_NUM; j++) {
      packetBuffer.dataBuffer[j] = data[i+j];
    }


    sensorCharacteristic.writeValue((byte *) &packetBuffer, CHARACTERISTIC_SIZE);
    i += STRUCT_NUM;

  }

  if (trigger && i >= DATA_SIZE) {

    timeX = 0;

    i = 0;
    trigger = false; //update trigger flag
    triggerCharacteristic.writeValue(trigger); //send updated trigger flag
    bufferIndex = 0; //reset buffer pointer
    indexTest = 0;  //reset identifier
    tmr3.startTimer(); //resume timer
  }

}



void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  //BLE.stopAdvertise();
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
 //BLE.advertise();
}

void TIMER3_Callback() {

  if (tmr3.compareEventFlag(0)) {
    tmr3.clearCompareEvent(0);
    updateSensors = true;
  }

}










