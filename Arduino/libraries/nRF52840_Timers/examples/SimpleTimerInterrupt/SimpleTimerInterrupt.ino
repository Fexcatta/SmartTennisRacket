/*
  nRF52840_Timers - Simple Timer Interrupt

 Demonstrates the use of a timer interrupt. The nRF5280_Timers
 library works with all Arduino NANO boards that use the
 nRF5280 microcontroller. You can check the microcontroller
 used by your board on the official Arduino website.

 This sketch demonstrates the use of the attachInterrupt()
 function to enable an hardware interrupt to fire every second.

 In order to test the functionality, the program increases
 an integer value every time the timer interrupt fires and
 it sends the value to the pc through the Serial interface.



 Library originally added 10 Apr 2023
 by Federico Cattaneo
 example added 10 Apr 2023
 by Federico Cattaneo
 

 This example code is in the public domain.

 https://www.arduino.com

*/

//include the library code:
#include <nRF52840_Timers.h>




int number = 0; //number to be updated every time the timer interrupt fires
bool update_number = true;  //boolean flag to indicate to send a new value over Serial


//initialize the library by setting which timer (of the 5 available), mode and bit length to use
Timer tmr1(1, TIMER_MODE, bitMode_16);  //use TIMER1, timer mode and 16bit wide register (can count to 65.535)



void setup() {
  // put your setup code here, to run once:

  //initialize the serial interface (needed to read the number variable using a pc)
  Serial.begin(115200); //use a baud rate of 115200

  //Setup of timers parameters
  tmr1.begin(8);  //initialize the timer with a PRESCALER of 8 (16MHz clock / 2^(PRESCALER))

  tmr1.setCompareWithInterrupt(0, 62500); //set the compare register 0 to 62500 and the interrupt
  //when the timer reaches that amount

  tmr1.autoClear(0);  //the timer counter is set to zero when it reaches the compare register 0
  //value

  tmr1.attachInterrupt(TIMER1_Callback);  //set the interrupt fuction

  tmr1.startTimer();  //start the timer

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(update_number) { //when the number has been updated
    Serial.println(number, DEC); //send the number value through the serial interface
    update_number = false; //the number value is no longer updated (wait for a new value)
  }
  
  

}

//TIMER interrupt service routine - this function is run when an interrupt event is fired by the timer
void TIMER1_Callback(void) {
  
  if (tmr1.compareEventFlag(0)){  //check that the interrupt event is caused by the compare register 0

    tmr1.clearCompareEvent(0);  //clear the event flag associated to the compare register 0

    number += 1;  //increase the number value by 1
    update_number = true; //signal that the number value has been updated

  }
  
}
