//2D Graphics Header
//	Created By:		Mike Moss
//	Modified On:	05/19/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut
//	soil

//Begin Define Guards
#ifndef MSL_2D_H
#define MSL_2D_H

//2D Utilities Header
#include "2d_util.hpp"

//C Standard Library Header
#include <cstdlib>

//Glut Input Header
#include "glut_input.hpp"

//Font Header
#include "font.hpp"

//Sprite Header
#include "sprite.hpp"

//String Header
#include <string>

//String Utility Header
#include "string_util.hpp"

//Externs
extern void setup();
extern void loop(const double dt);
extern void draw();

//MSL Namespace
namespace msl
{
	//Input Variables
	extern double view_width;
	extern double view_height;
	extern double window_width;
	extern double window_height;

	//2D Start Function
	void start_2d(const std::string& window_title="",const int window_width=640,const int window_height=480,const bool window_scale=true,
		const msl::color& color=msl::color(0.5,0.5,0.5,1),int argc=0,char** argv=NULL);

	//2D Stop Function
	void stop_2d();

	//Basic Shape Drawing Functions
	void draw_point(const double x,const double y,const msl::color& color=msl::color(1,1,1,1));
	void draw_line(const double x1,const double y1,const double x2,const double y2,const msl::color& color=msl::color(1,1,1,1));
	void draw_triangle(const double x1,const double y1,const double x2,const double y2,const double x3,const double y3,
		const bool fill,const msl::color& color=msl::color(1,1,1,1));
	void draw_rectangle(const double x,const double y,const double width,const double height,const bool fill,const msl::color& color=msl::color(1,1,1,1));
	void draw_circle(const double x,const double y,const double radius,const msl::color& color=msl::color(1,1,1,1));

	//Text Drawing Function
	void draw_text(const double x,const double y,const std::string& text,const msl::color& color=msl::color(1,1,1,1));
}

//End Define Guards
#endif

//Example
/*
//2D Header
#include "2d.hpp"

int main()
{
	//Create a Window (640x480 scaled with a white background)
	start_2d("2D Example",640,480,true,msl::color(1,1,1));
	return 0;
}

void setup()
{
}

void loop(const double dt)
{
	//Escape Button Exits
	if(msl::input_check_pressed(kb_escape))
		exit(0);
}

void draw()
{
	//Draw Circle (at mouse cursor with radius 32 and a red color)
	msl::draw_circle(msl::mouse_x,msl::mouse_y,32,msl::color(1,0,0));
}
*/
