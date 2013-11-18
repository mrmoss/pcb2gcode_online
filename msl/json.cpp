//JSON Source
//	Created By:		Mike Moss
//	Modified On:	11/07/2013

//Definitions for "json.hpp"
#include "json.hpp"

//Stream Out Operator
std::ostream& operator<<(std::ostream& lhs,const msl::json& rhs)
{
	return (lhs<<rhs.str());
}

//Constructor (Default, if error is found json contains only only entry, "error".
//	Error is an object containing "what", the error message, and "position",
//	the position of the error in the passed string.)
msl::json::json(const std::string& json_string)
{
	//Error Variables
	bool error=false;
	unsigned int error_position=1;
	std::string error_what="";

	//Get Data Between Brackets
	std::string temp=msl::extract_between(json_string,'{','}',false);

	//Parse Data
	while(temp.size()>0)
	{
		//Remove Whitespace
		while(temp.size()>0&&isspace(temp[0]))
		{
			temp.erase(0,1);
			++error_position;
		}

		//Variable Name Variable
		std::string var;

		//Get Variable Name (With "")
		if(temp[0]=='\"')
		{
			var=msl::extract_between(temp,'\"','\"',true);
			temp.erase(0,var.size());
			var=msl::extract_between(var,'\"','\"',false);
		}

		//Get Variable Name (With '')
		else if(temp[0]=='\'')
		{
			var=msl::extract_between(temp,'\'','\'',true);
			temp.erase(0,var.size());
			var=msl::extract_between(var,'\'','\'',false);
		}

		//Update Error Postion
		++error_position;

		//Bad Variable Name
		if(var=="")
		{
			error_what="bad variable name";
			error=true;
			break;
		}

		//Update Error Position
		error_position+=var.size();
		++error_position;

		//Remove Whitespace
		while(temp.size()>0&&isspace(temp[0]))
		{
			temp.erase(0,1);
			++error_position;
		}

		//Remove Colon
		if(temp.size()>0&&temp[0]==':')
		{
			temp.erase(0,1);
			++error_position;
		}

		//No Colon, Error
		else
		{
			error_what="colon expected";
			error=true;
			break;
		}

		//Remove Whitespace
		while(temp.size()>0&&isspace(temp[0]))
		{
			temp.erase(0,1);
			++error_position;
		}

		//Variable Value Variable
		std::string val="";

		//Check Size
		if(temp.size()>0)
		{
			//Extract Object
			if(temp[0]=='{')
			{
				//Get Object
				val=msl::extract_between(temp,'{','}',true);
				temp.erase(0,val.size());
				error_position+=val.size();
			}

			//Extract String ("" || '')
			else if(temp[0]=='\"'||temp[0]=='\'')
			{
				//Get String
				val=msl::extract_until(temp,',',false);

				//Check for Commas in Strings
				while(true)
				{
					//Temp Copy of Val
					std::string check_for_comma_in_string=val;

					//Remove Whitespace At End of String
					while(check_for_comma_in_string.size()>0&&std::isspace(val[check_for_comma_in_string.size()-1]))
						check_for_comma_in_string.erase(check_for_comma_in_string.size()-1,1);

					//If Comma Found
					if(check_for_comma_in_string.size()>0&&check_for_comma_in_string[check_for_comma_in_string.size()-1]!=temp[0])
					{
						//Add Comma
						val+=',';

						//Get Rest of String
						std::string val_add=msl::extract_until(temp.substr(val.size(),temp.size()-val.size()),',',false);

						//Nothing Found, Break
						if(val_add.size()==0)
							break;

						//Add Rest of Value to Value String
						val+=val_add;
					}

					//If No Comma, Break
					else
					{
						break;
					}
				}

				//Remove Extracted Data
				temp.erase(0,val.size());

				//Update Error Position
				error_position+=val.size();

				//Remove Whitespace At End of String
				while(val.size()>0&&std::isspace(val[val.size()-1]))
					val.erase(val.size()-1,1);

				//Valid String, Remove Quotes
				if((val.size()>=2&&val[0]=='\"'&&val[val.size()-1]=='\"')||(val.size()>=2&&val[0]=='\''&&val[val.size()-1]=='\''))
				{
					val=val.substr(1,val.size()-2);
				}
				//Bad String
				else
				{
					error_what="invalid string";
					error=true;
					break;
				}
			}

			//Extract Boolean
			else if(msl::starts_with(msl::to_lower(temp),"true")||msl::starts_with(msl::to_lower(temp),"false"))
			{
				//Get Boolean
				val=msl::to_lower(msl::extract_until(temp,',',false));
				temp.erase(0,val.size());

				//Keep Old Val Size, For Error Position Later On
				unsigned int old_val_size=val.size();

				//Remove Whitespace At End of Boolean
				while(val.size()>0&&std::isspace(val[val.size()-1]))
					val.erase(val.size()-1,1);

				//Check for Good Boolean
				if(val=="true"||val=="false")
				{
					//Update Error Position
					error_position+=old_val_size-val.size();
				}

				//Error
				else
				{
					error_what="invalid variable value";
					error=true;
				}
			}

			//Extract Number
			else
			{
				//Get Number
				val=msl::extract_until(temp,',',false);
				temp.erase(0,val.size());

				//Keep Old Val Size, For Error Position Later On
				unsigned int old_val_size=val.size();

				//Remove Whitespace At End of Number
				while(val.size()>0&&std::isspace(val[val.size()-1]))
					val.erase(val.size()-1,1);

				//Period Finder
				bool found_period=false;

				//Check for Valid Number
				for(unsigned int ii=0;ii<val.size();++ii)
				{
					//Non-Number or Period or Mutiple Periods
					if((!std::isdigit(val[ii])&&val[ii]!='.'&&val[ii]!='-')||
						(val[ii]=='.'&&found_period)||((val[ii]=='-'&&ii!=0)))
					{
						error_what="invalid variable value";
						error=true;
						break;
					}

					//Look for Multiple Periods
					if(val[ii]=='.')
						found_period=true;

					//Update Error Position
					++error_position;
				}

				//Update Error Position
				error_position+=old_val_size-val.size();
			}

			//Remove Whitespace
			while(temp.size()>0&&isspace(temp[0]))
			{
				temp.erase(0,1);
				++error_position;
			}

			//Remove Comma
			if(temp.size()>0&&temp[0]==',')
			{
				temp.erase(0,1);
				++error_position;
			}

			//No Comma, Error
			else if(temp.size()>0)
			{
				error_what="comma expected";
				error=true;
				break;
			}
		}

		//On Error
		else
		{
			error_what="unexpected end of object";
			error=true;
		}

		//Add Variable
		set(var,val);
	}

	//On Errors
	if(error)
	{
		//Clear Data
		_data.clear();

		//Create and Set Error Object
		msl::json error_object;
		error_object.set("what",error_what);
		error_object.set("position",error_position);
		set("error",error_object);
	}
}

