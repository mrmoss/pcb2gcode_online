//Socket Header
//	Created By:		Mike Moss
//	Modified On:	10/16/2013

//Required Libraries:
//	Ws2_32 (windows only)

//Begin Define Guards
#ifndef MSL_SOCKET_H
#define MSL_SOCKET_H

//String Header
#include <string>

//String Stream Header
#include <sstream>

//Windows Dependencies
#if(defined(_WIN32)&&!defined(__CYGWIN__))
	#include <winsock2.h>

	#if(!defined(socklen_t))
		typedef int socklen_t;
	#endif

//Unix Dependencies
#else
	#include <netinet/in.h>
	#include <unistd.h>
	#include <fcntl.h>

	#ifndef SOCKET
		#define SOCKET unsigned int
		#define INVALID_SOCKET (SOCKET)(~0)
		#define SOCKET_ERROR (-1)
	#endif
#endif

//MSL Namespace
namespace msl
{
	//Socket Class Pre-Declaration(For msl::ipv4)
	class socket;

	//IPv4 Address Class Declaration
	class ipv4
	{
		public:
			//Constructor (Default)
			ipv4(const unsigned char ip[4]=NULL,const unsigned short port=0);

			//Copy Constructor
			ipv4(const msl::ipv4& copy);

			//Copy Assignment Operator
			msl::ipv4& operator=(const msl::ipv4& copy);

			//Build Function (Returns Raw Socket Address)
			sockaddr_in build() const;

			//String Accessor (X.X.X.X:PORT)
			std::string str() const;

			//Socket Class Friend
			friend class msl::socket;

		private:
			//Member Variables
			unsigned char _ip[4];
			unsigned short _port;
	};

	//Socket Class Declaration (NOT SURE IF BEING A STD::OSTREAM CHILD IS THE WAY TO GO HERE)
	class socket:public std::ostream
	{
		public:
			//Constructor (Default)
			socket(const std::string& address="0.0.0.0:0");

			//Copy Constructor
			socket(const msl::socket& copy);

			//Copy Assignment Operator
			socket& operator=(const msl::socket& copy);

			//Boolean Operator (Tests if Socket is Good)
			operator bool() const;

			//Not Operator (For Boolean Operator)
			bool operator!() const;

			//Good Function (Tests if Socket is Good)
			bool good() const;

			//Create Functions (Hosts a Socket Locally)
			void create_tcp();
			void create_udp(const unsigned int buffersize);

			//Connect Functions (Connects to a Remote Socket)
			void connect_tcp();
			void connect_udp();

			//Close Function (Closes a Local Socket)
			void close();

			//Accept Function (Accepts a Remote Connection to a Local Socket)
			msl::socket accept();

			//Available Function (Checks if there are Bytes to be Read)
			int available() const;

			//Read Function (Returns Number of Bytes Read, -1 on Error)
			int read(void* buffer,const unsigned int size,const long time_out=0,const int flags=0) const;

			//Write Function (Returns Number of Bytes Sent, -1 on Error)
			int write(const void* buffer,const unsigned int size,const long time_out=0,const int flags=0) const;

			//IP Address Accessor (Read Only)
			msl::ipv4 ip() const;

			//System Socket Accessor
			SOCKET system_socket() const;

			//Stream Out Operator
			template <typename T> friend msl::socket& operator<<(msl::socket& lhs,const T& rhs);

		private:
			//Member Variables
			msl::ipv4 _address;
			SOCKET _socket;
			bool _hosting;
	};

	//Socket Class Stream Operator (Templated Function)
	template <typename T> msl::socket& operator<<(msl::socket& lhs,const T& rhs)
	{
		//Create a String Stream
		std::ostringstream ostr;

		//Put in Data
		ostr<<rhs;

		//Write Data
		lhs.write(reinterpret_cast<void*>(const_cast<char*>(ostr.str().c_str())),ostr.str().size(),200);

		//Return Stream
		return lhs;
	}
}

//Socket Initialize Function (Sets up the use of sockets, operating system dependent...)
void socket_init();

//Socket Create Function (Hosts a Socket Locally)
SOCKET socket_create(const msl::ipv4 ip,const long time_out=0,const bool UDP=false,const unsigned int buffersize=200);

//Socket Connection Function (Connects to a Remote Socket)
SOCKET socket_connect(const msl::ipv4 ip,const long time_out=0,const bool UDP=false);

//Socket Accept Function (Accepts a Remote Connection to a Local Socket)
SOCKET socket_accept(const SOCKET socket,msl::ipv4& client_ip,const long time_out=0);

//Socket Close Function (Closes a Local Socket)
SOCKET socket_close(const SOCKET socket);

//Socket Available Function (Checks if there are Bytes to be Read, -1 on Error)
int socket_available(const SOCKET socket,const long time_out=0);

//Socket Peek Function (Same as socket_read but Leaves Bytes in Socket Buffer)
int socket_peek(const SOCKET socket,void* buffer,const unsigned int size,const long time_out=0,const int flags=0);

