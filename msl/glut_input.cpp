//Glut Input Source
//	Created By:		Mike Moss
//	Modified On:	11/23/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut

//Definitions for "glut_input.hpp"
#include "glut_input.hpp"

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

//Input Variables
double msl::mouse_x;
double msl::mouse_y;
static bool keyboard_2d[256];
static bool keyboard_pressed_2d[256];
static bool keyboard_released_2d[256];
static bool special_2d[256];
static bool special_pressed_2d[256];
static bool special_released_2d[256];
static bool mouse_down_2d[3];
static bool mouse_pressed_2d[3];
static bool mouse_released_2d[3];
static int shifted_keys[256];

//Glut Keyboard Down Callback
static void keyboard_down_2d(unsigned char key,int x,int y)
{
	//Pressed
	if(!keyboard_2d[key])
		keyboard_pressed_2d[key]=true;

	//Down
	keyboard_2d[key]=true;
}

//Glut Keyboard Up Callback
static void keyboard_up_2d(unsigned char key,int x,int y)
{
	//Up
	keyboard_2d[key]=false;

	//Released
	keyboard_released_2d[key]=true;

	//Incase of Shifts
	if(shifted_keys[key]!=-1)
	{
		//Up
		keyboard_2d[shifted_keys[key]]=false;

		//Released
		keyboard_released_2d[shifted_keys[key]]=true;
	}
}

//Glut Special Down Callback
static void special_down_2d(int key,int x,int y)
{
	//Pressed
	if(!special_2d[key])
		special_pressed_2d[key]=true;

	//Down
	special_2d[key]=true;
}

//Glut Special Up Callback
static void special_up_2d(int key,int x,int y)
{
	//Up
	special_2d[key]=false;

	//Released
	special_released_2d[key]=true;
}

//Glut Mouse Callback
static void mouse_2d(int button,int state,int x,int y)
{
	//Pressed
	if(!mouse_down_2d[button]&&!state)
		mouse_pressed_2d[button]=true;

	//Down/Up
	mouse_down_2d[button]=!state;

	//Released
	mouse_released_2d[button]=state;
}

//Glut Mouse Motion Callback
static void mouse_motion_2d_scaled(int x,int y)
{
	//Get Original Dimensions
	double window_width=glutGet(GLUT_INIT_WINDOW_WIDTH);
	double window_height=glutGet(GLUT_INIT_WINDOW_HEIGHT);

	//If Width Dependent
	if(glutGet(GLUT_WINDOW_WIDTH)<=glutGet(GLUT_WINDOW_HEIGHT))
	{
		double scaler=(double)glutGet(GLUT_WINDOW_HEIGHT)/(double)glutGet(GLUT_WINDOW_WIDTH);
		window_height=glutGet(GLUT_INIT_WINDOW_WIDTH)*scaler;
	}

	//If Height Dependent
	else
	{
		double scaler=(double)glutGet(GLUT_WINDOW_WIDTH)/(double)glutGet(GLUT_WINDOW_HEIGHT);
		window_width=glutGet(GLUT_INIT_WINDOW_HEIGHT)*scaler;
	}

	//Fix Scaling
	double scale_width=(double)glutGet(GLUT_WINDOW_WIDTH)/window_width;
	double scale_height=(double)glutGet(GLUT_WINDOW_HEIGHT)/window_height;

	//Set Mouse Coordinates
	msl::mouse_x=(x-glutGet(GLUT_WINDOW_WIDTH)/2.0)/scale_width;
	msl::mouse_y=(-y+glutGet(GLUT_WINDOW_HEIGHT)/2.0)/scale_height;
}

//Glut Mouse Motion Callback
static void mouse_motion_2d(int x,int y)
{
	//Set Mouse Coordinates
	msl::mouse_x=x-glutGet(GLUT_WINDOW_WIDTH)/2.0;
	msl::mouse_y=-y+glutGet(GLUT_WINDOW_HEIGHT)/2.0;
}

//Input Check Function
bool msl::input_check(const int key)
{
	//Mouse Buttons
	if(key>=800)
		return mouse_down_2d[key-800];

	//Special Keys
	else if(key>=500)
		return special_2d[key-500];

	//Regular Keys
	else if(key==8||key==9||key==13||key==27||(key>=32&&key<=127))
		return keyboard_2d[key];

	//Other
	return false;
}

