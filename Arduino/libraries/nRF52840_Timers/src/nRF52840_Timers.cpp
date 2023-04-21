#include "nRF52840_Timers.h"

#include "Arduino.h"



Timer::Timer(uint8_t timerIndex, uint8_t timerMode, uint8_t bitMode) {


	init(timerIndex, timerMode, bitMode);

}



void Timer::init(uint8_t timerIndex, uint8_t timerMode, uint8_t bitMode) {
//initialize private class variables

	if(timerIndex < 5) {
		_timerIndex = timerIndex;
	} else {
		_timerIndex = 0;
	}

	if (bitMode == bitMode_16) {
		_bitMode = bitMode_16;
	} else if(bitMode == bitMode_8) {
		_bitMode = bitMode_8;
	} else if (bitMode == bitMode_24) {
		_bitMode = bitMode_24;
	} else if (bitMode == bitMode_32) {
		_bitMode = bitMode_32;
	} else {
		_bitMode = bitMode_16;
	}

	if (timerMode == TIMER_MODE) {
		_timerMode = TIMER_MODE;
	} else if (timerMode == COUNTER_MODE) {
		_timerMode = COUNTER_MODE;
	} else {
		_timerMode = TIMER_MODE;
	}

}


void Timer::begin(uint8_t timerPrescaler) {
//setup the timer (MODE, BIT WIDTH and PRESCALER)

	if (_timerIndex == 0) {

		//TIMER MODE
		if(_timerMode == TIMER_MODE) {
			NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;
		} else if (_timerMode == COUNTER_MODE) {
			NRF_TIMER0->MODE = TIMER_MODE_MODE_LowPowerCounter;
		} else {
			NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;
		}


		//BIT MODE
		if (_bitMode == bitMode_16) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		} else if(_bitMode == bitMode_8) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
		} else if (_bitMode == bitMode_24) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_24Bit;
		} else if (_bitMode == bitMode_32) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
		} else {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		}


		//PRESCALER
		if(timerPrescaler > 8){
			NRF_TIMER0->PRESCALER = 8;
		} else {
			NRF_TIMER0->PRESCALER = timerPrescaler;
		}

		//CLEAR
		NRF_TIMER0->TASKS_CLEAR = 1;


	} else if (_timerIndex == 1) {

		//TIMER MODE
		if(_timerMode == TIMER_MODE) {
			NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
		} else if (_timerMode == COUNTER_MODE) {
			NRF_TIMER1->MODE = TIMER_MODE_MODE_LowPowerCounter;
		} else {
			NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
		}


		//BIT MODE
		if (_bitMode == bitMode_16) {
			NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		} else if(_bitMode == bitMode_8) {
			NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
		} else if (_bitMode == bitMode_24) {
			NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_24Bit;
		} else if (_bitMode == bitMode_32) {
			NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
		} else {
			NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		}


		//PRESCALER
		if(timerPrescaler > 8){
			NRF_TIMER1->PRESCALER = 8;
		} else {
			NRF_TIMER1->PRESCALER = timerPrescaler;
		}

		//CLEAR
		NRF_TIMER1->TASKS_CLEAR = 1;


	} else if (_timerIndex == 2) {

		//TIMER MODE
		if(_timerMode == TIMER_MODE) {
			NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
		} else if (_timerMode == COUNTER_MODE) {
			NRF_TIMER2->MODE = TIMER_MODE_MODE_LowPowerCounter;
		} else {
			NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
		}


		//BIT MODE
		if (_bitMode == bitMode_16) {
			NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		} else if(_bitMode == bitMode_8) {
			NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
		} else if (_bitMode == bitMode_24) {
			NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_24Bit;
		} else if (_bitMode == bitMode_32) {
			NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
		} else {
			NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		}


		//PRESCALER
		if(timerPrescaler > 8){
			NRF_TIMER2->PRESCALER = 8;
		} else {
			NRF_TIMER2->PRESCALER = timerPrescaler;
		}

		//CLEAR
		NRF_TIMER2->TASKS_CLEAR = 1;

	} else if (_timerIndex == 3) {

		//TIMER MODE
		if(_timerMode == TIMER_MODE) {
			NRF_TIMER3->MODE = TIMER_MODE_MODE_Timer;
		} else if (_timerMode == COUNTER_MODE) {
			NRF_TIMER3->MODE = TIMER_MODE_MODE_LowPowerCounter;
		} else {
			NRF_TIMER3->MODE = TIMER_MODE_MODE_Timer;
		}


		//BIT MODE
		if (_bitMode == bitMode_16) {
			NRF_TIMER3->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		} else if(_bitMode == bitMode_8) {
			NRF_TIMER3->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
		} else if (_bitMode == bitMode_24) {
			NRF_TIMER3->BITMODE = TIMER_BITMODE_BITMODE_24Bit;
		} else if (_bitMode == bitMode_32) {
			NRF_TIMER3->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
		} else {
			NRF_TIMER3->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		}


		//PRESCALER
		if(timerPrescaler > 8){
			NRF_TIMER3->PRESCALER = 8;
		} else {
			NRF_TIMER3->PRESCALER = timerPrescaler;
		}

		//CLEAR
		NRF_TIMER3->TASKS_CLEAR = 1;

	} else if (_timerIndex == 4) {

		//TIMER MODE
		if(_timerMode == TIMER_MODE) {
			NRF_TIMER4->MODE = TIMER_MODE_MODE_Timer;
		} else if (_timerMode == COUNTER_MODE) {
			NRF_TIMER4->MODE = TIMER_MODE_MODE_LowPowerCounter;
		} else {
			NRF_TIMER4->MODE = TIMER_MODE_MODE_Timer;
		}


		//BIT MODE
		if (_bitMode == bitMode_16) {
			NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		} else if(_bitMode == bitMode_8) {
			NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
		} else if (_bitMode == bitMode_24) {
			NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_24Bit;
		} else if (_bitMode == bitMode_32) {
			NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
		} else {
			NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		}


		//PRESCALER
		if(timerPrescaler > 8){
			NRF_TIMER4->PRESCALER = 8;
		} else {
			NRF_TIMER4->PRESCALER = timerPrescaler;
		}

		//CLEAR
		NRF_TIMER4->TASKS_CLEAR = 1;

	} else {

		//TIMER MODE
		if(_timerMode == TIMER_MODE) {
			NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;
		} else if (_timerMode == COUNTER_MODE) {
			NRF_TIMER0->MODE = TIMER_MODE_MODE_LowPowerCounter;
		} else {
			NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;
		}


		//BIT MODE
		if (_bitMode == bitMode_16) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		} else if(_bitMode == bitMode_8) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_08Bit;
		} else if (_bitMode == bitMode_24) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_24Bit;
		} else if (_bitMode == bitMode_32) {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
		} else {
			NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
		}


		//PRESCALER
		if(timerPrescaler > 8){
			NRF_TIMER0->PRESCALER = 8;
		} else {
			NRF_TIMER0->PRESCALER = timerPrescaler;
		}

	} //end if-then-else

}







