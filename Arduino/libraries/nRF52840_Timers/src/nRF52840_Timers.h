

#ifndef _nRF52840_Timers_H_
#define _nRF52840_Timers_H_


#include "Arduino.h"

#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This library only supports boards with a nRF52840 microcontroller. Please check your Tools->Board setting.
#endif



enum : uint8_t {
	TIMER_MODE,
	COUNTER_MODE
};

enum : uint8_t {
	bitMode_16,
	bitMode_8,
	bitMode_24,
	bitMode_32
};



class Timer {

	public:

		Timer(uint8_t timerIndex, uint8_t timerMode = TIMER_MODE, uint8_t bitMode = bitMode_16);

		void init(uint8_t timerIndex, uint8_t timerMode = TIMER_MODE, uint8_t bitMode = bitMode_16);

		void begin(uint8_t timerPrescaler = 0);

		void setCompare(uint8_t compareIndex, unsigned long value);

		void setCompareWithInterrupt(uint8_t compareIndex, unsigned long value);

		void autoClear(uint8_t compareIndex);

		void noAutoClear(uint8_t compareIndex);

		void setPrescaler(uint8_t timerPrescaler);


		void startTimer();

		void stopTimer();

		void count();


		void attachInterrupt(void (*Callback)());

		void clearCompareEvent(uint8_t compareIndex);

		bool compareEventFlag(uint8_t compareIndex);


	private:
		
		uint8_t _timerIndex;
		uint8_t _timerMode;
		uint8_t _bitMode;

};
















#endif