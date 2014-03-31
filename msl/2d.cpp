//2D Graphics Source
//	Created By:		Mike Moss
//	Modified On:	12/27/2013

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
int msl::start_2d(const std::string& window_title,const int view_width,const int view_height,const bool window_scale,
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

	//Set Default Font
	try
	{
		msl::set_text_font("msl/verdana.ttf");
		msl::set_text_size(12);
	}
	catch(...)
	{}

	//Start Glut
	glutMainLoop();

	//Return
	return 0;
}

//2D Stop Function
void msl::stop_2d()
{
	exit(0);
}