void Timer::setCompare(uint8_t compareIndex, unsigned long value) {
//set value of the Capture/Compare register

	uint8_t cc_index;


	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			cc_index = 3;
		} else {
			cc_index = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			cc_index = 5;
		} else {
			cc_index = compareIndex;
		}
	}


	if (_timerIndex == 0) {

		NRF_TIMER0->CC[cc_index] = value;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->CC[cc_index] = value;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->CC[cc_index] = value;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->CC[cc_index] = value;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->CC[cc_index] = value;

	}

}


void Timer::setCompareWithInterrupt(uint8_t compareIndex, unsigned long value) {
//set value of the Capture/Compare register and enables the corrisponding interrupt event

	unsigned long bitPosition;
	uint8_t cc_index;


	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			bitPosition = 16UL + 3UL;
			cc_index = 3;
		} else {
			bitPosition = 16UL + compareIndex;
			cc_index = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			bitPosition = 16UL + 5UL;
			cc_index = 5;
		} else {
			bitPosition = 16UL + compareIndex;
			cc_index = compareIndex;
		}
	}


	if (_timerIndex == 0) {

		NRF_TIMER0->CC[cc_index] = value;
		NRF_TIMER0->INTENSET |= (1UL << bitPosition);
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->CC[cc_index] = value;
		NRF_TIMER1->INTENSET |= (1UL << bitPosition);

	} else if (_timerIndex == 2) {

		NRF_TIMER2->CC[cc_index] = value;
		NRF_TIMER2->INTENSET |= (1UL << bitPosition);

	} else if (_timerIndex == 3) {

		NRF_TIMER3->CC[cc_index] = value;
		NRF_TIMER3->INTENSET |= (1UL << bitPosition);

	} else if (_timerIndex == 4) {

		NRF_TIMER4->CC[cc_index] = value;
		NRF_TIMER4->INTENSET |= (1UL << bitPosition);

	}

}


