#define ARDUINO_BLE_SENSE
#define PRINT_GYR

#ifdef ARDUINO_BLE_SENSE
  #include <Arduino_BMI270_BMM150.h>
#endif

#include "IMUfun.h"
#include <ArduinoBLE.h>
#include <nRF52840_Timers.h>

#define BUFFER_SIZE 55


const char *uuidService = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *uuidCharacteristic = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *namePeripheral = "Arduino Nano 33 BLE";

BLEService sensorService(uuidService);

BLECharacteristic sensorCharacteristic(uuidCharacteristic, BLERead | BLENotify, BUFFER_SIZE, 0);

Timer tmr3(3, TIMER_MODE, bitMode_16);

char stringBuffer[56];
bool newData = false;
volatile bool updateSensors = true;
unsigned long indexTest = 0;

float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

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
  tmr3.setCompareWithInterrupt(0, 2750); //3ms - 375 | 10ms - 1250 | 12ms - 1500 | 15ms - 1875 | 20ms - 2500 | 22ms - 2750
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
  BLE.addService(sensorService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  sensorCharacteristic.setValue("test");

  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));


  tmr3.startTimer();
}

void loop() {

  BLE.poll();

  if (updateSensors) {
    getIMUdata(accX, accY, accZ, gyrX, gyrY, gyrZ);
    newData = true;
    updateSensors = false;
  }
  

  if (newData && BLE.connected()) {

    sprintf(stringBuffer, "%+06.2f;%+06.2f;%+06.2f;%+08.2f;%+08.2f;%+08.2f;%07d", accX, accY, accZ, gyrX, gyrY, gyrZ, indexTest);

    sensorCharacteristic.setValue(stringBuffer);

    indexTest += 1;
    if(indexTest > 4294967294) indexTest = 0; 
    newData = false;

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










