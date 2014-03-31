//2D Utilities Header
//	Created By:		Mike Moss
//	Modified On:	12/27/2013

//Required Libraries:
//	ftgl
//	freetype
//	gl
//	glew
//	glu
//	glut/freeglut

//Begin Define Guards
#ifndef MSL_2D_UTIL_H
#define MSL_2D_UTIL_H

//PI Definition
#define PI 3.14159265359

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	enum halign
	{
		LEFT,
		CENTER,
		RIGHT
	};

	enum valign
	{
		TOP,
		MIDDLE,
		BOTTOM
	};

	//Color Class Declaration
	class color
	{
		public:
			//Friend Classes
			friend class sprite;

			//Constructor (Default)
			color(const float red=1,const float green=1,const float blue=1,const float alpha=1);

			//Member Variables
			float r;
			float g;
			float b;
			float a;
	};

	//Point Direction Function (Returns direction between two points in degrees)
	double point_direction(const double x1,const double y1,const double x2,const double y2);

	//Point Distance Function (Returns distance between two points)
	double point_distance(const double x1,const double y1,const double x2,const double y2);

	//Basic Shape Drawing Functions
	void draw_point(const double x,const double y,const msl::color& color=msl::color(1,1,1,1));
	void draw_line(const double x1,const double y1,const double x2,const double y2,const msl::color& color=msl::color(1,1,1,1));
	void draw_triangle(const double x1,const double y1,const double x2,const double y2,const double x3,const double y3,
		const bool fill,const msl::color& color=msl::color(1,1,1,1));
	void draw_rectangle(const double x,const double y,const double width,const double height,const bool fill,const msl::color& color=msl::color(1,1,1,1));
	void draw_rectangle_center(const double x,const double y,const double width,const double height,const bool fill,const msl::color& color=msl::color(1,1,1,1));
	void draw_rectangle_gradient(const double x,const double y,const double width,const double height,const bool fill,
		const msl::color& color_top_left=msl::color(1,1,1,1),
		const msl::color& color_top_right=msl::color(1,1,1,1),
		const msl::color& color_bottom_right=msl::color(1,1,1,1),
		const msl::color& color_bottom_left=msl::color(1,1,1,1));
	void draw_rectangle_center_gradient(const double x,const double y,const double width,const double height,const bool fill,
		const msl::color& color_top_left=msl::color(1,1,1,1),
		const msl::color& color_top_right=msl::color(1,1,1,1),
		const msl::color& color_bottom_right=msl::color(1,1,1,1),
		const msl::color& color_bottom_left=msl::color(1,1,1,1));
	void draw_circle(const double x,const double y,const double radius,const msl::color& color=msl::color(1,1,1,1));

	//Text Set Font Function (Loads TrueType style fonts)
	void set_text_font(const std::string& font);

	//Text Set Size Function (In standard font sizes)
	void set_text_size(const double size);

	//Text Width Function (Returns width of text in pixels)
	double text_width(const std::string& text);

	//Text Height Function (Returns height of text in pixels)
	double text_height(const std::string& text);

	//Text Drawing Function
	void draw_text(const double x,const double y,const std::string& text,const msl::halign horizontal_alignment=LEFT,
		const msl::valign vertical_alignment=BOTTOM,const msl::color& color=msl::color(1,1,1,1));
}

//End Define Guards
#endif

//Example
/*
#include "2d_util.hpp"
#include <iostream>
#include "sprite.hpp"
using namespace msl;

int main()
{
	double ax=10;
	double ay=0;
	double bx=10;
	double by=20;

	std::cout<<"Direction between points a and b is:\t"<<point_direction(ax,ay,bx,by)<<std::endl;
	std::cout<<"Distance between points a and b is:\t"<<point_distance(ax,ay,bx,by)<<std::endl;

	return 0;
}
*/