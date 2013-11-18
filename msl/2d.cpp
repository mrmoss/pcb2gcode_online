//2D Graphics Source
//	Created By:		Mike Moss
//	Modified On:	05/19/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut
//	soil

//Definitions for "2d.hpp"
#include "2d.hpp"

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

//Exception Header
#include <stdexcept>

//Math Header
#include <math.h>

//Global Variables
double msl::view_width=640;
double msl::view_height=480;
double msl::window_width=msl::view_width;
double msl::window_height=msl::view_height;
static double dt_start;
static int glut_window;

//Glut Reshape Scale Callback
static void reshape_scale(int width,int height)
{
	//Set Viewport Size
	glViewport(0,0,width,height);

	//Goto Projection View
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Get Window Positions
	msl::view_width=glutGet(GLUT_INIT_WINDOW_WIDTH);
	msl::view_height=glutGet(GLUT_INIT_WINDOW_HEIGHT);
	msl::window_width=glutGet(GLUT_WINDOW_WIDTH);
	msl::window_height=glutGet(GLUT_WINDOW_HEIGHT);

	//If Width Dependent
	if(width<=height)
	{
		double scaler=static_cast<double>(glutGet(GLUT_WINDOW_HEIGHT))/static_cast<double>(glutGet(GLUT_WINDOW_WIDTH));
		msl::view_height=glutGet(GLUT_INIT_WINDOW_WIDTH)*scaler;
	}

	//If Height Dependent
	else
	{
		double scaler=static_cast<double>(glutGet(GLUT_WINDOW_WIDTH))/static_cast<double>(glutGet(GLUT_WINDOW_HEIGHT));
		msl::view_width=glutGet(GLUT_INIT_WINDOW_HEIGHT)*scaler;
	}

	//Set View
	glOrtho(-msl::view_width/2,msl::view_width/2,-msl::view_height/2,msl::view_height/2,0,1);

	//Return to Model View
	glMatrixMode(GL_MODELVIEW);
}

//Glut Reshape Callback
static void reshape(int width,int height)
{

	//Set Viewport
	glViewport(0,0,width,height);

	//Goto Projection View
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Get Window Positions
	msl::window_width=glutGet(GLUT_WINDOW_WIDTH);
	msl::window_height=glutGet(GLUT_WINDOW_HEIGHT);

	//Set View
	glOrtho(-width/2,width/2,-height/2,height/2,0,1);

	//Return to Model View
	glMatrixMode(GL_MODELVIEW);
}

//Glut Idle Callback
static void idle()
{
	//Set Window
	if(glutGetWindow()!=glut_window)
		glutSetWindow(glut_window);

	//Get Time
	double dt_end=glutGet(GLUT_ELAPSED_TIME);
	double dt=dt_end-dt_start;

	//Set Time
	dt_start=dt_end;

	//Loop
	loop(dt/1000.0);

	//Reset Pressed and Released Inputs
	msl::input_reset();

	//Display
	glutPostRedisplay();
}

//Glut Display Callback
static void display()
{
	//Pre-Display (Clearing and Such)
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
	glLoadIdentity();

	//Draw
	draw();

	//Double Buffering
	glutSwapBuffers();
}

//2D Start Function
void msl::start_2d(const std::string& window_title,const int view_width,const int view_height,const bool window_scale,
	const msl::color& color,int argc,char** argv)
{
	//Set Window Parameters
	msl::view_width=view_width;
	msl::view_height=view_height;

	//Initialize Glut
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(msl::view_width,msl::view_height);

	//Create Glut Window
	glut_window=glutCreateWindow(window_title.c_str());

	//Check Glew
	if(glewInit())
		throw std::runtime_error("msl::start_2d - could not initialize glew!");

	//Setup Function
	setup();

	//Glut Settings
	glClearColor(color.r,color.g,color.b,color.a);
	glDisable(GL_DEPTH_TEST);

	//Glut Callbacks
	msl::input_setup(window_scale);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	//Scale Reshape Function
	if(window_scale)
		glutReshapeFunc(reshape_scale);

	//Resize Reshape Function
	else
		glutReshapeFunc(reshape);

	//Get Time
	dt_start=glutGet(GLUT_ELAPSED_TIME);

	//Start Glut
	glutMainLoop();
}

//2D Stop Function
void msl::stop_2d()
{
	exit(0);
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

		glVertex2d(x-width/2.0,y+height/2.0);
		glVertex2d(x+width/2.0,y+height/2.0);
		glVertex2d(x+width/2.0,y-height/2.0);
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

//Text Drawing Function
void msl::draw_text(const double x,const double y,const std::string& text,const msl::color& color)
{
	//Disable Texture
	glDisable(GL_TEXTURE_2D);

	//Go To Project Mode
	glMatrixMode(GL_PROJECTION);

	//Save Current Matrix
	glPushMatrix();

	//Clear New Matrix
	glLoadIdentity();

	//Setup Drawing View
	glOrtho(-glutGet(GLUT_WINDOW_WIDTH)/2,glutGet(GLUT_WINDOW_WIDTH)/2,-glutGet(GLUT_WINDOW_HEIGHT)/2,glutGet(GLUT_WINDOW_HEIGHT)/2,0,1);

	//Set Color
	glColor4d(color.r,color.g,color.b,color.a);

	//Vertical Offset Variable
	double offset_increment=13;
	double offset=offset_increment;

	//Move Text Drawing Position
	glRasterPos2d(x+1,y-offset+1);

	//Draw String
	for(unsigned int ii=0;ii<text.size();++ii)
	{
		//Newlines
		if(text[ii]=='\n')
		{
			offset+=offset_increment;
			glRasterPos2d(x+1,y-offset+1);
		}

		//Tabs
		else if(text[ii]=='\t')
		{
			//Get Current Line Width
			unsigned int line_width=0;

			for(int jj=ii-1;jj>=0&&text[jj]!='\t'&&text[jj]!='\n';--jj)
				++line_width;

			//Add Indents
			for(unsigned int jj=line_width%4;jj<4;++jj)
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13,' ');
		}

		//Characters
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13,text[ii]);
		}
	}

	//Reset Color
	glColor4d(1,1,1,1);

	//Load Old Matrix
	glPopMatrix();

	//Return To Matrix Mode
	glMatrixMode(GL_MODELVIEW);
}
