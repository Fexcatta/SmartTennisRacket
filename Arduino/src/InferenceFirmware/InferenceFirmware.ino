#define ARDUINO_BLE_SENSE

#ifdef ARDUINO_BLE_SENSE
  #include <Arduino_BMI270_BMM150.h>
#endif


#include <ArduinoBLE.h>
#include <nRF52840_Timers.h>

#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "IMUfun.h"
#include "BLEconfig.h"
#include "Debug.h"
#include "Constants.h"
#include "Variables.h"
#include "Timer.h"
#include "configModel.h"
#include "model.h"







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

  //DEBUG init MACRO
  debugInit;

  //BLEsetup
  BLEsetup();
  BLE.advertise();

  //Model setup
  setupModel();

}


void loop() {

  BLEDevice central = BLE.central();

  if (updateSensors) { //if enough time has passed since last data acquisition

    acquireData();

  }

  if (central) {  //if there's a BLE connection

    if (BLE_SUBSCRIBED_AND_DATA_NEEDS_TO_BE_SENT) {
      sendData();
    }

    if (RESET_STATE_AFTER_DATA_IS_SENT) {
      resetState();
    }

  }

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

    if (data[bufferIndex].accX >= TRIGGER_THRESHOLD_G || data[bufferIndex].accZ >= TRIGGER_THRESHOLD_G || data[bufferIndex].accZ >= TRIGGER_THRESHOLD_G) {
      trigger = true;
      bufferStart = bufferIndex;
      samplesSaved = 0;
      debugPrint("Data acquisition started with starting index: ");
      debugPrintln(bufferStart);
    }
  } else { //trigger is true

    if (samplesSaved >= DATA_SIZE_POST_TRIGGER-1) { //finished acquiring data
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

    } else { //data acquisition hasn't ended

      debugPrint("Saving sample n. ");
      debugPrint(samplesSaved);
      debugPrint("\t");
      debugPrintln(bufferIndex);

      samplesSaved += 1;
    }

  }

  //Circular buffer index
  if (bufferIndex < DATA_SIZE-1) {
    bufferIndex += 1;
  } else {
    bufferIndex = 0;
  }

}

void sendData() {

  //Create packet data
  for (uint8_t j = 0; j < STRUCT_NUM; j++) {

    if ((packetIndex+j) >= DATA_SIZE) { //circular buffer correction
      packetIndex = -j;
    }

    packetBuffer.dataBuffer[j] = data[packetIndex+j];

    if (packetIndex + j == bufferEnd) { //detect all data has been sent
      resetFlag = true;
      sendFlag = false;
    }\
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



