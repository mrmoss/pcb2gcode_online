//JSON Header
//	Created By:		Mike Moss
//	Modified On:	11/07/2013

//Begin Define Guards
#ifndef MSL_JSON_H
#define MSL_JSON_H

//Map Header
#include <map>

//OStream Header
#include <ostream>

//String Header
#include <string>

//String Utility Header
#include "string_util.hpp"

//MSL Namespace
namespace msl
{
	//JSON Class Declaration
	class json
	{
		public:
			//Constructor (Default, if error is found json contains only only entry, "error".
			//	Error is an object containing "what", the error message, and "position",
			//	the position of the error in the passed string.)
			json(const std::string& json_string="");

			//Size Accessor (Returns number of variables)
			unsigned int size() const;

			//Set Operator (Sets a variable to a value)
			template<typename T> void set(const std::string& lhs,const T& rhs);

			//Set Operator (Sets a variable to a value) (JSON Version)
			void set(const std::string& lhs,const json& rhs);

			//Get Operator (Returns variable from an index)
			std::string get(const unsigned int index);

			//Get Operator (Returns the value of a variable)
			std::string get(const std::string& index);

			//String Function (Returns the JSON string)
			std::string str() const;

		private:
			//Member Variables
			std::map<std::string,std::string> _data;
	};
}

//Set Operator (Sets a variable to a value)
template<typename T> void msl::json::set(const std::string& lhs,const T& rhs)
{
	_data[lhs]=msl::to_string(rhs);
}

//End Define Guards
#endif

//Example
/*
//IO Stream Header
#include <iostream>

//JSON Header
#include "msl/json.hpp"

int main()
{
	//Create a JSON Object
	msl::json oo("{\"test1\":\"123\",\"test2\":\"1.456\",\"test3\":{\"test4\":{\"abc\":\"678\"}},\"test5\":\"lol kk\"}");

	//Print Out JSON Object
	std::cout<<oo<<std::endl;

	//Print Out JSON Values
	std::cout<<"\ttest1="<<oo.get("test1")<<std::endl;
	std::cout<<"\ttest2="<<oo.get("test2")<<std::endl;
	std::cout<<"\ttest3="<<oo.get("test3")<<std::endl;
	std::cout<<"\t\ttest4="<<msl::json(oo.get("test3")).get("test4")<<std::endl;
	std::cout<<"\t\t\tabc="<<msl::json(msl::json(oo.get("test3")).get("test4")).get("abc")<<std::endl;
	std::cout<<"\ttest5="<<oo.get("test5")<<std::endl;

	//All Done
	return 0;
}
*/