//Size Accessor (Returns number of variables)
unsigned int msl::json::size() const
{
	return _data.size();
}

//Set Operator (Sets a variable to a value) (JSON Version)
void msl::json::set(const std::string& lhs,const json& rhs)
{
	set(lhs,rhs.str());
}

//Get Operator (Returns variable from an index)
std::string msl::json::get(const unsigned int index)
{
	//Create a Count Variable (Damn iterators...)
	unsigned int count=0;

	//Iterate Through Variables
	for(std::map<std::string,std::string>::const_iterator ii=_data.begin();ii!=_data.end();++ii)
	{
		//Found Variable, Return Value
		if(count==index)
			return ii->first;

		//Increment Counter
		++count;
	}

	//Return Empty Value When Not Found
	return "";
}

//Get Operator (Returns the value of a variable)
std::string msl::json::get(const std::string& index)
{
	return _data[index];
}

//String Function (Returns the JSON string)
std::string msl::json::str() const
{
	//Add Initial Bracket
	std::string json_string="{";

	//Go Through Variables
	for(std::map<std::string,std::string>::const_iterator ii=_data.begin();ii!=_data.end();++ii)
	{
		//Add Variable to String
		json_string+="\""+ii->first+"\":";

		//Add Value to String
		if(ii->second.size()>0&&ii->second[0]=='{')
			json_string+=ii->second;
		else
			json_string+="\""+ii->second+"\"";

		//Get Next Variable
		std::map<std::string,std::string>::const_iterator next=ii;
		++next;

		//If Next is Not Last Add Comma
		if(next!=_data.end())
			json_string+=",";
	}

	//Add Final Bracket
	json_string+="}";

	//Return JSON String
	return json_string;
}