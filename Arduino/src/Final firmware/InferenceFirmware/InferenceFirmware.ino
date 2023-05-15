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

  if (inferenceFlag) {

    inference();

  }


  if (central && sendFlag) {  //if there's a BLE connection

    sendData();

  } else if (!central && sendFlag) {

    resetFlag = true;
  }

  if (resetFlag) {

    resetState();

  }

} //end loop



void acquireData() {

  updateSensors = false;
    
  getIMUdata(sensorData[bufferIndex].accX, sensorData[bufferIndex].accY, sensorData[bufferIndex].accZ, sensorData[bufferIndex].gyrX, sensorData[bufferIndex].gyrY, sensorData[bufferIndex].gyrZ);

  #if DATA_INDEX
    data[bufferIndex].id = indexTest;
    indexTest += 1;
  #endif


  if (TRIGGER_IS_FALSE) {

    debugPrint("Acquired sample with index: ");
    debugPrintln(bufferIndex);

    if (abs(sensorData[bufferIndex].accX) >= TRIGGER_THRESHOLD_G || abs(sensorData[bufferIndex].accZ) >= TRIGGER_THRESHOLD_G || abs(sensorData[bufferIndex].accZ) >= TRIGGER_THRESHOLD_G) {
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
      inferenceFlag = true;
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



void inference() {

  int inferenceBufferStart;
  unsigned int j = 0;

  inferenceFlag = false;

  TfLiteTensor* model_input = interpreter->input(0);
  
  //Adjust the buffer to the circular index
  if ((inferenceBufferStart = bufferStart - 150) < 0) {
    inferenceBufferStart = DATA_SIZE + inferenceBufferStart;
    j = inferenceBufferStart;
  } else {
    j = inferenceBufferStart;
  }

  debugPrint("Index j start: ");
  debugPrintln(j);



  
  for (int i = 0; i < 300; i++) {
    model_input->data.f[i * 6 + 0] = sensorData[j].accX;
    model_input->data.f[i * 6 + 1] = sensorData[j].accY;
    model_input->data.f[i * 6 + 2] = sensorData[j].accZ;
    model_input->data.f[i * 6 + 3] = sensorData[j].gyrX;
    model_input->data.f[i * 6 + 4] = sensorData[j].gyrY;
    model_input->data.f[i * 6 + 5] = sensorData[j].gyrZ;

    debugPrint("Index j: ");
    debugPrintln(j);

    debugPrint("Index i * 6: ");
    debugPrintln(i * 6);

    j = (j + 1) % DATA_SIZE;

  }

  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    MicroPrintf("Invoke failed");
    return;
  }

  TfLiteTensor* output = interpreter->output(0);

  for (uint8_t i = 0; i < 4; i++) {
    lastInferenceOutput[i] = output->data.f[i];
  }

  debugPrint("Forehand: ");
  debugPrintln(output->data.f[0]);
  
  debugPrint("Backhand: ");
  debugPrintln(output->data.f[1]);
  
  debugPrint("Nothing: ");
  debugPrintln(output->data.f[2]);
  
  debugPrint("Serve: ");
  debugPrintln(output->data.f[3]);





  //TODO: get ouputs and process them
  //TODO: exit the function and reset state

  sendFlag = true;


}







void sendData() {

  sendFlag = false;

  char str_buf[128];

  snprintf(str_buf, 128, "Forehand: %.4f\n Backhand: %.4f\n Nothing: %.4f\n Serve: %.4f\n", lastInferenceOutput[0], lastInferenceOutput[1], lastInferenceOutput[2], lastInferenceOutput[3]);

  outputCharacteristic.writeValue(str_buf);

  resetFlag = true;


}

void resetState() {

  resetFlag = false;
  sendFlag = false;

  debugPrintln("Resetting state. Ready to acquire new data.");

  packetIndex = 0; //reset packet pointer
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



