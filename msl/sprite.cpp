//2D Sprite Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut
//	soil

//Definitions for "sprite.hpp"
#include "sprite.hpp"

//Exception Header
#include <stdexcept>

//Math Header
#include <math.h>

//Soil Header
#include <SOIL/SOIL.h>

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

//Texture Load Function
static unsigned int load_texture(const std::string& filename,unsigned int& width,unsigned int& height)
{
	//Check for Bad Window
	if(glutGetWindow()==0)
		throw std::runtime_error("msl::load_texture - opengl not loaded, are you loading the texture in the correct place?");

	//Load Texture
	unsigned int return_texture=SOIL_load_OGL_texture(filename.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS|SOIL_FLAG_DDS_LOAD_DIRECT);

	//Load Texture Data
	unsigned char* texture_data=SOIL_load_image(filename.c_str(),reinterpret_cast<int*>(&width),reinterpret_cast<int*>(&height),NULL,SOIL_LOAD_AUTO);

	//Check for Bad Filename
	if(return_texture==0||texture_data==NULL)
		throw std::runtime_error("msl::load_texture - bad filename, is the filename correct?");

	//Clean Up
	delete[] texture_data;

	//Return Good Texture
	return return_texture;
}

//Sprite Class Constructor (Default, Raw OpenGL Texture)
msl::sprite::sprite(const unsigned int texture,const unsigned int number_of_frames)
	:_texture(texture),_number_of_frames(number_of_frames),_origin_x(0.0),_origin_y(0.0)
{
	//Get Texture Size
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_texture);
	glDisable(GL_TEXTURE_2D);

	//Check Number of Frames
	if(_number_of_frames==0)
		_number_of_frames=1;
}

//Sprite Class Constructor (String Filename)
msl::sprite::sprite(const std::string& filename,const unsigned int number_of_frames)
	:_texture(0),_number_of_frames(number_of_frames),_origin_x(0.0),_origin_y(0.0)
{
	//Assign Members
	unsigned int width=0;
	unsigned int height=0;
	*this=msl::sprite(load_texture(filename,width,height),number_of_frames);
	_width=width;
	_height=height;
}
//Release Texture Function (Releases OpenGL Memory)
void msl::sprite::release()
{
	if(_texture!=0)
		glDeleteTextures(1,&_texture);
}

//Sprite Class Number of Frames Accessor
unsigned int msl::sprite::number_of_frames() const
{
	return _number_of_frames;
}

//Sprite Class Width Accessor
unsigned int msl::sprite::width() const
{
	return _width/_number_of_frames;
}

//Sprite Class Height Member Accessor
unsigned int msl::sprite::height() const
{
	return _height;
}

//Sprite Class Set Origin Mutator
void msl::sprite::set_origin(const double x,const double y)
{
	_origin_x=x;
	_origin_y=y;
}

//Sprite Class Draw Function
void msl::sprite::draw(const double x,const double y,const double rotation,const unsigned int frame,
	const double scale_x,const double scale_y,
	const msl::color& color,const bool smooth) const
{
	//Bound Passed Frame
	unsigned int frame_bounded=std::min(_number_of_frames,frame);

	//Determine Frame Drawing Variables
	double frame_width_halfed=((_width/_number_of_frames)*scale_x)/2.0;
	double frame_height_halfed=(_height*scale_y)/2.0;
	double frame_to_draw_begin=(1.0/_number_of_frames)*frame_bounded;
	double frame_to_draw_end=(1.0/_number_of_frames)*(frame_bounded+1);

	//Enable Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//Enable Texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_texture);

	//Determine Texture Filter(Pixelated/Smooth)
	int filter=GL_NEAREST;

	if(smooth)
		filter=GL_LINEAR;

	//Set Texture Filters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filter);

	//Limit Rotation
	double rotation_limited=rotation;

	while(rotation_limited>=360)
		rotation_limited-=360;

	while(rotation_limited<=0)
		rotation_limited+=360;

	//Draw Image Frame with Rotation and Alpha Value (0.001 and 0.999 are to Remove Bad Borders)
	glPushMatrix();
		glTranslated(x,y,0);
		glRotated(rotation_limited+180,0,0,1);
		glTranslated(_origin_x,_origin_y,0);
		glBegin(GL_QUADS);
			glColor4d(color.r,color.g,color.b,color.a);
			glTexCoord2d(frame_to_draw_begin+0.001,0.999);
			glVertex2d(frame_width_halfed,frame_height_halfed);
			glTexCoord2d(frame_to_draw_end-0.001,0.999);
			glVertex2d(-frame_width_halfed,frame_height_halfed);
			glTexCoord2d(frame_to_draw_end-0.001,0.001);
			glVertex2d(-frame_width_halfed,-frame_height_halfed);
			glTexCoord2d(frame_to_draw_begin+0.001,0.001);
			glVertex2d(frame_width_halfed,-frame_height_halfed);
			glColor4f(1,1,1,1);
		glEnd();
	glPopMatrix();

	//Disable Texture
	glDisable(GL_TEXTURE_2D);

	//Disable Transparency
	glDisable(GL_BLEND);
}