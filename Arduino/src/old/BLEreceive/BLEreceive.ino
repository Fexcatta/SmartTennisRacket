#include <ArduinoBLE.h>

//#define ARDUINO_BLE_SENSE

float accX, accY, accZ;

void setup() {

  //SERIAL init
  Serial.begin(115200);

  //BLE setup
  if(BLE.begin()) {
    Serial.println("Bluetooth® started");
  }
  
  Serial.println("Scanning for device...");
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");

}

void loop() {
  
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    BLE.stopScan();

    BLEReceiveData(peripheral);

    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");

  }



}


void BLEReceiveData(BLEDevice peripheral) {

  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }   


  BLECharacteristic accXCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic accYCharacteristic = peripheral.characteristic("19b10002-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic accZCharacteristic = peripheral.characteristic("19b10003-e8f2-537e-4f6c-d104768a1214");

  Serial.println("Subscribing to accXCharacteristic ...");
  if (!accXCharacteristic) {
    Serial.println("Characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!accXCharacteristic.canSubscribe()) {
    Serial.println("Characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!accXCharacteristic.subscribe()) {
    Serial.println("Subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
  }

  Serial.println("Subscribing to accYCharacteristic ...");
  if (!accYCharacteristic) {
    Serial.println("Characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!accYCharacteristic.canSubscribe()) {
    Serial.println("Characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!accYCharacteristic.subscribe()) {
    Serial.println("Subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
  }

  Serial.println("Subscribing to accZCharacteristic ...");
  if (!accZCharacteristic) {
    Serial.println("Characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!accZCharacteristic.canSubscribe()) {
    Serial.println("Characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!accZCharacteristic.subscribe()) {
    Serial.println("Subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
  }

  while (peripheral.connected()) {
    // while the peripheral is connected
    
    // check if the value of the characteristic has been updated
    if (accXCharacteristic.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      
      accXCharacteristic.readValue(&accX, 4);
    }

    // check if the value of the characteristic has been updated
    if (accYCharacteristic.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      
      accYCharacteristic.readValue(&accY, 4);
    }

    // check if the value of the characteristic has been updated
    if (accZCharacteristic.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      
      accZCharacteristic.readValue(&accZ, 4);
    }


    Serial.print(accX);
    Serial.print('\t');
    Serial.print(accY);
    Serial.print('\t');
    Serial.println(accZ);

  }


  Serial.println("Device disconnected!");

}



