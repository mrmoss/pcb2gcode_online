//File Utility Source
//	Created By:		Mike Moss
//	Modified On:	11/08/2013

//Requires POSIX File System, if on Windows running Visual Studios, use:
//	http://www.softagalleria.net/dirent.php

//Begin Define Guards
#ifndef MSL_FILE_UTIL_H
#define MSL_FILE_UTIL_H

//JSON Header
#include "json.hpp"

//String Header
#include <string>

//Vector Header
#include <vector>

//MSL Namespace
namespace msl
{
	//List Directory Function (Lists files in directory as strings in a vector)
	std::vector<std::string> list_directory(const std::string& path_name=".");

	//List Directory Files Function (Lists files in directory as strings in a vector)
	std::vector<std::string> list_directory_files(const std::string& path_name=".");

	//List Directory Folders Function (Lists folders in directory as strings in a vector)
	std::vector<std::string> list_directory_folders(const std::string& path_name=".");

	//List Directory JSON Function (Lists directories and files inside them recursively, returned as a JSON object)
	msl::json list_directory_json(const std::string& path_name=".");

	//File to String Function (Loads a file from disk as a string, returns false on error)
	bool file_to_string(const std::string& filename,std::string& data,const bool binary=false);

	//String to File Function (Saves a string to disk as a file, returns false on error)
	bool string_to_file(const std::string& data,const std::string& filename,const bool binary=false);

	//Remove File Function (Removes a file from disk, returns false on error)
	bool remove_file(const std::string& filename);

	//Remove Directory Function (Removes a directory from disk recursively, returns false on error)
	bool remove_directory(const std::string& directory);

	//File Exists Function (Checks if a file exists in path)
	bool file_exists(const std::string& filename,const std::string& path_name=".");
}

//End Define Guards
#endif

//Example
/*
#include "msl/file_util.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace msl;

int main()
{
	//List files in current directory
	std::cout<<"Directory listing for \".\""<<std::endl;
	std::vector<std::string> list=list_directory(".");

	for(unsigned int ii=0;ii<list.size();++ii)
		std::cout<<"\t"<<list[ii]<<std::endl;

	std::cout<<std::endl;

	//Load a file
	std::string save_file_name="file.txt";
	std::string save_file_data="";

	if(file_exists(save_file_name)&&file_to_string(save_file_name,save_file_data))
		std::cout<<"Contents of file named \""<<save_file_name<<"\""<<std::endl<<save_file_data<<std::endl;

	//Save a string to a file
	std::string load_file_data="hello world!";
	std::string load_file_name="hello.txt";

	if(string_to_file(load_file_data,load_file_name))
		std::cout<<"Saved \""<<load_file_data<<"\" to the file named \""<<load_file_name<<"\""<<std::endl<<std::endl;

	//Remove a file
	std::string remove_file_name="remove_me.txt";

	if(file_exists(remove_file_name)&&remove_file(remove_file_name)&&!file_exists(remove_file_name))
		std::cout<<"Removed file named \""<<remove_file_name<<"\""<<std::endl<<std::endl;

	//Remove a directory
	std::string directory="directory_to_remove";

	if(remove_directory(directory))
		std::cout<<"Removed directory named \""<<directory<<"\""<<std::endl;

	return 0;
}
*/