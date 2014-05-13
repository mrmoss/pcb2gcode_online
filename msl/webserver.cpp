//Web Server Source
//	Created By:		Mike Moss
//	Modified On:	04/11/2014

//Required Libraries:
//	Ws2_32 (windows only)

//Definitions for "webserver.hpp"
#include "webserver.hpp"

//File Utility Header
#include "file_util.hpp"

//Socket Utility Header
#include "socket_util.hpp"

//String Stream Header
#include <sstream>

//String Utility Header
#include "string_util.hpp"

//Time Utility Header
#include "time_util.hpp"

//Constructor (Default)
msl::webserver::webserver(const std::string& address,bool(*user_service_client)(msl::socket& client,const std::string& message),
	const std::string& web_directory):_user_service_client(user_service_client),_socket(address),_web_directory(web_directory)
{}

//Boolean Operator (Tests if Server is Good)
msl::webserver::operator bool() const
{
	return good();
}

//Not Operator (For Boolean Operator)
bool msl::webserver::operator!() const
{
	return !good();
}

//Good Function (Tests if Server is Good)
bool msl::webserver::good() const
{
	return _socket.good();
}

//Setup Function (Creates Socket)
void msl::webserver::setup()
{
	_socket.create_tcp();
}

//Update Function (Connects Clients and Runs Server)
void msl::webserver::update()
{
	//Check for a Connecting Client
	msl::socket client=_socket.accept();

	//If Client Connected
	if(client.good())
	{
		_clients.push_back(client);
		_client_messages.push_back("");
	}

	//Handle Clients
	for(unsigned int ii=0;ii<_clients.size();++ii)
	{
		//Dead Check Variable
		bool dead=false;

		//Get a Byte
		while(_clients[ii].good()&&_clients[ii].available()>0)
		{
			//Temp
			char byte='\n';

			//Get a Byte
			if(_clients[ii].read(&byte,1)==1)
			{
				//Add the Byte to Client Buffer
				_client_messages[ii]+=byte;

				//Check for an End Byte
				if(msl::ends_with(_client_messages[ii],"\r\n\r\n"))
				{
					service_client(_clients[ii],_client_messages[ii]);
					_client_messages[ii].clear();
				}
			}

			//If There are Bytes to Read But None are Readable then Client is "dead"
			else
			{
				dead=true;
				break;
			}
		}

		//Disconnect Bad Clients
		if(!_clients[ii].good()||dead)
		{
			_clients[ii].close();
			_clients.erase(_clients.begin()+ii);
			_client_messages.erase(_client_messages.begin()+ii);
			--ii;
		}
	}


	//Give OS a Break
	msl::nsleep(1000000);
}

//Close Function (Closes Server)
void msl::webserver::close()
{
	//Close Down Clients
	for(unsigned int ii=0;ii<_clients.size();++ii)
		_clients[ii].close();

	//Resize Clients
	_clients.resize(0);
	_client_messages.resize(0);

	//Close Server
	_socket.close();
}

//Service Client Function Definition
void msl::webserver::service_client(msl::socket& client,const std::string& message)
{
	//If User Options Fail
	if(_user_service_client==NULL||!_user_service_client(client,message))
	{
		//Get Requests
		if(msl::starts_with(message,"GET"))
		{
			//Create Parser
			std::istringstream istr(message);

			//Parse the Request
			std::string request;
			istr>>request;
			istr>>request;

			//Translate Request
			request=msl::http_to_ascii(request);

			//Remove ../'s
			request=msl::search_and_replace(request,"..","");

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
			if(msl::file_to_string(_web_directory+request,file,true))
			{
				std::string response_str=msl::http_pack_string(file,mime_type,false);
				client.write(response_str.c_str(),response_str.size());
			}

			//Bad File
			else if(msl::file_to_string(_web_directory+"/not_found.html",file,true))
			{
				std::string response_str=msl::http_pack_string(file);
				client.write(response_str.c_str(),response_str.size());
			}

			//No Bad File
			else
			{
				std::string response_str=msl::http_pack_string("sorry...");
				client.write(response_str.c_str(),response_str.size());
			}
		}

		//Other Requests (Just kill connection...it's either hackers or idiots...)
		else
		{
			client.close();
		}
	}
}
