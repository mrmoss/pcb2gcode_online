//Webcam Header
//	Created By:		Mike Moss
//	Modified On:	10/16/2013

//Required Libraries:
//	opencv_core
//	opencv_highgui

//Begin Define Guards
#ifndef MSL_WEBCAM_H
#define MSL_WEBCAM_H

//String Header
#include <string>

//OpenCV Header
#include <opencv2/opencv.hpp>

//MSL Namespace
namespace msl
{
	//Webcam Class Declaration
	class webcam
	{
		public:
			//Boolean Operator (Tests if webcam is good)
			operator bool() const;

			//Not Operator (For boolean operator)
			bool operator!() const;

			//Good Function (Tests if webcam is good)
			bool good() const;

			//Connect Function (Connects to a webcam based on name)
			void connect(const std::string& name);

			//Connect Function (Connects to a webcam based on an operating system set index)
			void connect(const int number);

			//Close Function (Disconnects from a webcam)
			void close();

			//Update Function (Gets current frame from a webcam)
			void update();

			//Width Accessor (Returns width of webcam frames)
			unsigned int width() const;

			//Height Accessor (Returns height of webcam frames)
			unsigned int height() const;

			//Data Accessor (Returns pixel data of the current webcam frame, in BGR format)
			const void* data() const;

		private:
			//Member Variables
			cv::VideoCapture _capture;
			cv::Mat _frame;
	};
}

//End Define Guards
#endif

//Example
/*
//2D Header
#include "msl/2d.hpp"

//IO Stream Header
#include <iostream>

//Webcam Header
#include "msl/webcam.hpp"

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLUT/glew.h>
	#include <GLUT/glut.h>
#endif

//Camera Variables
msl::webcam camera;

//Video Texture Variables
unsigned int video_texture_id;

//Main
int main()
{
	//Get Camera
	camera.connect(0);

	//Camera Opened
	if(camera.good())
	{
		std::cout<<":)"<<std::endl;
	}

	//Camera Failed to Open
	else
	{
		std::cout<<":("<<std::endl;
		exit(0);
	}

	//Open 2D Window
	msl::start_2d("Webcam Test",camera.width(),camera.height());

	//Call Me Plz T_T
	return 0;
}

//Setup Function
void setup()
{
	//Generate Video Texture
	glGenTextures(1,&video_texture_id);
}

//Loop Function
void loop(const double dt)
{
	//Exit Key
	if(msl::input_check(kb_escape))
		exit(0);

	//Get Current Frame
	camera.update();

	//Generate Video Texture
	glBindTexture(GL_TEXTURE_2D,video_texture_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,camera.width(),camera.height(),0,
		GL_BGR,GL_UNSIGNED_BYTE,(GLvoid*)camera.data());
	glBindTexture(GL_TEXTURE_2D,0);
}

//Draw Function
void draw()
{
	//Draw Video Texture
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,video_texture_id);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-msl::window_width/2.0,msl::window_height/2.0);
		glTexCoord2f(1,0);
		glVertex2f(msl::window_width/2.0,msl::window_height/2.0);
		glTexCoord2f(1,1);
		glVertex2f(msl::window_width/2.0,-msl::window_height/2.0);
		glTexCoord2f(0,1);
		glVertex2f(-msl::window_width/2.0,-msl::window_height/2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
*/