//Socket Read Function (Returns Number of Bytes Read, -1 on Error)
int socket_read(const SOCKET socket,void* buffer,const unsigned int size,const long time_out=0,const int flags=0);

//Socket Write Function (Returns Number of Bytes Sent, -1 on Error)
int socket_write(const SOCKET socket,const void* buffer,const unsigned int size,const long time_out=200,const int flags=0);

//End Define Guards
#endif

//Example (You need to make a folder called web and put index.html and not_found.html, located in comments below this example, in it for this to work)
/*
//Basic Web Server Source
//	Created By:		Mike Moss
//	Modified On:	04/25/2013

//File Utility Header
#include "msl/file_util.hpp"

//IO Stream Header
#include <iostream>

//Socket Header
#include "msl/socket.hpp"

//Socket Utility Header
#include "msl/socket_util.hpp"

//String Header
#include <string>

//String Stream Header
#include <sstream>

//String Utility Header
#include "msl/string_util.hpp"

//Time Utility Header
#include "msl/time_util.hpp"

//Vector Header
#include <vector>

//Service Client Function Declaration
void service_client(msl::socket& client,const std::string& message);

//Main
int main(int argc,char* argv[])
{
	//Create Port
	std::string server_port="8080";

	//Get Command Line Port
	if(argc>1)
		server_port=argv[1];

	//Create Server
	msl::socket server("0.0.0.0:"+server_port);
	server.create_tcp();

	//Check Server
	if(server.good())
		std::cout<<"=)"<<std::endl;
	else
		std::cout<<"=("<<std::endl;

	//Vectors for Clients
	std::vector<msl::socket> clients;
	std::vector<std::string> client_messages;

	//Be a server...forever...
	while(true)
	{
		//Check for a Connecting Client
		msl::socket client=server.accept();

		//If Client Connected
		if(client.good())
		{
			clients.push_back(client);
			client_messages.push_back("");
		}

		//Handle Clients
		for(unsigned int ii=0;ii<clients.size();++ii)
		{
			//Service Good Clients
			if(clients[ii].good())
			{
				//Temp
				char byte='\n';

				//Get a Byte
				if(clients[ii].available()>0&&clients[ii].read(&byte,1)==1)
				{
					//Add the Byte to Client Buffer
					client_messages[ii]+=byte;

					//Check for an End Byte
					if(msl::ends_with(client_messages[ii],"\r\n\r\n"))
					{
						service_client(clients[ii],client_messages[ii]);
						client_messages[ii].clear();
					}
				}
			}

			//Disconnect Bad Clients
			else
			{
				clients[ii].close();
				clients.erase(clients.begin()+ii);
				client_messages.erase(client_messages.begin()+ii);
				--ii;
			}
		}

		//Give OS a Break
		usleep(0);
	}

	//Call Me Plz T_T
	return 0;
}

//Service Client Function Definition
void service_client(msl::socket& client,const std::string& message)
{
	//Get Requests
	if(msl::starts_with(message,"GET"))
	{
		//Create Parser
		std::istringstream istr(msl::http_to_ascii(message));

		//Parse the Request
		std::string request;
		istr>>request;
		istr>>request;

		//Web Root Variable (Where your web files are)
		std::string web_root="web";

		//Check for Index
		if(request=="/")
			request="/index.html";

		//Mime Type Variable (Default plain text)
		std::string mime_type="text/plain";

		//Check for Code Mime Type
		if(msl::ends_with(request,".js"))
			mime_type="application/x-javascript";

		//Check for Images Mime Type
		else if(msl::ends_with(request,".gif"))
			mime_type="image/gif";
		else if(msl::ends_with(request,".jpeg"))
			mime_type="image/jpeg";
		else if(msl::ends_with(request,".png"))
			mime_type="image/png";
		else if(msl::ends_with(request,".tiff"))
			mime_type="image/tiff";
		else if(msl::ends_with(request,".svg"))
			mime_type="image/svg+xml";
		else if(msl::ends_with(request,".ico"))
			mime_type="image/vnd.microsoft.icon";

		//Check for Text Mime Type
		else if(msl::ends_with(request,".css"))
			mime_type="text/css";
		else if(msl::ends_with(request,".htm")||msl::ends_with(request,".html"))
			mime_type="text/html";

		//File Data Variable
		std::string file;

		//Load File
		if(msl::file_to_string(web_root+request,file,true))
			client<<msl::http_pack_string(file,mime_type,false);

		//Bad File
		else if(msl::file_to_string(web_root+"/not_found.html",file,true))
			client<<msl::http_pack_string(file);

		//Close Connection
		client.close();
	}

	//Other Requests (Just kill connection...it's either hackers or idiots...)
	else
	{
		client.close();
	}
}
*/

//index.html
/*
<html>
	<head>
		<title>Your here!</title>
	</head>
	<body>
		<center>Now go away...</center>
	</body>
</html>
*/

//not_found.html
/*
<html>
	<head>
		<title>Not found!</title>
	</head>
	<body>
		<center>T_T</center>
	</body>
</html>
*/