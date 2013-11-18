//String Utility Source
//	Created By:		Mike Moss
//	Modified On:	11/17/2013

//Definitions for "string_util.hpp"
#include "string_util.hpp"

//Algorithm Header
#include <algorithm>

//Convert To Boolean Function
bool msl::to_bool(const std::string& value)
{
	if(msl::to_lower(value)=="false"||(value.size()==1&&value=="0")||value.size()==0)
		return false;

	return true;
}

//Convert To Double Function
double msl::to_double(const std::string& value)
{
	//Convert Variables
	std::istringstream istr(value,std::ios_base::in);
	double temp=0.0;

	//Convert
	istr>>temp;

	//Return Converted Value
	return temp;
}

//Convert To Int Function
int msl::to_int(const std::string& value)
{
	//Convert Variables
	std::istringstream istr(value,std::ios_base::in);
	int temp=0;

	//Convert
	istr>>temp;

	//Return Converted Value
	return temp;
}

//Convert To Char Function
char msl::to_char(const std::string& value)
{
	//Integers Will Work!
	return to_int(value);
}

//Convert To Unsigned Char Function
unsigned char msl::to_uchar(const std::string& value)
{
	//Integers Will Work!
	int temp=to_int(value);

	//Unsigned Conversion Factor
	while(temp<0)
		temp+=256;

	//Return Converted Value
	return temp;
}

//Starts With Function (Checks if string starts with another string)
bool msl::starts_with(const std::string& str,const std::string& start)
{
	//Check if Sizes are Comparable
	if(start.size()<=str.size())
	{
		//Check Each Character
		for(unsigned int ii=0;ii<start.size();++ii)
			if(str[ii]!=start[ii])
				return false;

		//If Match
		return true;
	}

	//Default False
	return false;
}

//Ends With Function (Checks if string ends with another string)
bool msl::ends_with(const std::string& str,const std::string& end)
{
	//Check if Sizes are Comparable
	if(end.size()<=str.size())
	{
		//Check Each Character
		for(unsigned int ii=1;ii<=end.size();++ii)
			if(str[str.size()-ii]!=end[end.size()-ii])
				return false;

		//If Match
		return true;
	}

	//Default False
	return false;
}

//To Lower Function (Returns a copy of a string that is all lower case)
std::string msl::to_lower(const std::string& str)
{
	//Create Return String
	std::string return_str=str;

	//Convert to Lower Case
	std::transform(str.begin(),str.end(),return_str.begin(),tolower);

	//Return Lower Case String
	return return_str;
}

//To Upper Function (Returns a copy of a string that is all upper case)
std::string msl::to_upper(const std::string& str)
{
	//Create Return String
	std::string return_str=str;

	//Convert to Upper Case
	std::transform(str.begin(),str.end(),return_str.begin(),toupper);

	//Return Upper Case String
	return return_str;
}

//Extract Until Function (Returns a string containing data from the start of the passed string until
//	the delimiter is reached).
std::string msl::extract_until(const std::string& str,const char until,const bool include_delimiter)
{
	//Return String Variable
	std::string return_string="";

	//Parse Passed String
	for(unsigned int ii=0;ii<str.size();++ii)
	{
		//Look for Delimiter Character
		if(str[ii]==until)
		{
			//Add Delimiter If Needed
			if(include_delimiter)
				return_string+=str[ii];

			//Break;
			break;
		}

		//Extract Data Until Delimiter
		else
		{
			return_string+=str[ii];
		}
	}

	//Return Extracted Data
	return return_string;
}

//Extract Between Function (Returns a string containing data between open and close characters.
//	Supports different and same open/close characters.)
std::string msl::extract_between(const std::string& str,const char open_char,const char close_char,
	const bool include_delimiters)
{
	//Variables for Counting Opens and Closes
	unsigned int open=0;
	unsigned int close=0;

	//Variable to Determine Whether to Extract Data or Not
	bool extract=false;

	//Return String Variable
	std::string return_string="";

	//Swap Variable Used When Open and Close Delimiters are the Same Symbol
	bool swap=false;

	//Parse Passed String
	for(unsigned int ii=0;ii<str.size();++ii)
	{
		//Look for Open Character
		if(str[ii]==open_char&&(!swap||open_char!=close_char))
		{
			//Increment Count
			++open;

			//Start Look for End (When Open and Close Delimiters are the Same Symbol)
			swap=true;
		}

		//Look for Close Character
		else if(str[ii]==close_char&&(swap||open_char!=close_char))
		{
			//Increment Count
			++close;

			//Start Look for End (When Open and Close Delimiters are the Same Symbol)
			swap=false;
		}

		//Check for Error (Reached end of passed string without finding last close character)
		if(ii==str.size()-1&&open!=close)
			return "";

		//If Extracting Data
		if(extract)
		{
			//Found Last Close, Break
			if(open==close)
				break;

			//Extract Data Between Open and Close
			else
				return_string+=str[ii];
		}

		//If Open is Found, Start Extracting Data
		if(open!=0)
			extract=true;
	}

	//Add Delimiters If Needed
	if(include_delimiters)
		return_string=open_char+return_string+close_char;

	//Return Extracted Data
	return return_string;
}