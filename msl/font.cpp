//2D Font Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut
//	soil

//Needs an image strip that has monospaced font in ascii order starting with ascii value ' ' (space) and ending with ascii value '~' (tilde).

//Definitions for "font.hpp"
#include "font.hpp"

//Vector Header
#include <vector>

//Font Class Constructor (Default and 1 argument version)
msl::font::font(const std::string& filename)
{
	//Default Filename
	if(filename=="")
	{
		_sprite=msl::sprite();
	}

	//Other
	else
	{
		_sprite=msl::sprite(filename,126-32+1);
	}
}

//Sprite Class Draw Function
void msl::font::draw(const double x,const double y,const std::string& text,const int h_align,
	const int v_align,const msl::color& color) const
{
	//Parsing Variables
	std::vector<std::string> sub_strings;
	std::string temp="";

	//Parse Into Sub Strings Seperated By Newlines
	for(unsigned int ii=0;ii<text.size();++ii)
	{
		//Parse Newlines
		if(text[ii]=='\n')
		{
			sub_strings.push_back(temp);
			temp="";
		}

		//End of Text Case
		else if(ii==text.size()-1)
		{
			temp+=text[ii];
			sub_strings.push_back(temp);
			temp="";
		}

		//Parse Other Characters
		else
		{
			temp+=text[ii];
		}
	}

	//Top Alignment
	double y_offset=-static_cast<double>(_sprite.height())/2.0;

	//Middle Alignment
	if(v_align==font_middle)
		y_offset+=static_cast<double>(_sprite.height())*sub_strings.size()/2.0;

	//Bottom Alignment
	if(v_align==font_bottom)
		y_offset+=static_cast<double>(_sprite.height())*sub_strings.size();

	//Go Through Sub Strings
	for(unsigned int ii=0;ii<sub_strings.size();++ii)
	{
		//Left Alignment
		double x_offset=static_cast<double>(_sprite.width())/2.0;

		//Center Alignment
		if(h_align==font_center)
			x_offset-=static_cast<double>(_sprite.width())*sub_strings[ii].size()/2.0;

		//Right Alignment
		if(h_align==font_right)
			x_offset-=static_cast<double>(_sprite.width())*sub_strings[ii].size();

		//Go Through Characters of Sub String
		for(unsigned int jj=0;jj<sub_strings[ii].size();++jj)
		{
			//Handle Unsupported Characters (Draw Spaces)
			if(sub_strings[ii][jj]<32||sub_strings[ii][jj]>126)
			{
				_sprite.draw(x+x_offset+_sprite.width()*jj,y+y_offset-_sprite.height()*ii,0,0,1,1,color,true);
			}

			//Handle Supported Characters
			else
			{
				_sprite.draw(x+x_offset+_sprite.width()*jj,y+y_offset-_sprite.height()*ii,0,sub_strings[ii][jj]-32,1,1,color,true);
			}
		}
	}
}