//Input Check Pressed Function
bool msl::input_check_pressed(const int key)
{
	//Mouse Buttons
	if(key>=800)
		return mouse_pressed_2d[key-800];

	//Special Keys
	else if(key>=500)
		return special_pressed_2d[key-500];

	//Regular Keys
	else if(key==8||key==9||key==13||key==27||(key>=32&&key<=127))
		return keyboard_pressed_2d[key];

	//Other
	return false;
}

//Input Check Released Function
bool msl::input_check_released(const int key)
{
	//Mouse Buttons
	if(key>=800)
		return mouse_released_2d[key-800];

	//Special Keys
	else if(key>=500)
		return special_released_2d[key-500];

	//Regular Keys
	else if(key==8||key==9||key==13||key==27||(key>=32&&key<=127))
		return keyboard_released_2d[key];

	//Other
	return false;
}

//Input Start Routine (Sets up glut)
void msl::input_setup(const bool scaled_window)
{
	//For Pressed and Released
	glutIgnoreKeyRepeat(true);

	//Set Glut Callbacks
	glutKeyboardFunc(keyboard_down_2d);
	glutKeyboardUpFunc(keyboard_up_2d);
	glutSpecialFunc(special_down_2d);
	glutSpecialUpFunc(special_up_2d);
	glutMouseFunc(mouse_2d);

	//Scaled Mouse Motion Functions
	if(scaled_window)
	{
		glutMotionFunc(mouse_motion_2d_scaled);
		glutPassiveMotionFunc(mouse_motion_2d_scaled);
	}

	//Non Scaled Mouse Motion Functions
	else
	{
		glutMotionFunc(mouse_motion_2d);
		glutPassiveMotionFunc(mouse_motion_2d);
	}

	//Set  Mouse Coordinates
	msl::mouse_x=-9999999;
	msl::mouse_y=-9999999;

	//Initialize List of Shifted Keys to -1
	for(int ii=0;ii<256;++ii)
		shifted_keys[ii]=-1;

	//Add Alphabetical Characters to Shifted Keys
	for(int ii=97;ii<=122;++ii)
		shifted_keys[ii]=ii-32;
	for(int ii=65;ii<=90;++ii)
		shifted_keys[ii]=ii+32;

	//Add Other Characters to Shifted Keys
	shifted_keys['`']='~';
	shifted_keys['1']='!';
	shifted_keys['2']='@';
	shifted_keys['3']='#';
	shifted_keys['4']='$';
	shifted_keys['5']='%';
	shifted_keys['6']='^';
	shifted_keys['7']='&';
	shifted_keys['8']='*';
	shifted_keys['9']='(';
	shifted_keys['0']=')';
	shifted_keys['-']='_';
	shifted_keys['=']='+';
	shifted_keys['[']='{';
	shifted_keys[']']='}';
	shifted_keys['\\']='|';
	shifted_keys[';']=':';
	shifted_keys['\'']='\"';
	shifted_keys[',']='<';
	shifted_keys['.']='>';
	shifted_keys['/']='?';
	shifted_keys['~']='`';
	shifted_keys['!']='1';
	shifted_keys['@']='2';
	shifted_keys['#']='3';
	shifted_keys['$']='4';
	shifted_keys['%']='5';
	shifted_keys['^']='6';
	shifted_keys['&']='7';
	shifted_keys['*']='8';
	shifted_keys['(']='9';
	shifted_keys[')']='0';
	shifted_keys['_']='-';
	shifted_keys['+']='=';
	shifted_keys['{']='[';
	shifted_keys['}']=']';
	shifted_keys['|']='\\';
	shifted_keys[':']=';';
	shifted_keys['\"']='\'';
	shifted_keys['<']=',';
	shifted_keys['>']='.';
	shifted_keys['?']='/';
}

//Input Released and Pressed Keys Reset Function (Call at end of timer function)
void msl::input_reset()
{
	//Reset Pressed and Released Keyboard Keys
	for(int ii=0;ii<256;++ii)
	{
		keyboard_pressed_2d[ii]=false;
		keyboard_released_2d[ii]=false;
		special_pressed_2d[ii]=false;
		special_released_2d[ii]=false;
	}

	//Reset Pressed Mouse Buttons
	mouse_pressed_2d[0]=false;
	mouse_pressed_2d[1]=false;
	mouse_pressed_2d[2]=false;

	//Reset Released Mouse Buttons
	mouse_released_2d[0]=false;
	mouse_released_2d[1]=false;
	mouse_released_2d[2]=false;
}