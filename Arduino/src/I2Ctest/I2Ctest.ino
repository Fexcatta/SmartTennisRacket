#include <Arduino_BMI270_BMM150.h>




void setup() {

  Serial.begin(115200);

  IMU.begin();

  Wire1.setClock(400000);

}

void loop() {

  char strBuffer[8];
  float accX, accY, accZ;
  float accX1, accY1, accZ1;
  float gyrX, gyrY, gyrZ;
  unsigned long time1, time2;

  time1 = micros();

  getIMUdata(accX, accY, accZ, gyrX, gyrY, gyrZ);

  time2 = micros() - time1;
  //Serial.println(time2);

  /*
  Serial.print(accX);
  Serial.print('\t');
  Serial.print(accY);
  Serial.print('\t');
  Serial.println(accZ);
  */
  /*
  Serial.print(gyrX);
  Serial.print('\t');
  Serial.print(gyrY);
  Serial.print('\t');
  Serial.println(gyrZ);
  */

  delay(100);

}








void getAcceleration(float &x, float &y, float &z) {

  int8_t acc[5];
  uint8_t bytesReceived;
  int16_t accX, accY, accZ;

  Wire1.beginTransmission(0x68);
  Wire1.write(0x0C);

  if ( Wire1.endTransmission() == 0 ) {
    bytesReceived = Wire1.requestFrom(0x68, 6);
    for (uint8_t i = 0; i < bytesReceived; i++) {
      acc[i] = Wire1.read();
    }
  }



  accX = (acc[1] << 8) + acc[0];
  accY = (acc[3] << 8) + acc[2];
  accZ = (acc[5] << 8) + acc[4];

  x = accX / 2048.0;
  y = accY / 2048.0;
  z = accZ / 2048.0;

}



void getRotation(float &x, float &y, float &z) {

  int8_t gyr[5];
  uint8_t bytesReceived;
  int16_t gyrX, gyrY, gyrZ;

  Wire1.beginTransmission(0x68);
  Wire1.write(0x12);

  if ( Wire1.endTransmission() == 0 ) {
    bytesReceived = Wire1.requestFrom(0x68, 6);
    for (uint8_t i = 0; i < bytesReceived; i++) {
      gyr[i] = Wire1.read();
    }
  }

  gyrX = (gyr[1] << 8) + gyr[0];
  gyrY = (gyr[3] << 8) + gyr[2];
  gyrZ = (gyr[5] << 8) + gyr[4];

  x = gyrX / 16.384;
  y = gyrY / 16.384;
  z = gyrZ / 16.384;

}


void getIMUdata(float &accX, float &accY, float &accZ, float &gyrX, float &gyrY, float &gyrZ) {

  int8_t rawData[11];
  uint8_t bytesReceived;
  int16_t accX_int, accY_int, accZ_int;
  int16_t gyrX_int, gyrY_int, gyrZ_int;

  Wire1.beginTransmission(0x68);
  Wire1.write(0x0C);

  if ( Wire1.endTransmission() == 0 ) {
    bytesReceived = Wire1.requestFrom(0x68, 12);
    for (uint8_t i = 0; i < bytesReceived; i++) {
      rawData[i] = Wire1.read();
    }
  }

  accX_int = (rawData[1] << 8) + rawData[0];
  accY_int = (rawData[3] << 8) + rawData[2];
  accZ_int = (rawData[5] << 8) + rawData[4];

  accX = accX_int / 2048.0;
  accY = accY_int / 2048.0;
  accZ = accZ_int / 2048.0;

  gyrX_int = (rawData[7] << 8) + rawData[6];
  gyrY_int = (rawData[9] << 8) + rawData[8];
  gyrZ_int = (rawData[11] << 8) + rawData[10];

  gyrX = gyrX_int / 16.384;
  gyrY = gyrY_int / 16.384;
  gyrZ = gyrZ_int / 16.384;

}


