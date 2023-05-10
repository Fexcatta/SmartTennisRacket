#ifndef _TIMER_H_
#define _TIMER_H_


namespace TMR {

  Timer tmr3(3, TIMER_MODE, bitMode_32); //Timer for data acquisition
  Timer tmr2(2, TIMER_MODE, bitMode_16); //Watchdog timer

  //TIMER ISR
  void TIMER3_Callback() {

    if (TMR::tmr3.compareEventFlag(0)) {
      TMR::tmr3.clearCompareEvent(0);
      updateSensors = true;
    }

  } 

  void TIMER2_Callback() {

    if (TMR::tmr2.compareEventFlag(0)) {
      TMR::tmr2.clearCompareEvent(0);
      checkSensors = true;
    }

  }

  //TIMER initialization
  void init() {

    /*<milliseconds> * TIMER_CLOCK = <compareValue> */
    tmr3.begin(TIMER_PRESCALER); // TIMER_CLOCK = (16MHz / 2 ^ (TIMER_PRESCALER))
    tmr3.setCompareWithInterrupt(0, TIMER_COMPARE_VALUE);
    tmr3.autoClear(0);
    tmr3.attachInterrupt(TIMER3_Callback);

    tmr2.begin(8); // TIMER_CLOCK = (16MHz / 2 ^ (TIMER_PRESCALER))
    tmr2.setCompareWithInterrupt(0, 62500);
    tmr2.autoClear(0);
    tmr2.attachInterrupt(TIMER2_Callback);


  }

}










#endif