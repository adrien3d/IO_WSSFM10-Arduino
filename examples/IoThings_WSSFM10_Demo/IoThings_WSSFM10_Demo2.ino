#include "IO_WSSFM10.h"
IO_WSSFM10 mySigfox(10, 11, true);

boolean debug =  true;

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
  uint16_t light=0, temp=0;
  char to_be_sent[12];
  memset(to_be_sent, 0, sizeof(to_be_sent));

  counter = counter;
  light = analogRead(A1);
  temp = analogRead(A2);
  
  sprintf(to_be_sent,  "%04X%04X",light,temp);//Saving values in HEX format. To use other format specifiers have a look at http://www.cplusplus.com/reference/cstdio/printf/
  if (debug) {
    Serial.println(">>> "+String(to_be_sent)+" <<<");
    Serial.println("Message length: "+String(strlen(to_be_sent)));
    Serial.println("light: "+String(light));
    Serial.println("temp: "+String(temp)+"\n");  
  }
    
  sendMessageTwo(to_be_sent);
  if (debug) {
    delay(30*1000);
  } 
}
