//String Utility Header
//	Created By:		Mike Moss
//	Modified On:	11/17/2013

//Begin Define Guards
#ifndef MSL_STRING_UTIL_H
#define MSL_STRING_UTIL_H

//String Header
#include <string>

//String Stream Header
#include <sstream>

//MSL Namespace
namespace msl
{
	//String Convert Functions
	bool to_bool(const std::string& value);
	double to_double(const std::string& value);
	int to_int(const std::string& value);
	char to_char(const std::string& value);
	unsigned char to_uchar(const std::string& value);

	//To String Convert Function
	template<typename T> std::string to_string(const T& value)
	{
		//Convert Variables
		std::ostringstream ostr;

		//Set Precision to 10 Digits
		ostr.precision(10);

		//Convert
		ostr<<value;

		//Return Converted Value
		return ostr.str();
	}

	//String Write Function (Writes the bytes of an object to a string)
	template<typename T> std::string string_write(const T& item)
	{
		std::string return_str="";

		for(unsigned int ii=0;ii<sizeof(T);++ii)
			return_str+=((char*)&item)[ii];

		return return_str;
	}

	//String Read Function (Reads the bytes of an object from a string)
	template<typename T> T string_read(const std::string& str)
	{
		return *(T*)str.c_str();
	}

	//Starts With Function (Checks if string starts with another string)
	bool starts_with(const std::string& str,const std::string& start);

	//Ends With Function (Checks if string ends with another string)
	bool ends_with(const std::string& str,const std::string& end);

	//To Lower Function (Returns a copy of a string that is all lower case)
	std::string to_lower(const std::string& str);

	//To Upper Function (Returns a copy of a string that is all upper case)
	std::string to_upper(const std::string& str);

	//Extract Until Function (Returns a string containing data from the start of the passed string until
	//	the delimiter is reached).
	std::string extract_until(const std::string& str,const char until,const bool include_delimiter=false);

	//Extract Between Function (Returns a string containing data between open and close characters.
	//	Supports different and same open/close characters.)
	std::string extract_between(const std::string& str,const char open_char,const char close_char,
		const bool include_delimiters=false);
}

//End Define Guards
#endif

//Example
/*
#include <iostream>
#include <string>
#include "msl/string_util.hpp"
using namespace msl;

int main()
{
	std::cout<<"Convert from string:"<<std::endl;
	std::string str_double="3.1415";
	std::string str_bool="false";
	std::string str_int="1234";

	std::cout<<"\t"<<to_double(str_double)<<std::endl;
	std::cout<<"\t"<<to_bool(str_bool)<<std::endl;
	std::cout<<"\t"<<to_int(str_int)<<std::endl<<std::endl;

	std::cout<<"Convert to string:"<<std::endl;
	double test_double=89.9;
	bool test_bool=true;
	int test_int=255;

	std::cout<<"\t"<<to_string(test_double)<<std::endl;
	std::cout<<"\t"<<to_string(test_bool)<<std::endl;
	std::cout<<"\t"<<to_string(test_int)<<std::endl<<std::endl;

	std::cout<<"String starts and ends:"<<std::endl;
	std::string test="Hello world!";

	if(starts_with(test,"Hello"))
		std::cout<<"\tString starts with \"Hel\""<<std::endl;

	if(ends_with(test,"world!"))
		std::cout<<"\tString ends with \"rld!\""<<std::endl<<std::endl;

	std::string case_test="hElLo ThErE pArTnEr!";
	std::cout<<to_lower(case_test)<<std::endl;
	std::cout<<to_upper(case_test)<<std::endl;

	return 0;
}
*/