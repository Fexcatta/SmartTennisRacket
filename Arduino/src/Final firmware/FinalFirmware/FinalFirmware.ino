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

  //BLEsetup
  BLEsetup();
  BLE.advertise();

  //Model setup
  setupModel();

  STATUS_FLAG = DATA_ACQUISITION;

}


void loop() {

  BLEDevice central = BLE.central();

  if (updateSensors && STATUS_FLAG == DATA_ACQUISITION) { //if enough time has passed since last data acquisition

    acquireData();

  }

  if (STATUS_FLAG == INFERENCE) {

    inference();

  }

  if (STATUS_FLAG == DATA_OUTPUT) {  //if there's a BLE connection

    outputGeneration();

  }

  if (STATUS_FLAG == RESET) {

    resetState();

  }

} //end loop



void acquireData() {

  updateSensors = false;
    
  getIMUdata(sensorData[bufferIndex].accX, sensorData[bufferIndex].accY, sensorData[bufferIndex].accZ, sensorData[bufferIndex].gyrX, sensorData[bufferIndex].gyrY, sensorData[bufferIndex].gyrZ);


  if (TRIGGER_IS_FALSE) {

    if (abs(sensorData[bufferIndex].accX) >= TRIGGER_THRESHOLD_G || abs(sensorData[bufferIndex].accZ) >= TRIGGER_THRESHOLD_G || abs(sensorData[bufferIndex].accZ) >= TRIGGER_THRESHOLD_G) {
      trigger = true;
      bufferStart = bufferIndex;
      samplesSaved = 0;
    }

  } else { //trigger is true

    if (FINISHED_ACQUIRING_DATA) {

      STATUS_FLAG = INFERENCE;
      TMR::tmr3.stopTimer(); //stop data acquisition timer
      TMR::tmr3.clearTimer();

    } else { //data acquisition hasn't ended

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

  TfLiteTensor* model_input = interpreter->input(0);
  
  //Adjust the buffer to the circular index
  if ((inferenceBufferStart = bufferStart - DATA_SIZE_PRE_TRIGGER) < 0) {
    inferenceBufferStart = DATA_SIZE + inferenceBufferStart;
    j = inferenceBufferStart;
  } else {
    j = inferenceBufferStart;
  }
  
  //Setup the inputs to the model
  for (int i = 0; i < DATA_SIZE; i++) {
    model_input->data.f[i * 6 + 0] = sensorData[j].accX;
    model_input->data.f[i * 6 + 1] = sensorData[j].accY;
    model_input->data.f[i * 6 + 2] = sensorData[j].accZ;
    model_input->data.f[i * 6 + 3] = sensorData[j].gyrX;
    model_input->data.f[i * 6 + 4] = sensorData[j].gyrY;
    model_input->data.f[i * 6 + 5] = sensorData[j].gyrZ;

    j = (j + 1) % DATA_SIZE;
  }

  //Perform inference
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    MicroPrintf("Invoke failed");
    return;
  }

  TfLiteTensor* output = interpreter->output(0);

  //Save last inference results
  for (uint8_t i = 0; i < 4; i++) {
    lastInferenceOutput[i] = output->data.f[i];
  }

  STATUS_FLAG = DATA_OUTPUT;

}


void updateBLEdata() {

  char str_buf[128];

  snprintf(str_buf, 128, "Forehand: %.4f\n Backhand: %.4f\n Nothing: %.4f\n Serve: %.4f\n", lastInferenceOutput[0], lastInferenceOutput[1], lastInferenceOutput[2], lastInferenceOutput[3]);

  debugCharacteristic.writeValue(str_buf);

  snprintf(str_buf, 128, "Forehand: %i\n Backhand: %i\n Nothing: %i\n Serve: %i\n", outputData.forehand, outputData.backhand, outputData.nothing, outputData.serve);

  outputCharacteristic.writeValue(str_buf);

  STATUS_FLAG = RESET;

}


void resetState() {

  samplesSaved = 0;
  trigger = false; //update trigger flag
  TMR::tmr3.startTimer();

  STATUS_FLAG = DATA_ACQUISITION;

}


void outputGeneration() {

  if (lastInferenceOutput[0] >= INFERENCE_THRESHOLD_PROBABILITY) {
    outputData.forehand += 1;
  } else if (lastInferenceOutput[1] >= INFERENCE_THRESHOLD_PROBABILITY) {
    outputData.backhand += 1;
  } else if (lastInferenceOutput[3] >= INFERENCE_THRESHOLD_PROBABILITY) {
    outputData.serve += 1;
  } else {
    outputData.nothing += 1;
  }

  //Update BLE data
  char str_buf[128];

  snprintf(str_buf, 128, "Forehand: %.4f\n Backhand: %.4f\n Nothing: %.4f\n Serve: %.4f\n", lastInferenceOutput[0], lastInferenceOutput[1], lastInferenceOutput[2], lastInferenceOutput[3]);
  debugCharacteristic.writeValue(str_buf);
  
  snprintf(str_buf, 128, "Forehand: %i\n Backhand: %i\n Nothing: %i\n Serve: %i\n", outputData.forehand, outputData.backhand, outputData.nothing, outputData.serve);
  outputCharacteristic.writeValue(str_buf);

  STATUS_FLAG = RESET;

}
















void dataCheck() {

  checkSensors = false;

  getAcceleration(checkData.accX, checkData.accY, checkData.accZ);
  
  if (checkData.accX >= 5 || checkData.accZ >= 5 || checkData.accZ >= 5) {

    trigger = true;
    TMR::tmr3.startTimer(); //start data acquisition timer
    TMR::tmr2.stopTimer(); //stop watchdog timer

  }

}
