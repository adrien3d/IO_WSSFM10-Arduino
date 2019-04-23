/***************************************************************************
  This is a library for the Wisol 10 module 

  Designed to work with all kinds of WSSFM10R Breakout Boards

  This module use UART, 2 pins are required to interface.

  Written by Adrien Chapelet for IoThings
 ***************************************************************************/

#ifndef __IO_WSSFM10_H__
#define __IO_WSSFM10_H__

//#include <inttypes.h>
//#include "String.h"
#include "Arduino.h"
#include <SoftwareSerial.h>
//#include "SoftSerialSTM32.h"

#define LED_BUILTIN 13

class IO_WSSFM10
{
	protected:
	// Instance-specific properties

	public:
		//IO_WSSFM10(void);
		//IO_WSSFM10(uint8_t rxPin, uint8_t txPin);
		IO_WSSFM10(uint8_t rxPin, uint8_t txPin, bool debugEn);

		String test(void);

		void blink(void);
		void begin(void);
		String getData(void);

		String getID(void);
		String getPAC(void);

		//uint16_t getTemp(void);
		uint16_t getTemp(void);
		String setPowerMode(uint8_t mode);
		String setOutputPower(uint8_t power);
		void wakeUp(void);

		bool send(const void* data, uint8_t size);
		bool sendReceive(const void* data, uint8_t size, String response);

		bool sendString(String str);
		bool sendReceiveString(String str, String response);
		/*int8_t getRev();
		bool setPower(uint8_t power);*/

		String _nextReturn();

		typedef enum {
			OK,
			KO,
			SENT
		} Status;

		bool debug;

	private:
		uint8_t rx, tx;
		//SoftSerialSTM32 Sigfox;
		SoftwareSerial Sigfox;
		//#if defined(__STM32F1__)
		//  HardwareSerial *port;
		//#endif
};

extern IO_WSSFM10 WSSFM10;

#endif
