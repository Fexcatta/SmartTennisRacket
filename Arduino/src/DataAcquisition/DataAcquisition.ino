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
  TMR::tmr3.startTimer();

  //DEBUG init
  debugInit;

  //BLEsetup
  BLEsetup();
  BLE.advertise();

  debugPrintln("Bluetooth® device active, waiting for connections...");
}


void loop() {

  BLEDevice central = BLE.central();

  if (updateSensors) {

    acquireData();

  }

  if (central) {

    if (sensorCharacteristic.subscribed() && sendFlag) {
      sendData();
    }

    if (resetFlag) {
      resetState();
    }

  }





/*

  //if there's a central device connected
  if (central) {

    //if there's a BLE connection and we haven't received the trigger event, poll for it
    if (TRIGGER_IS_FALSE) {

    } else { //if trigger isn't false
      
      if (BLE_SUBSCRIBED_AND_DATA_LEFT_TO_SEND) {
        sendData();
      }

      if (FINISHED_SENDING_DATA) {
        resetState();
      }

    }//end if TRIGGER_IS_FALSE

  } //end if (central)
  */

} //end loop



void acquireData() {

  updateSensors = false;
    
  getIMUdata(data[bufferIndex].accX, data[bufferIndex].accY, data[bufferIndex].accZ, data[bufferIndex].gyrX, data[bufferIndex].gyrY, data[bufferIndex].gyrZ);

  #if DATA_INDEX
    data[bufferIndex].id = indexTest;
    indexTest += 1;
  #endif


  if (TRIGGER_IS_FALSE) {

    debugPrint("Acquired sample with index: ");
    debugPrintln(bufferIndex);

    if (data[bufferIndex].accX >= 10 || data[bufferIndex].accZ >= 10 || data[bufferIndex].accZ >= 10) {
      trigger = true;
      bufferStart = bufferIndex;
      samplesSaved = 0;
      debugPrint("Data acquisition started with starting index: ");
      debugPrintln(bufferStart);
    }
  } else {



    if (samplesSaved >= DATA_SIZE_POST_TRIGGER-1) {
      bufferEnd = bufferIndex;
      packetIndex = bufferEnd + 1;
      sendFlag = true;
      TMR::tmr3.stopTimer(); //stop data acquisition timer
      TMR::tmr3.clearTimer();
      debugPrint("Saving sample n. ");
      debugPrint(samplesSaved);
      debugPrint("\t");
      debugPrintln(bufferIndex);
      debugPrintln("Finished acquiring data. Waiting for characteristic subscription to send...");
      debugPrint("bufferEnd: ");
      debugPrint(bufferEnd);
      debugPrint("\t");
      debugPrint("packetIndex: ");
      debugPrintln(packetIndex);
    } else {

      debugPrint("Saving sample n. ");
      debugPrint(samplesSaved);
      debugPrint("\t");
      debugPrintln(bufferIndex);

      samplesSaved += 1;
    }

    




  }

  if (bufferIndex < DATA_SIZE-1) {
    bufferIndex += 1;
  } else {
    bufferIndex = 0;
  }
  

  
/*
  if (bufferIndex >= DATA_SIZE) {
    TMR::tmr3.stopTimer(); //stop data acquisition timer
    debugPrintln("Finished acquiring data. Waiting for characteristic subscription to send...");
  }
*/

}

void sendData() {

  for (uint8_t j = 0; j < STRUCT_NUM; j++) {

    if ((packetIndex+j) >= DATA_SIZE) {
      packetIndex = -j;
    }

    packetBuffer.dataBuffer[j] = data[packetIndex+j];

    if (packetIndex + j == bufferEnd) {
      resetFlag = true;
      sendFlag = false;
    }
  }

  debugPrint("Sending packet with packetIndex: ");
  debugPrintln(packetIndex);

  sensorCharacteristic.writeValue((byte *) &packetBuffer, CHARACTERISTIC_SIZE);
  packetIndex += STRUCT_NUM;

}

void resetState() {

  debugPrintln("Resetting state. Ready to acquire new data.");

  packetIndex = 0; //reset packet pointer
  resetFlag = false;
  samplesSaved = 0;
  trigger = false; //update trigger flag
  //bufferIndex = 0; //reset buffer pointer
  #if DATA_INDEX
    indexTest = 0;  //reset identifier
  #endif
  TMR::tmr3.startTimer();

}


void dataCheck() {

  checkSensors = false;

  getAcceleration(checkData.accX, checkData.accY, checkData.accZ);
  
  if (checkData.accX >= 5 || checkData.accZ >= 5 || checkData.accZ >= 5) {

    trigger = true;
    TMR::tmr3.startTimer(); //start data acquisition timer
    TMR::tmr2.stopTimer(); //stop watchdog timer

    debugPrintln("Data acquisition started");

  }

}



