#ifndef _BLECONFIG_H_
#define _BLECONFIG_H_

#import "Arduino.h"
#import "ArduinoBLE.h"
#import "Constants.h"


const char *uuidService = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *uuidSensorCharacteristic = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *uuidTriggerCharacteristic = "19B10002-E8F2-537E-4F6C-D104768A1214";
const char *namePeripheral = "Arduino Nano 33 BLE";


BLEService sensorService(uuidService);

BLECharacteristic sensorCharacteristic(uuidSensorCharacteristic, BLERead | BLEIndicate, CHARACTERISTIC_SIZE, 0);
BLEBoolCharacteristic triggerCharacteristic(uuidTriggerCharacteristic, BLERead | BLEWrite);


void BLEsetup() {

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName(namePeripheral);
  BLE.setAdvertisedService(sensorService);

  sensorService.addCharacteristic(sensorCharacteristic);
  sensorService.addCharacteristic(triggerCharacteristic);
  BLE.addService(sensorService);

  sensorCharacteristic.setValue("test");
  triggerCharacteristic.setValue(0);

}




#endif