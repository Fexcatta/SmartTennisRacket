#ifndef _BLECONFIG_H_
#define _BLECONFIG_H_

#import "Arduino.h"
#import "ArduinoBLE.h"
#import "Constants.h"


const char *uuidService = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *uuidOutputCharacteristic = "19B10003-E8F2-537E-4F6C-D104768A1214";
const char *uuidDebugCharacteristic = "19B10004-E8F2-537E-4F6C-D104768A1214";
const char *namePeripheral = "Arduino Nano 33 BLE";


BLEService sensorService(uuidService);

BLECharacteristic outputCharacteristic(uuidOutputCharacteristic, BLERead | BLEIndicate, 128, 0);
BLECharacteristic debugCharacteristic(uuidDebugCharacteristic, BLERead | BLEIndicate, 128, 0);


void BLEsetup() {

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName(namePeripheral);
  BLE.setAdvertisedService(sensorService);


  sensorService.addCharacteristic(outputCharacteristic);
  sensorService.addCharacteristic(debugCharacteristic);
  BLE.addService(sensorService);

  outputCharacteristic.setValue("test");
  debugCharacteristic.setValue("test");

}




#endif