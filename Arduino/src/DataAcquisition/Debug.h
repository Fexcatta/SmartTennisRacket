#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "Constants.h"
#include "Variables.h"

#define DEBUG 0 //1- DEBUG mode on; 0- DEBUG mode off

#if DEBUG

  #define debugPrint(x) Serial.print(x)
  #define debugPrintln(x) Serial.println(x)
  #define debugInit Debug::init()

  namespace Debug {

    //DEBUG variables
    unsigned long time1, time2, timeX;
    unsigned long tmr4CC1, tmr4CC2;
    float tmr4CC;

    Timer tmr4(4, TIMER_MODE, bitMode_32); //DEBUG: Timer for time


    void timer4Setup() {
        tmr4.begin(0);
        tmr4.startTimer();
    }

    void timeTest() {

      tmr4CC2 = tmr4.captureTimer(0);
      tmr4CC = (tmr4CC2 - tmr4CC1) / 16.0;
      timeX += tmr4CC;
      Serial.print(packetIndex);
      Serial.print("\t");
      Serial.print(timeX);
      Serial.print("\t");
      Serial.println(tmr4CC);
      tmr4.clearTimer();

    }

    void timeTestInit() {
      tmr4CC1 = tmr4.captureTimer(0);
    }

    void init() {
      timer4Setup();
    }

  }

#else

  #define debugPrint(x)
  #define debugPrintln(x)
  #define debugInit

#endif



  
#endif