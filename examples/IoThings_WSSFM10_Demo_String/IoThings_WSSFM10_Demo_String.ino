/***************************************************************************
  This is a library for the Wisol 10 module 

  Designed to work with all kinds of WSSFM10R Breakout Boards

  This module use UART, 2 pins are required to interface.

  Written by Adrien Chapelet for IoThings
 ***************************************************************************/

#include "IO_WSSFM10.h"

IO_WSSFM10 mySigfox(10, 11, true);

uint8_t counter = 0;
uint16_t light = 0, temp = 0;
  
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
  String sfResponse;
 
  char payload_to_send[12];
  memset(payload_to_send, 0, sizeof(payload_to_send));

  light = analogRead(A1);
  temp = analogRead(A2);

  sprintf(to_be_sent, "%02X%04X%04X", counter, light, temp);

  bool statusS = mySigfox.sendString(&p, sizeof(p));
  if (statusS) counter++;

  delay(60000);
  bool statusSR = mySigfox.sendReceiveString(&p, sizeof(p), sfResponse);
  if (statusSR) {
    for (uint8_t i= 0; i<26; ++i) {//RX= 01 02 03 04 05 06 07 08
      Serial.println(sfResponse[i]);
      counter++;
    }
  }
}
