//Web Server Threaded Header
//	Created By:		Mike Moss
//	Modified On:	10/03/2013

//Required Libraries:
// 	pthread
//	wsock32 (windows only)

//Begin Define Guards
#ifndef MSL_WEBSERVER_THREADED_H
#define MSL_WEBSERVER_THREADED_H

//Socket Header
#include "socket.hpp"

//String Header
#include <string>

//Thread Header
#include <pthread.h>

//Vector Header
#include <vector>

//MSL Namespace
namespace msl
{
	//Web Server Threaded Class Declaration
	class webserver_threaded
	{
		public:
			//Constructor (Default)
			webserver_threaded(const std::string& address,bool(*user_service_client)(msl::socket& client,const std::string& message)=NULL,
				const std::string& web_directory="web");

			//Boolean Operator (Tests if Server is Good)
			operator bool() const;

			//Not Operator (For Boolean Operator)
			bool operator!() const;

			//Good Function (Tests if Server is Good)
			bool good() const;

			//Setup Function (Creates Socket)
			void setup();

			//Update Function (Connects Clients and Runs Web Server)
			void update();

			//Close Function (Closes Server)
			void close();

			//Max Size Accessor (Accesses max upload size, in bytes.  Default is 200 MB.)
			unsigned int get_max_upload_size() const;

			//Max Size Mutator (Changes max upload size, in bytes.  Default is 200 MB.)
			void set_max_upload_size(const unsigned int size);

		private:
			//Member Variables
			bool(*_user_service_client)(msl::socket& client,const std::string& message);
			msl::socket _socket;
			std::string _web_directory;
			unsigned long _max_upload_size;
	};
}

//End Define Guards
#endif

//Example (You need to make a folder called web and put index.html and not_found.html, located in comments below this example, in it for this to work)
/*
//Threaded Web Server Source
//	Created By:		Mike Moss
//	Modified On:	10/03/2013

//IO Stream Header
#include <iostream>

//Socket Header
#include "msl/socket.hpp"

//Socket Utility Header
#include "msl/socket_util.hpp"

//String Stream Header
#include <sstream>

//Web Server Threaded Header
#include "msl/webserver_threaded.hpp"

//Our Service Client Function Declaration
bool service_client(msl::socket& client,const std::string& message);

//Main
int main(int argc,char* argv[])
{
	//Create Port
	std::string server_port="8080";

	//Get Command Line Port
	if(argc>1)
		server_port=argv[1];

	//Create Server
	msl::webserver_threaded server("0.0.0.0:"+server_port,service_client);
	server.setup();

	//Check Server
	if(server.good())
		std::cout<<"=)"<<std::endl;
	else
		std::cout<<"=("<<std::endl;

	//Be a server...forever...
	while(true)
	{
		//Update Server
		server.update();

		//Give OS a Break
		usleep(0);
	}

	//Call Me Plz T_T
	return 0;
}

//Our Service Client Function Definition
bool service_client(msl::socket& client,const std::string& message)
{
	//Create Parser
	std::istringstream istr(message);

	//Parse the Request
	std::string request;
	istr>>request;
	istr>>request;

	//Translate Request
	request=msl::http_to_ascii(request);

	//Check For a Custom Request
	if(request=="/custom_request")
	{
		//Send Custom Message
		client<<"custom request detected!";

		//Close Client Connection (Not a Keep-Alive Request)
		client.close();

		//Return True (We serviced the client)
		return true;
	}

	//Default Return False (We did not service the client)
	return false;
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