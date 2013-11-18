//Socket Utility Header
//	Created By:		Mike Moss
//	Modified On:	10/16/2013

//Required Libraries:
//	Ws2_32 (windows only)

//Begin Define Guards
#ifndef MSL_SOCKET_UTIL_H
#define MSL_SOCKET_UTIL_H

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	//IP Lookup Function (Returns the IP address of a hostname)
	std::string lookup_ip(const std::string& hostname);

	//HTTP Create Header Function (Creates a header for sending HTTP messages)
	std::string http_create_header(const unsigned int message_size,const std::string& mime_type="text/html",
		const bool compressed=false,const bool keepalive=true);

	//HTTP to ASCII Function (Converts a string with http symbols to ascii symbols)
	std::string http_to_ascii(std::string symbols);

	//HTTP Pack String Function (Packages a string for http travel)
	std::string http_pack_string(const std::string& message,const std::string& mime_type="text/html",
		const bool compressed=false,const bool keepalive=true);
}

//End Define Guards
#endif

//Example
/*
#include <iostream>
#include <string>
#include "socket_util.hpp"
using namespace msl;

int main()
{
	std::cout<<"Get IP Address of google.com:"<<std::endl;
	std::cout<<lookup_ip("google.com")<<std::endl<<std::endl;

	std::cout<<"Create http header for a png with 3000 bytes of data:"<<std::endl;
	std::cout<<"<------start------>"<<std::endl;
	std::cout<<http_create_header(3000,"image/png");
	std::cout<<"<-------end------->"<<std::endl<<std::endl;

	std::cout<<"Convert http symbols to ascii:"<<std::endl;
	std::string http_symbols="%54%61%20%44%61%21";
	std::cout<<http_to_ascii(http_symbols)<<std::endl<<std::endl;

	std::cout<<"Prepare http header for a string and append string to http message:"<<std::endl;
	std::string html="<html>\n\t<head>\n\t\t<title>\n\t\t\tHello!\n\t\t</title>\n\t</head>\n\t<body>\n\t\t<h1>\n\t\t\tHello!\n\t</h1>\n\t</body>\n</html>";
	std::cout<<"<------start------>"<<std::endl;
	std::cout<<http_pack_string(html,"text/html")<<std::endl;
	std::cout<<"<-------end------->"<<std::endl<<std::endl;

	return 0;
}
*/