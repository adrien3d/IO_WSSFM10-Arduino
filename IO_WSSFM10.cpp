/***************************************************************************
  This is a library for the Wisol 10 module 

  Designed to work with all kinds of WSSFM10R Breakout Boards

  This module use UART, 2 pins are required to interface.

  Written by Adrien Chapelet for IoThings
 ***************************************************************************/

#include "IO_WSSFM10.h"

//IO_WSSFM10 WSSFM10;

//Message buffer
uint8_t msg[12];

/*IO_WSSFM10::IO_WSSFM10 () : Sigfox(), debug(1) {
	//Sigfox(rxPin, txPin, 3);
}*/

/*IO_WSSFM10::IO_WSSFM10 (uint8_t rxPin, uint8_t txPin) : rx(rxPin), tx(txPin), debug(0) {
	Sigfox(rxPin, txPin, 3);
}*/

IO_WSSFM10::IO_WSSFM10 (uint8_t rxPin=10 , uint8_t txPin=11, bool debugEn=true) : Sigfox(rxPin, txPin), debug(debugEn) {
	//Sigfox(rxPin, txPin);
}

//IO_WSSFM10::IO_WSSFM10 (uint8_t rxPin, uint8_t txPin, bool debugEn) : Sigfox(rxPin, txPin, 3), debug(debugEn) {
//}


void IO_WSSFM10::blink(){
	digitalWrite(LED_BUILTIN, 1);   // turn the LED on (HIGH is the voltage level)
	delay(50);                       // wait for a second
	digitalWrite(LED_BUILTIN, 0);    // turn the LED off by making the voltage LOW
	delay(50);
}

String IO_WSSFM10::getData(){
	String data = "";
	char output;

	while (!Sigfox.available()){
		blink();
	}

	while(Sigfox.available()){
		output = Sigfox.read();
		if ((output!=0x0A)&&(output!=0x0D)){//0x0A Line feed | 0x0D Carriage return
			data += output;
    	}

		delay(10);
	}	

	if(debug){
		Serial.print("Data: ");
		Serial.println(data);
	}

	return data;
}

String IO_WSSFM10::test(){
	Sigfox.print("AT\r");
	String status = getData();

	if(debug){
		Serial.print("Status: ");
		Serial.print(status);
	}

	return status;
}

void IO_WSSFM10::begin(){
	//pinMode(rxPin, INPUT);
	//pinMode(txPin, OUTPUT);
	Sigfox.begin(9600);

	if (debug) {
		Serial.begin(9600);
		delay(100);
		getID();
		delay(100);
		getPAC();
	}
}

//Get Sigfox ID
String IO_WSSFM10::getID(){
	Sigfox.print("AT$I=10\r");
	String id = getData();

	if(debug){
		Serial.print("ID: ");
		Serial.println(id);
	}

	return id;
}


//Get PAC number
String IO_WSSFM10::getPAC(){
	Sigfox.print("AT$I=11\r");
	String pac = getData();

	if(debug){
		Serial.print("PAC: ");
		Serial.println(pac);
	}

	return pac;
}

uint16_t IO_WSSFM10::getTemp(void){
	Sigfox.print("AT$T?\r");

	uint16_t tempVal = getData().toInt();

	if(debug){
		Serial.println("Module temperature: ");
		Serial.println(tempVal);
	}

	return tempVal;
}

String IO_WSSFM10::setPowerMode(uint8_t mode){
//0: Software reset
//1: sleep, send a break to wake up
//2: deep_sleep, toggle GPIO0 or Reset_N to wake up

	Sigfox.print("AT$P=");
	Sigfox.print(mode);
	Sigfox.print("\r");
	String res = getData();

	if(debug){
		Serial.println("Power Mode response: ");
		Serial.println(res);
	}

	return res;
}

String IO_WSSFM10::setOutputPower(uint8_t power){
	Sigfox.print("ATS302=");
	Sigfox.print(power);
	Sigfox.print("\r");
	String res = getData();

	if(debug){
		Serial.println("Output Power response: ");
		Serial.println(res);
	}

	return res;
}

void IO_WSSFM10::wakeUp(void){
	Sigfox.print("\r");
}

//Send Sigfox Message
bool IO_WSSFM10::send(const void* data, uint8_t size){//const void* data
	String status = "";
	char output;

	uint8_t* bytes = (uint8_t*)data;

	Sigfox.print("AT$SF=");
	for(uint8_t i= 0; i<size; ++i){
		Sigfox.print(bytes[i]);
		if(debug){
			Serial.print("Byte:");
			Serial.println(bytes[i], HEX);
		}
	}

	Sigfox.print("\r");

	while (!Sigfox.available()){
		blink();
	}

	String res = getData();

	if(res.indexOf("OK") >= 0) {
		Serial.println("Message successfully sent");
		return true;
	}
	
	if(debug){
		Serial.print("Status: ");
		Serial.println(res);
	}
	return false;
}

bool IO_WSSFM10::sendString(String str) {
	if(debug){
		Serial.print("Message:");
		Serial.println(str);
	}

	Sigfox.print("AT$SF=");
	Sigfox.print(str);
	Sigfox.print("\r");

  	while (!Sigfox.available()){
		blink();
	}

	String res = getData();

	if(res.indexOf("OK") >= 0) {
		Serial.println("Message successfully sent");
		return true;
	}
	
	if(debug){
		Serial.print("Status: ");
		Serial.println(res);
	}
	return false;
}



bool IO_WSSFM10::sendReceive(const void* data, uint8_t size, String response){
	String status = "";
	char output;

	uint8_t* bytes = (uint8_t*)data;
	Sigfox.print("AT$SF=");
	for(uint8_t i= 0; i<size; ++i){
		Sigfox.print(bytes[i]);
		if(debug){
			Serial.print("Byte:");
			Serial.println(bytes[i], HEX);
		}
	}

	Sigfox.print(",1\r");

	while (!Sigfox.available()){
		blink();
	}

	String res = getData();

	if(res.indexOf("OK") >= 0) {
		Serial.println("Message successfully sent");
		response = getData();
		return true;
	}
	
	if(debug){
		Serial.print("Status: ");
		Serial.println(res);
	}
	return false;
}


bool IO_WSSFM10::sendReceiveString(String str, String response) {
	String status = "";
	char output;


	Sigfox.print("AT$SF=");
	Sigfox.print(str);
	Sigfox.print(",1\r");

	while (!Sigfox.available()){
		blink();
	}

	String res = getData();

	if(res.indexOf("OK") >= 0) {
		Serial.println("Message successfully sent");
		response = getData();
		return true;
	}
	
	if(debug){
		Serial.print("Status: ");
		Serial.println(res);
	}
	return false;
}
