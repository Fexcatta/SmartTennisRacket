#define ARDUINO_BLE_SENSE

#include <ArduinoBLE.h>

#ifdef ARDUINO_BLE_SENSE
  #include <Arduino_BMI270_BMM150.h>
#endif

#define BUFFER_SIZE 50

const char *uuidService = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *uuidCharacteristic = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *namePeripheral = "Arduino Nano 33 BLE";


BLEService sensorService(uuidService);

BLECharacteristic sensorCharacteristic(uuidCharacteristic, BLERead | BLENotify, BUFFER_SIZE, 0);





float accX, accY, accZ;
float gyrX, gyrY, gyrZ;

bool accNewData = false;
bool gyrNewData = false;

volatile bool updateSensors = true;
char stringBuffer[50];
unsigned long indexTest = 0;

/*
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long time4 = 0;
unsigned long timeX = 0;
bool timeFlag = true;
*/


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

  BLE.setLocalName(namePeripheral);
  BLE.setAdvertisedService(sensorService);

  sensorService.addCharacteristic(sensorCharacteristic);
  BLE.addService(sensorService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  sensorCharacteristic.setValue("test");

  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));


}

void loop() {

  BLE.poll();


  if (IMU.accelerationAvailable()) {

      //timeLoopBack();
      //time3 = micros();
      IMU.readAcceleration(accX, accY, accZ);
      //time4 = micros() - time3;
      //Serial.println(time4);
      accNewData = true;
  }


  if(IMU.gyroscopeAvailable()) {
      //time3 = micros();
      //timeLoopBack();

      IMU.readGyroscope(gyrX, gyrY, gyrZ);
      gyrNewData = true;
      //Serial.println(micros()-time3);
  }

  


  if ((accNewData || gyrNewData) && BLE.connected()) {

    

    sprintf(stringBuffer, "%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%d", accX, accY, accZ, gyrX, gyrY, gyrZ, indexTest);
    sensorCharacteristic.setValue(stringBuffer);

    

    indexTest += 1;
    if(indexTest > 4294967294) indexTest = 0; 
    if (accNewData) accNewData = false;
    if (gyrNewData) gyrNewData = false;

    //timeLoopBack();
  }

  //timeLoopBack();

}




void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  BLE.stopAdvertise();
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}






/*
void timeLoopBack() {

  if (timeFlag) {
        time1 = micros();
        timeFlag = false;
      } else {
        time2 = micros() - time1;
        if(time2 > timeX) {
          timeX = time2;
          //Serial.println(timeX);
        }
        Serial.println(time2);
        timeFlag = true;
      }

}


*/





