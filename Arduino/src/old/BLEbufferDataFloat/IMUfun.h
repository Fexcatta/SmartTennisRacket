#ifndef _IMUfun_H_
#define _IMUfun_H_

#include <Arduino.h>
#include <Wire.h>


#define ACC_X 0
#define ACC_Y 1
#define ACC_Z 2
#define GYR_X 3
#define GYR_Y 4
#define GYR_Z 5


void getAcceleration(float &x, float &y, float &z);

void getRotation(float &x, float &y, float &z);

void getIMUdata(float &accX, float &accY, float &accZ, float &gyrX, float &gyrY, float &gyrZ);


#endif