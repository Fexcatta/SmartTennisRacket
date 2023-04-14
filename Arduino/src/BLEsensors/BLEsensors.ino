#define ARDUINO_BLE_SENSE_LITE

//#import <nRF52840_Timers.h>
#import <ArduinoBLE.h>

#ifdef ARDUINO_BLE_SENSE_LITE
  #import <Arduino_LSM9DS1.h>
#endif

#define DEBUG


BLEService accService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEService gyrService("19B1F000-E8F2-537E-4F6C-D104768A1214");

BLEFloatCharacteristic accXCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic accYCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic accZCharacteristic("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

BLEFloatCharacteristic gyrXCharacteristic("19B1F001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic gyrYCharacteristic("19B1F002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic gyrZCharacteristic("19B1F003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

//BLEDescriptor accServiceDescriptor("2901", "Accelerometer");

BLEDescriptor accXDescriptor("2901", "Accelerometer X Data");


float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

bool accNewData = false;
bool gyrNewData = false;




void setup() {
  
  //SERIAL init
  Serial.begin(115200);
 

  //IMU init
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }


  //BLE setup
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("BLEsensors");
  BLE.setAdvertisedService(accService);

  

  accService.addCharacteristic(accXCharacteristic);
  accService.addCharacteristic(accYCharacteristic);
  accService.addCharacteristic(accZCharacteristic);

  gyrService.addCharacteristic(gyrXCharacteristic);
  gyrService.addCharacteristic(gyrYCharacteristic);
  gyrService.addCharacteristic(gyrZCharacteristic);

  accXCharacteristic.addDescriptor(accXDescriptor);
  accYCharacteristic.addDescriptor(accXDescriptor);
  accZCharacteristic.addDescriptor(accXDescriptor);

  BLE.addService(accService);
  BLE.addService(gyrService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  accXCharacteristic.setValue(0.0);
  accYCharacteristic.setValue(0.0);
  accZCharacteristic.setValue(0.0);

  gyrXCharacteristic.setValue(0.0);
  gyrYCharacteristic.setValue(0.0);
  gyrZCharacteristic.setValue(0.0);

  

  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));

}

void loop() {

  BLE.poll();
  

  #ifdef ARDUINO_BLE_SENSE_LITE
    if(IMU.accelerationAvailable()) {
      IMU.readAcceleration(accX, accY, accZ);
      accNewData = true;
    }

    if(IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gyrX, gyrY, gyrZ);
      gyrNewData = true;
    }
  #endif

  if (accNewData && BLE.connected()) {

    accXCharacteristic.setValue(accX);
    accYCharacteristic.setValue(accY);
    accZCharacteristic.setValue(accZ);
    accNewData = false;

  }

  if (gyrNewData && BLE.connected()) {

    gyrXCharacteristic.setValue(gyrX);
    gyrYCharacteristic.setValue(gyrY);
    gyrZCharacteristic.setValue(gyrZ);
    gyrNewData = false;

  }




  #ifdef DEBUG
    Serial.print("Acceleration [g]: ");
    Serial.print("X = ");
    Serial.print(accX);
    Serial.print(" Y = ");
    Serial.print(accY);
    Serial.print(" Z = ");
    Serial.println(accZ);

    Serial.print("Rotation [DPS]: ");
    Serial.print("X = ");
    Serial.print(gyrX);
    Serial.print(" Y = ");
    Serial.print(gyrY);
    Serial.print(" Z = ");
    Serial.println(gyrZ);

    //delay(500);
  #endif




}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}


