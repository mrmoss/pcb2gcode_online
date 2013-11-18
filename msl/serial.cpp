//Serial Source
//	Created By:		Mike Moss
//	Modified On:	05/20/2013

//Definitions for "serial.hpp"
#include "serial.hpp"

//Time Utility Header
#include "time_util.hpp"

//Windows Dependencies
#if(defined(_WIN32)&&!defined(__CYGWIN__))

	//Windows Read Function (Unix-like replacement)
	SSIZE_T read(SERIAL fd,void* buf,size_t count)
	{
		//Return Value
		DWORD bytes_read=-1;

		//Read Bytes (this blocks)
		if(ReadFile(fd,buf,count,&bytes_read,0))
			return bytes_read;

		//On Error
		return -1;
	}

	//Windows Write Function (Unix-like replacement)
	SSIZE_T write(SERIAL fd,void* buf,size_t count)
	{
		//Return Value
		DWORD bytes_sent=-1;

		//Write Bytes (this blocks)
		if(WriteFile(fd,buf,count,&bytes_sent,0))
			return bytes_sent;

		//On Error
		return -1;
	}

	//Windows Select Function (Unix-like replacement)
	int select(SERIAL nfds)
	{
		//Checking Variables
		COMSTAT port_stats;
		DWORD error_flags=0;

		//Get Bytes in RX Buffer
		if(ClearCommError(nfds,&error_flags,&port_stats))
			return port_stats.cbInQue;

		//On Error
		return -1;
	}

//Unix Dependencies
#else
	#include <unistd.h>
	#include <fcntl.h>
	#include <termios.h>
	#include <sys/ioctl.h>
#endif

//Constructor(Default)
msl::serial::serial(const std::string& name,const unsigned int baud):_port(SERIAL_ERROR),_name(name),_baud(baud)
{}

//Copy Constructor
msl::serial::serial(const msl::serial& copy):_port(copy._port),_name(copy._name),_baud(copy._baud)
{}

//Copy Assignment Operator
msl::serial& msl::serial::operator=(const msl::serial& copy)
{
	if(this!=&copy)
	{
		_port=copy._port;
		_name=copy._name;
		_baud=copy._baud;
	}

	return *this;
}

//Boolean Operator (Tests if Port is Good)
msl::serial::operator bool() const
{
	return good();
}

//Not Operator (For Boolean Operator)
bool msl::serial::operator!() const
{
	return !static_cast<bool>(*this);
}

//Good Function (Tests if Port is Good)
bool msl::serial::good() const
{
	//Fix for Linux Errors
	#if(!defined(_WIN32)||defined(__CYGWIN__))
		if(_port>1024*1024||_port<0)
			return false;
	#endif

	return (_port!=SERIAL_ERROR&&available()>=0);
}

//Connect Function (Connects to a Port)
void msl::serial::connect()
{
	_port=serial_connect(_name,_baud);
}

//Close Function (Disconnects from a Port)
void msl::serial::close()
{
	serial_close(_port);
}

//Available Function (Checks if there are Bytes to be Read, -1 on Error)
int msl::serial::available() const
{
	return serial_available(_port,0);
}

//Read Function (Returns -1 on Error Else Returns Number of Bytes Read)
int msl::serial::read(void* buffer,const unsigned int size,const unsigned int time_out)
{
	return serial_read(_port,buffer,size,time_out);
}

//Write Function (Returns -1 on Error Else Returns Number of Bytes Sent)
int msl::serial::write(void* buffer,const unsigned int size,const unsigned int time_out)
{
	return serial_write(_port,buffer,size,time_out);
}

//System Port Accessor
SERIAL msl::serial::system_port() const
{
	return _port;
}

//Serial Connection Function (Connects to a Port)
SERIAL msl::serial_connect(const std::string& name,const unsigned int baud)
{
	//Windows
	#if(defined(_WIN32)&&!defined(__CYGWIN__))

		//Add Full Path to Serial Port
		std::string full_name="\\\\.\\"+name;

		//Open Serial Port
		SERIAL port=CreateFile(full_name.c_str(),GENERIC_READ|GENERIC_WRITE,0,0,CREATE_ALWAYS,0,NULL);

		//On Open Error
		if(port==SERIAL_ERROR)
			return serial_close(port);

		//Check for Valid Baud Rate
		if(baud!=300&&baud!=1200&&baud!=2400&&baud!=4800&&baud!=9600&&baud!=19200&&baud!=38400&&baud!=57600&&baud!=115200)
			return serial_close(port);

		//Serial Port Options Variable
		DCB options;

		//Get Current Serial Port Options
		if(!GetCommState(port,&options))
			return serial_close(port);

		//Setup Serial Port Settings
		options.BaudRate=baud;
		options.fParity=FALSE;
		options.Parity=NOPARITY;
		options.StopBits=ONESTOPBIT;
		options.ByteSize=8;
		options.fOutxCtsFlow=FALSE;
		options.fOutxDsrFlow=FALSE;
		options.fDtrControl=DTR_CONTROL_DISABLE;
		options.fRtsControl=RTS_CONTROL_DISABLE;

		//Apply Serial Port Settings
		if(!SetCommState(port,&options))
			return serial_close(port);

		//Clear DTR and RTS Signals
		if(!EscapeCommFunction(port,CLRDTR|CLRRTS))
			return serial_close(port);

	//Unix
	#else
		//Open Serial Port
		SERIAL port=open(name.c_str(),O_RDWR|O_NOCTTY|O_SYNC);

		//On Open Error
		if(port==SERIAL_ERROR)
			return serial_close(port);

		//Fix for Linux Errors
		if(port>1024*1024||port<0)
			return -1;

		//Check for Valid Baud Rate
		speed_t baud_rate;

		if(baud==300)
			baud_rate=B300;
		else if(baud==1200)
			baud_rate=B1200;
		else if(baud==2400)
			baud_rate=B2400;
		else if(baud==4800)
			baud_rate=B4800;
		else if(baud==9600)
			baud_rate=B9600;
		else if(baud==19200)
			baud_rate=B19200;
		else if(baud==38400)
			baud_rate=B38400;
		else if(baud==57600)
			baud_rate=B57600;
		else if(baud==115200)
			baud_rate=B115200;
		else
			return serial_close(port);

		//Serial Port Options Variable
		termios options;

		//Get Current Serial Port Options
		if(tcgetattr(port,&options)==-1)
			return serial_close(port);

		//Setup Serial Port Baud Rate
		if(cfsetispeed(&options,baud_rate)==-1||cfsetospeed(&options,baud_rate)==-1)
			return serial_close(port);

		//Setup Other Serial Port Settings
		options.c_cflag|=(CS8|CLOCAL|CREAD|HUPCL);
		options.c_iflag|=(IGNBRK|IGNPAR);
		options.c_iflag&=~(IXON|IXOFF|IXANY);
		options.c_lflag=0;
		options.c_oflag=0;
		options.c_cc[VMIN]=0;
		options.c_cc[VTIME]=1;
		options.c_cflag&=~(PARENB|PARODD);
		options.c_cflag&=~CSTOPB;
		options.c_cflag&=~CRTSCTS;

		//Apply Serial Port Settings
		if(tcsetattr(port,TCSANOW,&options)==-1)
			return serial_close(port);

		//Flush Serial Port
		if(tcflush(port,TCIFLUSH)==-1||tcdrain(port)==-1)
			return serial_close(port);
	#endif

	//Return Serial Port
	return port;
}

