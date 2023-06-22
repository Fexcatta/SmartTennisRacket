#ifndef _IMUfun_H_
#define _IMUfun_H_

#include <Arduino.h>
#include <Wire.h>


void getAcceleration(float &x, float &y, float &z);

void getRotation(float &x, float &y, float &z);

void getIMUdata(float &accX, float &accY, float &accZ, float &gyrX, float &gyrY, float &gyrZ);


#endif