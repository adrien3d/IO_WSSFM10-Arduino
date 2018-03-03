/***************************************************************************
  This is a library for the Wisol 10 module 

  Designed to work with all kinds of WSSFM10R Breakout Boards

  This module use UART, 2 pins are required to interface.

  Written by Adrien Chapelet for IoThings
 ***************************************************************************/

#include "IO_WSSFM10.h"

IO_WSSFM10 mySigfox(10, 11, true);

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    mySigfox.begin();
    delay(50);
    mySigfox.test();
    mySigfox.getID();
    mySigfox.getPAC();
    mySigfox.getTemp();

    //mySigfox.setPowerMode(1);
}
  
// the loop function runs over and over again forever
void loop() {
  mySigfox.send(&"aa",2);

  delay(60000);
}