//Serial Close Function (Disconnects from a Port)
SERIAL msl::serial_close(const SERIAL port)
{
	//Windows
	#if(defined(_WIN32)&&!defined(__CYGWIN__))
		if(port!=SERIAL_ERROR)
			CloseHandle(port);

	//Unix
	#else
		if(port!=SERIAL_ERROR)
			close(port);
	#endif

	return SERIAL_ERROR;
}

//Serial Available Function (Checks if there are Bytes to be Read, -1 on Error)
int msl::serial_available(const SERIAL port,const long time_out)
{
	//Check for Errored Port
	if(port==SERIAL_ERROR)
		return -1;

	//Fix for Linux Errors
	#if(!defined(_WIN32)||defined(__CYGWIN__))
		if(port>1024*1024||port<0)
			return -1;
	#endif

	//Return Variable
	int return_value=-1;

	//Reading Variables
	long time_start=msl::millis();

	//Unix
	#if(!defined(_WIN32)||defined(__CYGWIN__))
		timeval temp={0,0};
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(port,&rfds);
	#endif

	//While Port is Good
	do
	{
		//Windows
		#if(defined(_WIN32)&&!defined(__CYGWIN__))
			return_value=select(port);

		//Unix
		#else
			return_value=select(1+port,&rfds,NULL,NULL,&temp);
		#endif

		//If Bytes Break
		if(return_value>0)
			break;
	}
	while(msl::millis()-time_start<time_out);

	//Return Bytes Waiting
	return return_value;
}

//Serial Read Function (Returns Number of Bytes Read, -1 on Error)
int msl::serial_read(const SERIAL port,void* buffer,const unsigned int size,const long time_out)
{
	//Check for Bad Port
	if(port==SERIAL_ERROR)
		return -1;

	//Fix for Linux Errors
	#if(!defined(_WIN32)||defined(__CYGWIN__))
		if(port>1024*1024||port<0)
			return -1;
	#endif

	//Reading Variables
	unsigned int bytes_unread=size;
	long time_start=msl::millis();

	//While Port is Good and There are Bytes to Read
	do
	{
		//Get Bytes in Read Buffer
		unsigned int bytes_read=read(port,reinterpret_cast<char*>(buffer)+(size-bytes_unread),bytes_unread);

		//If Bytes Were Read
		if(bytes_read>0)
		{
			//Subtract Read Bytes
			bytes_unread-=bytes_read;

			//If Done Break
			if(bytes_unread==0)
				return size;
		}
	}
	while(msl::millis()-time_start<time_out);

	//Return Bytes Read
	return (size-bytes_unread);
}

//Serial Write Function (Returns Number of Bytes Sent, -1 on Error)
int msl::serial_write(const SERIAL port,void* buffer,const unsigned int size,const long time_out)
{
	//Check for Bad Port
	if(port==SERIAL_ERROR)
		return -1;

	//Fix for Linux Errors
	#if(!defined(_WIN32)||defined(__CYGWIN__))
		if(port>1024*1024||port<0)
			return -1;
	#endif

	//Writing Variables
	unsigned int bytes_unsent=size;
	long time_start=msl::millis();

	//While Port is Good and There are Bytes to Send
	do
	{
		//Get Bytes in Send Buffer
		unsigned int bytes_sent=write(port,reinterpret_cast<char*>(buffer)+(size-bytes_unsent),bytes_unsent);

		//If Bytes Were Sent
		if(bytes_sent>0)
		{
			//Subtract Sent Bytes
			bytes_unsent-=bytes_sent;

			//If Done Break
			if(bytes_unsent==0)
				return size;
		}
	}
	while(msl::millis()-time_start<time_out);

	//Return Bytes Sent
	return (size-bytes_unsent);
}