//Serial Sync Header
//	Created By:		Mike Moss
//	Modified On:	08/28/2013

//Begin Define Guards
#ifndef MSL_SERIALSYNC_H
#define MSL_SERIALSYNC_H

//Integer Standard Types Header
#include <inttypes.h>

//Serial Header
#include "serial.hpp"

//String Header
#include <string>

//Number of Serial Variables
#define MSL_SERIALSYNC_VARIABLES 85

//MSL Namespace
namespace msl
{
	//Serial Sync Class Declaration
	class serial_sync
	{
		public:
			//Constructor (Default)
			serial_sync(const std::string& port="",const uint32_t baud=57600);

			//Copy Assignment Operator
			serial_sync& operator=(const serial_sync& copy);

			//Setup Function (Sets up serial port)
			void setup();

			//Close Function (Closes Serial Port)
			void close();

			//Boolean Operator (Tests if Serial Port is Good)
			operator bool() const;

			//Not Operator (For Boolean Operator)
			bool operator!() const;

			//Good Function (Tests if Serial Port is Good)
			bool good() const;

			//Update RX Function (Receives updates over link)
			void update_rx();

			//Update TX Function (Sends updates over link)
			void update_tx();

			//Get Function (Gets a value from a variable)
			int16_t get(const uint8_t index);

			//Set Function (Sets a variable to a value)
			void set(const uint8_t index,const int16_t value);

		private:
			//Calculate CRC Function (XORs all bytes together)
			uint8_t calculate_crc(const uint8_t* buffer,const uint8_t size) const;

			//Member Variables
			uint32_t _baud;
			msl::serial _serial;
			int16_t _data[MSL_SERIALSYNC_VARIABLES];
			uint8_t _flags[MSL_SERIALSYNC_VARIABLES];
			uint8_t _tx_packet[3+1+MSL_SERIALSYNC_VARIABLES*3+1];
			uint8_t _rx_packet[3+1+MSL_SERIALSYNC_VARIABLES*3+1];
			uint32_t _rx_counter;
	};
}

//End Define Guards
#endif