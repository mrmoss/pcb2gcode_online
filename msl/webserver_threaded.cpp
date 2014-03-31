//Web Server Threaded Source
//	Created By:		Mike Moss
//	Modified On:	03/30/2014

//Required Libraries:
// 	pthread
//	Ws2_32 (windows only)

//Definitions for "webserver_threaded.hpp"
#include "webserver_threaded.hpp"

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

//Thread Argument Passing Container
class client_thread_arg
{
	public:
		msl::socket socket;
		std::string web_directory;
		bool(*user_service_client)(msl::socket& client,const std::string& message);
		unsigned int max_upload_size;
};

//Static Global Service Client Function
void service_client(msl::socket client,const std::string&message,const std::string web_directory,bool(*user_service_client)(msl::socket& client,const std::string& message))
{
	//If User Options Fail
	if(user_service_client==NULL||!user_service_client(client,message))
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
			if(msl::file_to_string(web_directory+request,file,true))
			{
				std::string response_str=msl::http_pack_string(file,mime_type,false);
				client.write(response_str.c_str(),response_str.size(),120000);
			}

			//Bad File
			else if(msl::file_to_string(web_directory+"/not_found.html",file,true))
			{
				std::string response_str=msl::http_pack_string(file);
				client.write(response_str.c_str(),response_str.size(),120000);
			}

			//No Bad File
			else
			{
				std::string response_str=msl::http_pack_string("sorry...");
				client.write(response_str.c_str(),response_str.size(),120000);
			}
		}

		//Other Requests (Just kill connection...it's either hackers or idiots...)
		else
		{
			client.close();
		}
	}
}

//Static Global Client Thread Function
static void* client_thread(void* args)
{
	//Extract Thread Argument Container
	client_thread_arg* client_data=(client_thread_arg*)args;

	//Check Thread Argument Container
	if(client_data!=NULL)
	{
		//Client Message Buffer
		std::string message="";

		//Keep Getting Messages
		while(true)
		{
			//Give OS a Break
			usleep(0);

			//Temp
			char byte='\n';

			//Get a Byte
			while(client_data->socket.available()>0&&client_data->socket.read(&byte,1)==1)
			{
				//Add the Byte to Client Buffer
				message+=byte;

				//Check for an End Byte
				if(msl::ends_with(message,"\r\n\r\n")||message.size()>=client_data->max_upload_size)
				{
					service_client(client_data->socket,message,client_data->web_directory,
						client_data->user_service_client);
					message.clear();
				}
			}

			//Disconnect Bad Clients
			if(!client_data->socket.good())
			{
				client_data->socket.close();
				break;
			}
		}
	}

	//Clean Up Thread Argument Container
	delete client_data;

	//Return No Data
	return NULL;
}

//Constructor (Default)
msl::webserver_threaded::webserver_threaded(const std::string& address,bool(*user_service_client)(msl::socket& client,const std::string& message),
	const std::string& web_directory):_user_service_client(user_service_client),_socket(address),_web_directory(web_directory),
	_max_upload_size(2*1000000)
{}

//Boolean Operator (Tests if Server is Good)
msl::webserver_threaded::operator bool() const
{
	return good();
}

//Not Operator (For Boolean Operator)
bool msl::webserver_threaded::operator!() const
{
	return !good();
}

//Good Function (Tests if Server is Good)
bool msl::webserver_threaded::good() const
{
	return _socket.good();
}

//Setup Function (Creates Socket)
void msl::webserver_threaded::setup()
{
	_socket.create_tcp();
}

//Update Function (Connects Clients and Runs Server)
void msl::webserver_threaded::update()
{
	//Check for a Connecting Client
	msl::socket client=_socket.accept();

	//If Client Connected
	if(client.good())
	{
		//Create a Thread
		pthread_t new_client_thread;

		//Create Thread Argument Container
		client_thread_arg* new_client_data=new client_thread_arg;
		new_client_data->socket=client;
		new_client_data->web_directory=_web_directory;
		new_client_data->user_service_client=_user_service_client;
		new_client_data->max_upload_size=get_max_upload_size();

		//Create Thread
		if(pthread_create(&new_client_thread,NULL,&client_thread,(void*)new_client_data)==0)
			pthread_detach(new_client_thread);

		//On Error Delete Thread Argument Container
		else
			delete new_client_data;
	}

	//Give OS a Break
	usleep(0);
}

//Close Function (Closes Server) (Warning!!!  This doesn't close all the threads, there is no way to kill a running joined thread in C++11...yet...)
void msl::webserver_threaded::close()
{
	_socket.close();
}

//Max Size Accessor (Accesses max upload size, in bytes.  Default is 200 MB.)
unsigned int msl::webserver_threaded::get_max_upload_size() const
{
	return _max_upload_size;
}

//Max Size Mutator (Changes max upload size, in bytes.  Default is 200 MB.)
void msl::webserver_threaded::set_max_upload_size(const unsigned int size)
{
	_max_upload_size=size;
}