void Timer::autoClear(uint8_t compareIndex) {
//enables the timer to auto reset to zero when it reaches the Capture/Compare register value

	unsigned long bitPosition;

	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			bitPosition = 3UL;
		} else {
			bitPosition = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			bitPosition = 5UL;
		} else {
			bitPosition = compareIndex;
		}
	}



	if (_timerIndex == 0) {

		NRF_TIMER0->SHORTS |= (1UL << bitPosition);
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->SHORTS |= (1UL << bitPosition);

	} else if (_timerIndex == 2) {

		NRF_TIMER2->SHORTS |= (1UL << bitPosition);

	} else if (_timerIndex == 3) {

		NRF_TIMER3->SHORTS |= (1UL << bitPosition);

	} else if (_timerIndex == 4) {

		NRF_TIMER4->SHORTS |= (1UL << bitPosition);

	}

}


void Timer::noAutoClear(uint8_t compareIndex) {
//disables the timer to auto reset to zero when it reaches the Capture/Compare register value

	unsigned long bitPosition;

	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			bitPosition = 3UL;
		} else {
			bitPosition = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			bitPosition = 5UL;
		} else {
			bitPosition = compareIndex;
		}
	}



	if (_timerIndex == 0) {

		NRF_TIMER0->SHORTS &= ~(1UL << bitPosition);
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->SHORTS &= ~(1UL << bitPosition);

	} else if (_timerIndex == 2) {

		NRF_TIMER2->SHORTS &= ~(1UL << bitPosition);

	} else if (_timerIndex == 3) {

		NRF_TIMER3->SHORTS &= ~(1UL << bitPosition);

	} else if (_timerIndex == 4) {

		NRF_TIMER4->SHORTS &= ~(1UL << bitPosition);

	}

}


void Timer::setPrescaler(uint8_t timerPrescaler) {
//set the prescaler value (clock = 16MHz / 2^(PRESCALER))

	uint8_t prescalerValue;

	if (timerPrescaler > 8) {
		prescalerValue = 8;
	} else {
		prescalerValue = timerPrescaler;
	}



	if (_timerIndex == 0) {

		NRF_TIMER0->PRESCALER = prescalerValue;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->PRESCALER = prescalerValue;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->PRESCALER = prescalerValue;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->PRESCALER = prescalerValue;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->PRESCALER = prescalerValue;

	}

}


uint32_t Timer::captureTimer(uint8_t compareIndex) {

	uint8_t cc_index;
	uint32_t result;


	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			cc_index = 3;
		} else {
			cc_index = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			cc_index = 5;
		} else {
			cc_index = compareIndex;
		}
	}


	if (_timerIndex == 0) {

		NRF_TIMER0->TASKS_CAPTURE[cc_index] = 1;
		result = NRF_TIMER0->CC[cc_index];
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->TASKS_CAPTURE[cc_index] = 1;
		result = NRF_TIMER1->CC[cc_index];

	} else if (_timerIndex == 2) {

		NRF_TIMER2->TASKS_CAPTURE[cc_index] = 1;
		result = NRF_TIMER2->CC[cc_index];

	} else if (_timerIndex == 3) {

		NRF_TIMER3->TASKS_CAPTURE[cc_index] = 1;
		result = NRF_TIMER3->CC[cc_index];

	} else if (_timerIndex == 4) {

		NRF_TIMER4->TASKS_CAPTURE[cc_index] = 1;
		result = NRF_TIMER4->CC[cc_index];

	}

	return result;


}






void Timer::startTimer() {
//starts the timer


	if (_timerIndex == 0) {

		NRF_TIMER0->TASKS_START = 1;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->TASKS_START = 1;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->TASKS_START = 1;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->TASKS_START = 1;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->TASKS_START = 1;

	}

}




