#define ARDUINO_BLE_SENSE

#ifdef ARDUINO_BLE_SENSE
  #include <Arduino_BMI270_BMM150.h>
#endif


#include <ArduinoBLE.h>
#include <nRF52840_Timers.h>

#include "IMUfun.h"
#include "BLEconfig.h"
#include "Debug.h"
#include "Constants.h"
#include "Variables.h"
#include "Timer.h"



void setup() {

  //SERIAL init
  Serial.begin(115200);

  //IMU init
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //TIMER setup
  TMR::init();

  //DEBUG init
  debugInit;

  //BLEsetup
  BLEsetup();
  BLE.advertise();

  debugPrintln("Bluetooth® device active, waiting for connections...");
}


void loop() {

  BLEDevice central = BLE.central();

  if (TRIGGER_IS_TRUE_AND_DATA_BUFFER_NOT_FULL) {

    acquireData();
  }

  //if there's a central device connected
  if (central) {

    //if there's a BLE connection and we haven't received the trigger event, poll for it
    if (TRIGGER_IS_FALSE) {

      if(triggerCharacteristic.written()) { //if the trigger has been written
        if(triggerCharacteristic.value()){ //if the trigger is true
          trigger = true;
          TMR::tmr3.clearTimer();
          TMR::tmr3.startTimer();
          debugPrintln("Starting to acquire data...");
        } else { //the trigger is false
          trigger = false;
        }
      }

    } else { //if trigger isn't false
      
      if (BLE_SUBSCRIBED_AND_DATA_LEFT_TO_SEND) {
        sendData();
      }

      if (FINISHED_SENDING_DATA) {
        resetState();
      }

    }//end if TRIGGER_IS_FALSE

  } //end if (central)

} //end loop



void acquireData() {

  updateSensors = false;
    
  getIMUdata(data[bufferIndex].accX, data[bufferIndex].accY, data[bufferIndex].accZ, data[bufferIndex].gyrX, data[bufferIndex].gyrY, data[bufferIndex].gyrZ);

  #if DATA_INDEX
    data[bufferIndex].id = indexTest;
    indexTest += 1;
  #endif

  debugPrint("Acquiring sample n. ");
  debugPrintln(bufferIndex);

  bufferIndex += 1;

  if (bufferIndex >= DATA_SIZE) {
    TMR::tmr3.stopTimer(); //stop timer
    debugPrintln("Finished acquiring data. Waiting for characteristic subscription to send...");
  }

}

void sendData() {

  for (uint8_t j = 0; j < STRUCT_NUM; j++) {
    packetBuffer.dataBuffer[j] = data[packetIndex+j];
  }

  debugPrint("Sending packet n. ");
  debugPrintln(packetIndex/STRUCT_NUM);

  sensorCharacteristic.writeValue((byte *) &packetBuffer, CHARACTERISTIC_SIZE);
  packetIndex += STRUCT_NUM;

}

void resetState() {

  debugPrintln("Resetting state. Ready to acquire new data.");

  packetIndex = 0; //reset packet pointer
  trigger = false; //update trigger flag
  triggerCharacteristic.writeValue(trigger); //send updated trigger flag
  bufferIndex = 0; //reset buffer pointer
  #if DATA_INDEX
    indexTest = 0;  //reset identifier
  #endif

}





