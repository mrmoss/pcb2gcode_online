//Serial Sync Header
//	Created By:		Mike Moss
//	Modified On:	08/28/2013

//Definitions for "serial_sync.hpp"
#include "serial_sync.hpp"

//Constructor (Default)
msl::serial_sync::serial_sync(const std::string& port,const uint32_t baud):
	_baud(baud),_serial(port,_baud),_rx_counter(0)
{
	//Zero Out Data Array
	for(uint8_t ii=0;ii<MSL_SERIALSYNC_VARIABLES;++ii)
	{
		_data[ii]=0x0000;
		_flags[ii]=0x00;
	}

	//Zero Out Buffers
	for(uint8_t ii=0;ii<3+1+MSL_SERIALSYNC_VARIABLES+1;++ii)
	{
		_tx_packet[ii]=0x00;
		_rx_packet[ii]=0x00;
	}
}

//Copy Assignment Operator
msl::serial_sync& msl::serial_sync::operator=(const msl::serial_sync& copy)
{
	//Check for Self Assignment
	if(this!=&copy)
	{
		_baud=copy._baud;
		_serial=copy._serial;

		for(unsigned int ii=0;ii<MSL_SERIALSYNC_VARIABLES;++ii)
		{
			_data[ii]=copy._data[ii];
			_flags[ii]=copy._flags[ii];
		}

		for(unsigned int ii=0;ii<MSL_SERIALSYNC_VARIABLES*3+1;++ii)
		{
			_tx_packet[ii]=copy._tx_packet[ii];
			_rx_packet[ii]=copy._rx_packet[ii];
		}

		_rx_counter=copy._rx_counter;
	}

	return *this;
}

//Setup Function (Sets up serial port)
void msl::serial_sync::setup()
{
	//Connect Serial Port
	_serial.connect();

	//Check Serial Port
	if(_serial.good())
	{
		//Packet Header
		_tx_packet[0]='m';
		_tx_packet[1]='s';
		_tx_packet[2]='l';

		//Packet Size
		_tx_packet[3]=0;

		//Packet CRC
		_tx_packet[3+1]=calculate_crc(_tx_packet,3+1);

		//Send Packet
		_serial.write(_tx_packet,3+1+1);
	}
}

//Boolean Operator (Tests if Serial Port is Good)
msl::serial_sync::operator bool() const
{
	return good();
}

//Not Operator (For Boolean Operator)
bool msl::serial_sync::operator!() const
{
	return !good();
}

//Good Function (Tests if Serial Port is Good)
bool msl::serial_sync::good() const
{
	return _serial.good();
}

//Close Function (Closes Serial Port)
void msl::serial_sync::close()
{
	_serial.close();
}

//Update RX Function (Receives updates over link)
void msl::serial_sync::update_rx()
{
	//Temporary Byte Variable
	uint8_t temp;

	//Read Bytes
	while(_serial.available()>0&&_serial.read(&temp,1)==1)
	{
		//Put Byte in Buffer
		_rx_packet[_rx_counter]=temp;

		//Parse Header and Size
		if((_rx_counter==0&&temp=='m')||(_rx_counter==1&&temp=='s')||(_rx_counter==2&&temp=='l')||_rx_counter==3)
		{
			//Increment Counter
			++_rx_counter;
		}

		//Parse Data
		else if(_rx_counter>3&&_rx_counter<3+1+(uint32_t)_rx_packet[3])
		{
			//Increment Counter
			++_rx_counter;
		}

		//Parse CRC
		else if(_rx_counter==3+1+(uint32_t)_rx_packet[3])
		{
			//Check CRC
			if(temp==calculate_crc(_rx_packet,_rx_counter))
			{

				//Packet Size 0
				if(_rx_packet[3]==0x00)
				{
					//Set All Variables t Update
					for(uint8_t ii=0;ii<MSL_SERIALSYNC_VARIABLES;++ii)
						_flags[ii]=0x01;

					//Send Global Update
					update_tx();
				}

				//Packet Size > 0
				else
				{
					//Save Data
					for(uint8_t ii=0;ii<_rx_packet[3];ii+=3)
						if(_rx_packet[3+1+ii]<MSL_SERIALSYNC_VARIABLES)
							_data[_rx_packet[3+1+ii]]=*(int16_t*)(_rx_packet+3+1+ii+1);
				}
			}

			//Reset Counter
			_rx_counter=0;
		}

		//Errors
		else
		{
			//Reset Counter
			_rx_counter=0;
		}
	}
}

//Update TX Function (Sends updates over link)
void msl::serial_sync::update_tx()
{
	//Packet Header
	_tx_packet[0]='m';
	_tx_packet[1]='s';
	_tx_packet[2]='l';

	//Packet Size
	_tx_packet[3]=0;

	//Go Through Values
	for(uint8_t ii=0;ii<MSL_SERIALSYNC_VARIABLES;++ii)
	{
		//If Set
		if(_flags[ii])
		{
			//Add Value Index
			_tx_packet[3+1+_tx_packet[3]+0]=ii;

			//Add Value
			_tx_packet[3+1+_tx_packet[3]+1]=*(((uint8_t*)_data)+2*ii+0);
			_tx_packet[3+1+_tx_packet[3]+2]=*(((uint8_t*)_data)+2*ii+1);

			//Increase Packet Size
			_tx_packet[3]+=3;
		}
	}

	//Check Size
	if(_tx_packet[3]>0)
	{
		//Packet CRC
		_tx_packet[3+1+_tx_packet[3]]=calculate_crc(_tx_packet,3+1+_tx_packet[3]);

		//Send Packet
		if(_serial.good())
			_serial.write(_tx_packet,3+1+_tx_packet[3]+1);
	}

	//Reset Set Flags
	for(uint8_t ii=0;ii<MSL_SERIALSYNC_VARIABLES;++ii)
		_flags[ii]=0x00;
}

//Get Function (Gets a value from a variable)
int16_t msl::serial_sync::get(const uint8_t index)
{
	//Get Value
	return _data[index];
}

//Set Function (Sets a variable to a value)
void msl::serial_sync::set(const uint8_t index,const int16_t value)
{
	//Set Value and Flag
	if(index<MSL_SERIALSYNC_VARIABLES)
	{
		_data[index]=value;
		_flags[index]=0x01;
	}
}

//Calculate CRC Function (XORs all bytes together)
uint8_t msl::serial_sync::calculate_crc(const uint8_t* buffer,const uint8_t size) const
{
	//CRC Return Value
	uint8_t crc=0x00;

	//XOR All the Bytes Together
	for(uint8_t ii=0;ii<size;++ii)
		crc^=((uint8_t*)buffer)[ii];

	//Return CRC Value
	return crc;
}