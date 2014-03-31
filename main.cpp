//PCB2GCODE Web Server Source
//	Created By:		Mike Moss
//	Modified On:	11/12/2013

//File Utility Header
#include "msl/file_util.hpp"

//IO Stream Header
#include <iostream>

//JSON Header
#include "msl/json.hpp"

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

//Web Server Threaded Header
#include "msl/webserver_threaded.hpp"

//Unix Dependencies
#include <stdlib.h>

//UNIX ONLY!!!  PCB2GCode is Linux only, so I didn't really fight it...

//Our Service Client Function Declaration
bool service_client(msl::socket& client,const std::string& message);

std::string fix_newlines(std::string str)
{
	size_t pos=0;

	while((pos=str.find("\\r",pos))!=std::string::npos)
	{
		str.replace(pos,4,"\n");
		pos+=1;
	}

	pos=0;

	while((pos=str.find("\\n",pos))!=std::string::npos)
	{
		str.replace(pos,2,"\n");
		pos+=1;
	}

	return str;
}

//Global Defaults Object
std::string pcb2gcode_defaults=
	"{\"z\":{\"work\":-0.007,\"safe\":0.2,\"cut\":-0.007,\"drill\":-0.075,\"change\":0.2},"
	"\"cut\":{\"diameter\":0.1250,\"feed\":3,\"speed\":1000,\"infeed\":1},"
	"\"outline\":{\"fill\":false,\"width\":0},"
	"\"mill\":{\"feed\":3,\"speed\":1000},"
	"\"drill\":{\"feed\":3,\"speed\":1000},"
	"\"other\":{\"offset\":\"0.025\",\"extra_passes\":0,\"use_mill_as_drill\":false,"
	"\"drill_front\":true,\"mirror_absolute\":false,\"return_home\":true}}";

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
	server.set_max_upload_size(2*90000000);
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
	if(msl::starts_with(request,"/defaults"))
	{
		//Create Response String
		std::string response=msl::http_pack_string(pcb2gcode_defaults,"application/json");

		//Send Response
		client.write(response.c_str(),response.size());

		//Close Client Connection (Not a Keep-Alive Request)
		client.close();

		//Return True (We serviced the client)
		return true;
	}

	//Check For a Custom Request
	else if(msl::starts_with(request,"/pcb2gcode="))
	{
		//Extract JSON Object
		std::string json_str=request.substr(11,request.size()-11);

		//Create JSON Objects
		msl::json json_obj(json_str);
		msl::json json_options(json_obj.get("options"));

		//Selection Booleans for Later
		bool error=false;
		bool picture=false;
		bool front=false;
		bool drill=false;

		//Create a unique id, this is based on the system time...
		unsigned long thread_id_number=msl::millis();

		while(msl::file_exists(msl::to_string(thread_id_number)))
			++thread_id_number;

		//Make Unique Id String
		std::string thread_id=msl::to_string(thread_id_number);

		//Create Directory Out of Unique Id
		std::string mkdir="mkdir "+thread_id;

		if(system(mkdir.c_str()))
		{}

		//Build PCB2GCode Command
		std::string pcb2gcode_command="cp home.sh "+thread_id+" && cd "+thread_id+" && pcb2gcode ";

		//Pictures
		if(json_obj.get("picture").size()>0)
		{
			std::string escaped=fix_newlines(json_obj.get("picture"));


			msl::string_to_file(escaped,thread_id+"/picture.test");
			pcb2gcode_command+="--front picture.test ";
			pcb2gcode_command+="--front-output picture.ngc ";
			picture=true;
		}

		//Mill Layers
		else if(json_obj.get("front").size()>0)
		{
			std::string escaped=fix_newlines(json_obj.get("front"));

			msl::string_to_file(escaped,thread_id+"/front.test");
			pcb2gcode_command+="--front front.test ";
			pcb2gcode_command+="--front-output front.ngc ";
			front=true;
		}

		//Drill Layers
		else if(json_obj.get("drill").size()>0)
		{
			//std::cout<<"\t\t\t\tgot a drill file!!!"<<std::endl;

			std::string escaped=fix_newlines(json_obj.get("drill"));

			msl::string_to_file(escaped,thread_id+"/drill.test");
			pcb2gcode_command+="--drill drill.test ";
			pcb2gcode_command+="--drill-output drill.ngc ";
			drill=true;
		}

		//Check for a Gerber File
		if(picture||front||drill)
		{
			//Extract Z Options
			msl::json json_z(json_options.get("z"));
				pcb2gcode_command+="--zwork "+msl::to_string(msl::to_double(json_z.get("work")))+" ";
				pcb2gcode_command+="--zsafe "+msl::to_string(msl::to_double(json_z.get("safe")))+" ";
				pcb2gcode_command+="--zcut "+msl::to_string(msl::to_double(json_z.get("cut")))+" ";
				pcb2gcode_command+="--zdrill "+msl::to_string(msl::to_double(json_z.get("drill")))+" ";
				pcb2gcode_command+="--zchange "+msl::to_string(msl::to_double(json_z.get("change")))+" ";

			//Extract Cutter Information
			msl::json json_cut(json_options.get("cut"));
				pcb2gcode_command+="--cutter-diameter "+msl::to_string(msl::to_double(json_cut.get("diameter")))+" ";
				pcb2gcode_command+="--cut-feed "+msl::to_string(msl::to_double(json_cut.get("feed")))+" ";
				pcb2gcode_command+="--cut-speed "+msl::to_string(msl::to_double(json_cut.get("speed")))+" ";
				pcb2gcode_command+="--cut-infeed "+msl::to_string(msl::to_double(json_cut.get("infeed")))+" ";

			//Extract Cutter Information
			msl::json json_outline(json_options.get("outline"));

				if(msl::to_bool(json_outline.get("fill")))
				{
					pcb2gcode_command+="--fill-outline ";
					pcb2gcode_command+="--outline-width "+msl::to_string(msl::to_double(json_outline.get("width")))+" ";
				}

			//Extract Mill Information
			msl::json json_mill(json_options.get("mill"));
				pcb2gcode_command+="--mill-feed "+msl::to_string(msl::to_double(json_mill.get("feed")))+" ";
				pcb2gcode_command+="--mill-speed "+msl::to_string(msl::to_double(json_mill.get("speed")))+" ";

			//Extract Drill Information
			msl::json json_drill(json_options.get("drill"));
				pcb2gcode_command+="--drill-feed "+msl::to_string(msl::to_double(json_drill.get("feed")))+" ";
				pcb2gcode_command+="--drill-speed "+msl::to_string(msl::to_double(json_drill.get("speed")))+" ";

			//Extract Other Information
			msl::json json_other(json_options.get("other"));
				pcb2gcode_command+="--offset "+msl::to_string(msl::to_double(json_other.get("offset")))+" ";
				pcb2gcode_command+="--extra-passes "+msl::to_string(msl::to_int(json_other.get("extra_passes")))+" ";

			if(msl::to_bool(json_other.get("use_mill_as_drill")))
				pcb2gcode_command+="--milldrill ";

			if(msl::to_bool(json_other.get("drill_front")))
				pcb2gcode_command+="--drill-front ";

			if(msl::to_bool(json_other.get("mirror_absolute")))
				pcb2gcode_command+="--mirror-absolute ";

			if(msl::to_bool(json_other.get("return_home")))
			{
				if(picture)
					pcb2gcode_command+="&& ./home.sh picture.ngc ";

				else if(front)
					pcb2gcode_command+="&& ./home.sh front.ngc ";

				else if(drill)
					pcb2gcode_command+="&& ./home.sh drill.ngc ";
			}

			//std::cout<<"COMMAND\n"<<pcb2gcode_command<<"\nCOMMAND"<<std::endl;

			//Execute PCB2GCode Command
			if(system(pcb2gcode_command.c_str())!=-1)
			{
				std::string response;

				if(picture)
				{
					std::string gcode;

					if(msl::file_to_string(thread_id+"/outp0_original_front.png",gcode))
						response=msl::http_pack_string(gcode,"image/png");
					else
						error=true;
				}

				else if(front)
				{
					std::string gcode;

					if(msl::file_to_string(thread_id+"/front.ngc",gcode))
						response=msl::http_pack_string(gcode,"text/plain");
					else
						error=true;
				}

				else if(drill)
				{
					std::string gcode;

					if(msl::file_to_string(thread_id+"/drill.ngc",gcode))
						response=msl::http_pack_string(gcode,"text/plain");
					else
						error=true;
				}

				//Send GCode
				if(!error)
					client.write(response.c_str(),response.size());
			}
		}

		//On Error return "error"
		if(error)
		{
			std::string response="error";
			client.write(response.c_str(),response.size());
		}

		//Cleanup files...the sleep seems to make life better...
		std::string rmdir="sleep 0.1 && rm -rf "+thread_id;

		if(system(rmdir.c_str()))
		{}

		//Close Client Connection (Not a Keep-Alive Request)
		client.close();

		//Return True (We serviced the client)
		return true;
	}

	//Default Return False (We did not service the client)
	return false;
}