//Serial Header
//	Created By:		Mike Moss
//	Modified On:	05/20/2013

//Begin Define Guards
#ifndef MSL_SERIAL_H
#define MSL_SERIAL_H

//String Header
#include <string>

//String Stream Header
#include <sstream>

//Windows Dependencies
#if(defined(_WIN32)&&!defined(__CYGWIN__))
	#include <conio.h>
	#include <stdio.h>
	#include <windows.h>
	#include <commctrl.h>
	#define SERIAL HANDLE
	#define SERIAL_ERROR INVALID_HANDLE_VALUE

//Unix Dependencies
#else
	#define SERIAL int
	#define SERIAL_ERROR (-1)
#endif

//MSL Namespace
namespace msl
{
	//Serial Class Declaration
	class serial
	{
		public:
			//Constructor (Default)
			serial(const std::string& name="",const unsigned int baud=57600);

			//Copy Constructor
			serial(const serial& copy);

			//Copy Assignment Operator
			serial& operator=(const serial& copy);

			//Boolean Operator (Tests if Port is Good)
			operator bool() const;

			//Not Operator (For Boolean Operator)
			bool operator!() const;

			//Good Function (Tests if Port is Good)
			bool good() const;

			//Connect Function (Connects to a Port)
			void connect();

			//Close Function (Disconnects from a Port)
			void close();

			//Available Function (Checks if there are Bytes to be Read)
			int available() const;

			//Read Function (Returns Number of Bytes Read, -1 on Error)
			int read(void* buffer,const unsigned int size,const unsigned int time_out=0);

			//Write Function (Returns Number of Bytes Sent, -1 on Error)
			int write(void* buffer,const unsigned int size,const unsigned int time_out=0);

			//Connection Timeout Mutator
			void set_timeout(const long time_out);

			//Connection Timeout Accessor
			long timeout() const;

			//System Port Accessor
			SERIAL system_port() const;

			//Stream Out Operator
			template <typename T> friend msl::serial& operator<<(msl::serial& lhs,const T& rhs);

		private:
			//Member Variables
			SERIAL _port;
			std::string _name;
			unsigned int _baud;
			long _time_out;
	};

	//Serial Connection Function (Connects to a Port)
	SERIAL serial_connect(const std::string& name,const unsigned int baud);

	//Serial Close Function (Disconnects from a Port)
	SERIAL serial_close(const SERIAL port);

	//Serial Available Function (Checks if there are Bytes to be Read, -1 on Error)
	int serial_available(const SERIAL port,const long time_out=0);

	//Serial Read Function (Returns Number of Bytes Read, -1 on Error)
	int serial_read(const SERIAL port,void* buffer,const unsigned int size,const long time_out=200);

	//Serial Write Function (Returns Number of Bytes Sent, -1 on Error)
	int serial_write(const SERIAL port,void* buffer,const unsigned int size,const long time_out=200);
}

//End Define Guards
#endif

//Example
/*
//Serial Testing Source
//	Created By:		Mike Moss
//	Modified On:	05/20/2013

//IO Stream Header
#include <iostream>

//Serial Header
#include "msl/serial.hpp"

//String Header
#include <string>

//Main
int main()
{
	//Connect to Serial Port
	msl::serial port("com5",57600);
	port.connect();

	//Check Serial Port
	if(port.good())
		std::cout<<"=)"<<std::endl;
	else
		std::cout<<"=("<<std::endl;

	//Write Test A
	port<<"Hello ";

	//Write Test B
	std::string test1="World!\n";
	port.write((void*)test1.c_str(),test1.size());

	//Read Test
	char buffer;

	while(port.available()>0&&port.read(&buffer,1)==1)
		std::cout<<buffer;

	//All Done
	return 0;
}
*/