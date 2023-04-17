#define ARDUINO_BLE_SENSE
#define PRINT_GYR

#ifdef ARDUINO_BLE_SENSE
  #include <Arduino_BMI270_BMM150.h>
#endif

#include "IMUfun.h"
#include <ArduinoBLE.h>
#include <nRF52840_Timers.h>

#define BUFFER_SIZE 55
#define DATA_SIZE 1000


const char *uuidService = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *uuidSensorCharacteristic = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *uuidTriggerCharacteristic = "19B10002-E8F2-537E-4F6C-D104768A1214";
const char *namePeripheral = "Arduino Nano 33 BLE";

BLEService sensorService(uuidService);

BLECharacteristic sensorCharacteristic(uuidSensorCharacteristic, BLERead | BLEIndicate, BUFFER_SIZE, 0);
BLEBoolCharacteristic triggerCharacteristic(uuidTriggerCharacteristic, BLERead | BLEWrite);

Timer tmr3(3, TIMER_MODE, bitMode_16);

char dataBuffer[DATA_SIZE][BUFFER_SIZE+1];



char stringBuffer[56];
bool newData = false;
volatile bool updateSensors = true;
unsigned long indexTest = 0;
bool trigger = false;
unsigned int bufferIndex = 0;
unsigned int i = 0;

float accX, accY, accZ;
float gyrX, gyrY, gyrZ;


unsigned long time1, time2;



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
  tmr3.setCompareWithInterrupt(0, 1250); //3ms - 375 | 10ms - 1250 | 12ms - 1500 | 15ms - 1875 | 20ms - 2500 | 22ms - 2750 | 30ms - 3750
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

    updateSensors = false;
    getIMUdata(accX, accY, accZ, gyrX, gyrY, gyrZ);
    sprintf(stringBuffer, "%+06.2f;%+06.2f;%+06.2f;%+08.2f;%+08.2f;%+08.2f;%07d", accX, accY, accZ, gyrX, gyrY, gyrZ, indexTest);
    strcpy(dataBuffer[bufferIndex], stringBuffer);
    bufferIndex += 1;

    indexTest += 1; 

  }


  if (trigger && bufferIndex >= DATA_SIZE && BLE.connected()) {  //buffer is full, start sending data

    time1 = micros();

    tmr3.stopTimer(); //stop timer
    sensorCharacteristic.writeValue(dataBuffer[i]);
    i += 1;
    

    time2 = micros() - time1;
    Serial.print(bufferIndex);
    Serial.print("\t");
    Serial.println(time2);


  }

  if (trigger && i >= DATA_SIZE) {

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