void Timer::stopTimer() {
//stops the timer

	if (_timerIndex == 0) {

		NRF_TIMER0->TASKS_STOP = 1;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->TASKS_STOP = 1;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->TASKS_STOP = 1;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->TASKS_STOP = 1;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->TASKS_STOP = 1;

	}

}


void Timer::clearTimer() {

	if (_timerIndex == 0) {

		NRF_TIMER0->TASKS_CLEAR = 1;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->TASKS_CLEAR = 1;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->TASKS_CLEAR = 1;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->TASKS_CLEAR = 1;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->TASKS_CLEAR = 1;

	}

}


void Timer::count() {
//increments the timer by one when in COUNTER_MODE

	if (_timerIndex == 0) {

		NRF_TIMER0->TASKS_COUNT = 1;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->TASKS_COUNT = 1;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->TASKS_COUNT = 1;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->TASKS_COUNT = 1;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->TASKS_COUNT = 1;

	}

}




void Timer::attachInterrupt(void (*Callback)()) {
//enables the interrupt and links the callback function

	if (_timerIndex == 0) {

		NVIC_SetVector(TIMER0_IRQn, (uint32_t)Callback);
		NVIC_EnableIRQ(TIMER0_IRQn);
	
	} else if (_timerIndex == 1) {

		NVIC_SetVector(TIMER1_IRQn, (uint32_t)Callback);
		NVIC_EnableIRQ(TIMER1_IRQn);

	} else if (_timerIndex == 2) {

		NVIC_SetVector(TIMER2_IRQn, (uint32_t)Callback);
		NVIC_EnableIRQ(TIMER2_IRQn);

	} else if (_timerIndex == 3) {

		NVIC_SetVector(TIMER3_IRQn, (uint32_t)Callback);
		NVIC_EnableIRQ(TIMER3_IRQn);

	} else if (_timerIndex == 4) {

		NVIC_SetVector(TIMER4_IRQn, (uint32_t)Callback);
		NVIC_EnableIRQ(TIMER4_IRQn);

	}	

}


void Timer::clearCompareEvent(uint8_t compareIndex) {
//clears the interrupt flag for the Compare Event

	uint8_t cc_index;

	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			cc_index = 3;
		} else {
			cc_index = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			cc_index = 5;
		} else {
			cc_index = compareIndex;
		}
	}


	if (_timerIndex == 0) {

		NRF_TIMER0->EVENTS_COMPARE[cc_index] = 0;
	
	} else if (_timerIndex == 1) {

		NRF_TIMER1->EVENTS_COMPARE[cc_index] = 0;

	} else if (_timerIndex == 2) {

		NRF_TIMER2->EVENTS_COMPARE[cc_index] = 0;

	} else if (_timerIndex == 3) {

		NRF_TIMER3->EVENTS_COMPARE[cc_index] = 0;

	} else if (_timerIndex == 4) {

		NRF_TIMER4->EVENTS_COMPARE[cc_index] = 0;

	}

}


bool Timer::compareEventFlag(uint8_t compareIndex) {
//returns true if the Compare Event flag is set, false otherwise

	uint8_t cc_index;

	if (_timerIndex < 3) {
		if (compareIndex > 3) {
			cc_index = 3;
		} else {
			cc_index = compareIndex;
		}
	} else {
		if (compareIndex > 5) {
			cc_index = 5;
		} else {
			cc_index = compareIndex;
		}
	}


	if (_timerIndex == 0) {

		return (NRF_TIMER0->EVENTS_COMPARE[cc_index] == 1);
	
	} else if (_timerIndex == 1) {

		return (NRF_TIMER1->EVENTS_COMPARE[cc_index] == 1);

	} else if (_timerIndex == 2) {

		return (NRF_TIMER2->EVENTS_COMPARE[cc_index] == 1);

	} else if (_timerIndex == 3) {

		return (NRF_TIMER3->EVENTS_COMPARE[cc_index] == 1);

	} else if (_timerIndex == 4) {

		return (NRF_TIMER4->EVENTS_COMPARE[cc_index] == 1);

	} else {

		return false;
	}

}

