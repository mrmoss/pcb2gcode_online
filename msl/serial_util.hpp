//Serial Utilities Header
//	Created By:		Mike Moss
//	Modified On:	06/23/2013

//Begin Define Guards
#ifndef MSL_SERIAL_UTIL_H
#define MSL_SERIAL_UTIL_H

//String Header
#include <string>

//Vector Header
#include <vector>

//MSL Namespace
namespace msl
{
	//List Serial Ports Function (Returns a list of valid serial ports currently connected to the system)
	std::vector<std::string> list_serial_ports();
}

//End Define Guards
#endif