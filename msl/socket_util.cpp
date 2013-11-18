//Socket Utility Source
//	Created By:		Mike Moss
//	Modified On:	10/16/2013

//Required Libraries:
//	Ws2_32 (windows only)

//Definitions for "socket_util.hpp"
#include "socket_util.hpp"

//Socket Header
#include "socket.hpp"

//String Stream Header
#include <sstream>

//Windows Dependencies
#if(defined(_WIN32)&&!defined(__CYGWIN__))
	#define _WIN32_WINNT 0x0501
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib,"Ws2_32.lib")

//Unix Dependencies
#else
	#include <netdb.h>
	#include <arpa/inet.h>
#endif

//IP Lookup Function (Returns the IP address of a hostname)
std::string msl::lookup_ip(const std::string& hostname)
{
	//Initialize Sockets
	socket_init();

	//Create Return IP String (Default is an invalid address)
	std::string return_address="169.254.57.249";

	//Create IP Lookup Linked List
	addrinfo* lookup;

	//Attempt IP Lookup
	if(getaddrinfo(hostname.c_str(),NULL,NULL,&lookup)==0)
	{
		//Extract IP Address
		in_addr ip_address;
		ip_address.s_addr=((sockaddr_in*)(lookup->ai_addr))->sin_addr.s_addr;

		//Set Return Value
		return_address=std::string(inet_ntoa(ip_address));

		//Free IP Lookup Linked List
		freeaddrinfo(lookup);
	}

	//Return IP
	return return_address;
}

//HTTP Create Header Function (Creates a header for sending HTTP messages)
std::string msl::http_create_header(const unsigned int message_size,const std::string& mime_type,
	const bool compressed,const bool keepalive)
{
	//Create HTML Header
	std::ostringstream header;
	header<<"HTTP/1.1 200 OK\n";
	header<<"Server: Super Lightning Automatic Systematic Wisdomatic Server 3000 v6.54.33.2.1a-177b-c Stable Beta\n";
	header<<"Content-Length: "<<message_size<<'\n';
	header<<"Content-Type: "<<mime_type<<"; charset=UTF-8\n";

	//Support Compressed Files
	if(mime_type=="application.x-gzip"||compressed)
		header<<"Content-Encoding: gzip\n";

	//Set Connection Type
	if(keepalive)
		header<<"Connection: keep-alive\r\n\r\n";
	else
		header<<"Connection: close\r\n\r\n";

	//Return HTML Header
	return header.str();
}

//HTTP to ASCII Function (Converts a string with http symbols to ascii symbols)
std::string msl::http_to_ascii(std::string symbols)
{
	//Replace Newlines
	while(true)
	{
		//Find a Newline
		int pos=symbols.find("%0D%0A");

		//If No Newlines Break
		if(pos==-1)
			break;

		//Replace Newlines
		symbols.replace(pos,6,"\n");
	}

	//Parse String
	for(unsigned int ii=0;ii<symbols.size();++ii)
	{
		//Found a Symbol
		if(symbols[ii]=='%')
		{
			//Check for Invalid Symbols
			if(ii+2<symbols.size()&&(isdigit(symbols[ii+1])||(symbols[ii+1]>='A'&&symbols[ii+1]<='F'))
				&&(isdigit(symbols[ii+2])||(symbols[ii+2]>='A'&&symbols[ii+2]<='F')))
			{
				//Temp Character
				char symbol=0;

				//Get 1's Digit
				if(symbols[ii+2]>'9')
					symbol+=symbols[ii+2]-65+10;
				else
					symbol+=symbols[ii+2]-48;

				//Get 16's Digit
				if(symbols[ii+1]>'9')
					symbol+=(symbols[ii+1]-65+10)*16;
				else
					symbol+=(symbols[ii+1]-48)*16;

				//Insert Symbol
				symbols[ii]=symbol;

				//Remove Extra Space
				symbols.erase(ii+1,2);
			}
		}
	}

	//Return Converted String
	return symbols;
}

//HTTP Pack String Function (Packages a string for http travel)
std::string msl::http_pack_string(const std::string& message,const std::string& mime_type,
	const bool compressed,const bool keepalive)
{
	return msl::http_create_header(message.size(),mime_type,compressed,keepalive)+message;
}