//Time Utilities Header
//	Created By:		Mike Moss
//	Modified On:	05/19/2013

//Begin Define Guards
#ifndef MSL_TIME_UTIL_H
#define MSL_TIME_UTIL_H

//Windows Dependices
#if(defined(_WIN32)&&!defined(__CYGWIN__))
	//Windows Header
	#include <windows.h>

	//Micro Second Sleep Function (Used for relinquishing time slices)
	int usleep(long useconds);
#else
	//Header for usleep
	#include <unistd.h>
#endif

//MSL Namespace
namespace msl
{
	//Get System Time in Milliseconds
	long millis();
}

//End Define Guards
#endif