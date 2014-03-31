//2D Utilities Source
//	Created By:		Mike Moss
//	Modified On:	12/27/2013

//Required Libraries:
//	ftgl
//	freetype
//	gl
//	glew
//	glu
//	glut/freeglut

//Definitions for "2d_util.hpp"
#include "2d_util.hpp"

//Algorithm Header
#include <algorithm>

//Exception Header
#include <stdexcept>

//FTGL Header
#include <FTGL/ftgl.h>

//Math Header
#include <math.h>

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

//String Utility Header
#include "string_util.hpp"

//Vector Header
#include <vector>

//Static Font Object
static FTGLPixmapFont* msl_text_font=new FTGLPixmapFont("");

//Color Class Constructor (Default)
msl::color::color(const float red,const float green,const float blue,const float alpha):r(red),g(green),b(blue),a(alpha)
{}

//Point Direction Function (Returns direction between two points in degrees)
double msl::point_direction(const double x1,const double y1,const double x2,const double y2)
{
	return atan2(y2-y1,x2-x1)*180.0/M_PI;
}

//Point Distance Function (Returns distance between two points)
double msl::point_distance(const double x1,const double y1,const double x2,const double y2)
{
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

//Draw Point Function
void msl::draw_point(const double x,const double y,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Point
	glBegin(GL_POINTS);
		glVertex2d(x,y);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Line Function
void msl::draw_line(const double x1,const double y1,const double x2,const double y2,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Line
	glBegin(GL_LINES);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Triangle Function
void msl::draw_triangle(const double x1,const double y1,const double x2,const double y2,const double x3,
	const double y3,const bool fill,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Triangle
	if(fill)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_LINE_LOOP);

		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
		glVertex2d(x3,y3);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Rectangle Function
void msl::draw_rectangle(const double x,const double y,const double width,const double height,const bool fill,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Rectangle
	if(fill)
		glBegin(GL_QUADS);
    else
		glBegin(GL_LINE_LOOP);

		glVertex2d(x,y);
		glVertex2d(x+width,y);
		glVertex2d(x+width,y-height);
		glVertex2d(x,y-height);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Rectangle Center Function
void msl::draw_rectangle_center(const double x,const double y,const double width,const double height,const bool fill,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Rectangle
	if(fill)
		glBegin(GL_QUADS);
    else
		glBegin(GL_LINE_LOOP);

		glVertex2d(x-width/2.0,y+height/2.0);
		glVertex2d(x+width/2.0,y+height/2.0);
		glVertex2d(x+width/2.0,y-height/2.0);
		glVertex2d(x-width/2.0,y-height/2.0);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Rectangle Gradient Function
void msl::draw_rectangle_gradient(const double x,const double y,const double width,const double height,const bool fill,
	const msl::color& color_top_left,const msl::color& color_top_right,const msl::color& color_bottom_right,
	const msl::color& color_bottom_left)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Draw Rectangle
	if(fill)
		glBegin(GL_QUADS);
    else
		glBegin(GL_LINE_LOOP);

		glColor4d(color_top_left.r,color_top_left.g,color_top_left.b,color_top_left.a);
		glVertex2d(x,y);
		glColor4d(color_top_right.r,color_top_right.g,color_top_right.b,color_top_right.a);
		glVertex2d(x+width,y);
		glColor4d(color_bottom_right.r,color_bottom_right.g,color_bottom_right.b,color_bottom_right.a);
		glVertex2d(x+width,y-height);
		glColor4d(color_bottom_left.r,color_bottom_left.g,color_bottom_left.b,color_bottom_left.a);
		glVertex2d(x,y-height);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Rectangle Center Gradient Function
void msl::draw_rectangle_center_gradient(const double x,const double y,const double width,const double height,const bool fill,
	const msl::color& color_top_left,const msl::color& color_top_right,const msl::color& color_bottom_right,
	const msl::color& color_bottom_left)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Draw Rectangle
	if(fill)
		glBegin(GL_QUADS);
    else
		glBegin(GL_LINE_LOOP);

		glColor4d(color_top_left.r,color_top_left.g,color_top_left.b,color_top_left.a);
		glVertex2d(x-width/2.0,y+height/2.0);
		glColor4d(color_top_right.r,color_top_right.g,color_top_right.b,color_top_right.a);
		glVertex2d(x+width/2.0,y+height/2.0);
		glColor4d(color_bottom_right.r,color_bottom_right.g,color_bottom_right.b,color_bottom_right.a);
		glVertex2d(x+width/2.0,y-height/2.0);
		glColor4d(color_bottom_left.r,color_bottom_left.g,color_bottom_left.b,color_bottom_left.a);
		glVertex2d(x-width/2.0,y-height/2.0);
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Draw Circle Function
void msl::draw_circle(const double x,const double y,const double radius,const msl::color& color)
{
	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Disable Culling
	glDisable(GL_CULL_FACE);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Draw Circle (Triangle Strip)
	glBegin(GL_TRIANGLE_STRIP);

		//Determine "Wedge" Variables
		int segments=std::max(10,(int)radius*2);
		double angle=2.0*M_PI/(double)segments;

		//Draw All But Last "Wedge"
		for(int ii=0;ii<segments;++ii)
		{
			glVertex2d(x+cos(angle*ii)*radius,y+sin(angle*ii)*radius);
			glVertex2d(x,y);
		}

	//Draw Last "Wedge"
	glVertex2d(x+radius,y);

	//Done With Circle
	glEnd();

	//Disable Transparency
	glDisable(GL_BLEND);
}

//Text Set Font Function (Loads TrueType style fonts)
void msl::set_text_font(const std::string& font)
{
	FTGLPixmapFont* temp_font=new FTGLPixmapFont(font.c_str());

	if(temp_font->Error())
		throw std::runtime_error("msl::set_text_font() - Font not found!");

	delete msl_text_font;
	msl_text_font=temp_font;

}

//Text Set Size Function (In standard font sizes)
void msl::set_text_size(const double size)
{
	if(msl_text_font->Error())
		throw std::runtime_error("msl::set_text_size() - Font not found!");

	msl_text_font->FaceSize(size);
}

//Text Width Function (Returns height of text in pixels)
double msl::text_width(const std::string& str)
{
	if(msl_text_font->Error())
		throw std::runtime_error("msl::text_width() - Font not found!");

	FTBBox box=msl_text_font->BBox((str+"B").c_str());
	FTBBox sub=msl_text_font->BBox("B");

	double box_width=box.Upper().X()-box.Lower().X();
	double sub_width=sub.Upper().X()-sub.Lower().X();

	return box_width-sub_width;
}

//Text Height Function (Returns height of text in pixels)
double msl::text_height(const std::string& str)
{
	if(msl_text_font->Error())
		throw std::runtime_error("msl::text_height() - Font not found!");

	FTBBox box=msl_text_font->BBox("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890`~!@#$%^&*()_+\"\\|]}[{\':;.>,</?");

	int lines=1;

	for(unsigned int ii=0;ii<str.size();++ii)
		if(str[ii]=='\n')
			++lines;

	return (box.Upper().Y()-box.Lower().Y())*lines;
}

//Text Drawing Function
void msl::draw_text(const double x,const double y,const std::string& str,const msl::halign horizontal_alignment,
	const msl::valign vertical_alignment,const msl::color& col)
{
	if(msl_text_font->Error())
		throw std::runtime_error("msl::draw_text() - Font not found!");

	double text_position_x=glutGet(GLUT_WINDOW_WIDTH)/2.0+x;
	double text_position_y=glutGet(GLUT_WINDOW_HEIGHT)/2.0+y;

	if(horizontal_alignment==CENTER)
		text_position_x-=msl::text_width(str)/2.0;
	if(horizontal_alignment==RIGHT)
		text_position_x-=msl::text_width(str);

	if(vertical_alignment==MIDDLE)
		text_position_y-=msl::text_height(str)/2.0-msl::text_height(str)/6.0;
	else if(vertical_alignment==TOP)
		text_position_y-=msl::text_height(str);

	glPixelTransferf(GL_RED_BIAS,col.r-1);
	glPixelTransferf(GL_GREEN_BIAS,col.g-1);
	glPixelTransferf(GL_BLUE_BIAS,col.b-1);
	msl_text_font->Render(str.c_str(),-1,FTPoint(text_position_x,text_position_y));
}