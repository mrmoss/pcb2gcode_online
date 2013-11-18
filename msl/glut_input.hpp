//Glut Input Header
//	Created By:		Mike Moss
//	Modified On:	04/24/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut

//Begin Define Guards
#ifndef MSL_GLUT_INPUT_H
#define MSL_GLUT_INPUT_H

//Input Bindings
#define kb_f1           0x0001
#define kb_f2           0x0002
#define kb_f3           0x0003
#define kb_f4           0x0004
#define kb_f5           0x0005
#define kb_f6           0x0006
#define kb_f7           0x0007
#define kb_f8           0x0008
#define kb_f9           0x0009
#define kb_f10          0x000A
#define kb_f11          0x000B
#define kb_f12          0x000C
#define kb_enter        0x100D
#define kb_escape       0x101B
#define kb_space        0x1020
#define kb_0            0x1030
#define kb_1            0x1031
#define kb_2            0x1032
#define kb_3            0x1033
#define kb_4            0x1034
#define kb_5            0x1035
#define kb_6            0x1036
#define kb_7            0x1037
#define kb_8            0x1038
#define kb_9            0x1039
#define kb_a            0x1061
#define kb_b            0x1062
#define kb_c            0x1063
#define kb_d            0x1064
#define kb_e            0x1065
#define kb_f            0x1066
#define kb_g            0x1067
#define kb_h            0x1068
#define kb_i            0x1069
#define kb_j            0x106A
#define kb_k            0x106B
#define kb_l            0x106C
#define kb_m            0x106D
#define kb_n            0x106E
#define kb_o            0x106F
#define kb_p            0x1070
#define kb_q            0x1071
#define kb_r            0x1072
#define kb_s            0x1073
#define kb_t            0x1074
#define kb_u            0x1075
#define kb_v            0x1076
#define kb_w            0x1077
#define kb_x            0x1078
#define kb_y            0x1079
#define kb_z            0x107A
#define kb_left         0x0064
#define kb_up           0x0065
#define kb_right        0x0066
#define kb_down         0x0067
#define kb_pageup       0x0068
#define kb_pagedown     0x0069
#define kb_home         0x006A
#define kb_end          0x006B
#define kb_insert       0x006C
#define mb_left         0x2000
#define mb_middle       0x2001
#define mb_right        0x2002

//MSL Namespace
namespace msl
{
	//Input Variables
	extern double mouse_x;
	extern double mouse_y;

	//Input Check Functions
	bool input_check(const int key);
	bool input_check_pressed(const int key);
	bool input_check_released(const int key);

	//Input Start Routine (Sets up glut)
	void input_setup(const bool scaled_window=true);

	//Input Released and Pressed Keys Reset Function (Call at end of timer function)
	void input_reset();
}

//End Define Guards
#endif

//Example
/*
//Glut Input Header
#include "glut_input.hpp"

//Glut Header
#ifndef __APPLE__
	#include <GL/glut.h>
#else
	#include <GLUT/glut.h>
#endif

//Glut Callback Declarations
void idle();
void display();
void reshape(int width,int height);

//Globals
bool red=false;

//Main
int main(int argc,char** argv)
{
	//Initialize Glut
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);

	//Create Glut Window
	glutCreateWindow("MSL Glut Input");

	//Glut Settings
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);

	//Glut Callbacks
	msl::input_setup();
	glutIdleFunc(idle);
	glutDisplayFunc(display);

	//Reshape Callback
	glutReshapeFunc(reshape);

	//Start Glut
	glutMainLoop();

	//Call Me Plz!!! T_T
	return 0;
}

//Glut Idle Callback
void idle()
{
	//Escape Key Exits
	if(msl::input_check_pressed(kb_escape))
		exit(0);

	//Change Color of Quad
	red=msl::input_check(mb_left);

	//Reset Pressed and Released Inputs
	msl::input_reset();

	//Redisplay
	glutPostRedisplay();
}

//Glut Display Callback
void display()
{
	//Pre-Display (Clearing and Such)
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
	glLoadIdentity();

	//Draw Quad at Mouse Coordinates
	glPushMatrix();
		glTranslatef(msl::mouse_x,msl::mouse_y,0);

		if(red)
			glColor3f(1,0,0);
		else
			glColor3f(1,1,1);

		glBegin(GL_QUADS);
			glVertex2d(-30,30);
			glVertex2d(30,30);
			glVertex2d(30,-30);
			glVertex2d(-30,-30);
		glEnd();
	glPopMatrix();

	//Double Buffering
	glutSwapBuffers();
}

//Glut Reshape Callback
void reshape(int width,int height)
{
	//Set Viewport Size
	glViewport(0,0,width,height);

	//Goto Projection View
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Get Original Dimensions
	double window_width=glutGet(GLUT_INIT_WINDOW_WIDTH);
	double window_height=glutGet(GLUT_INIT_WINDOW_HEIGHT);

	//If Width Dependent
	if(width<=height)
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

	//Set View
	glOrtho(-window_width/2,window_width/2,-window_height/2,window_height/2,0,1);

	//Return to Model View
	glMatrixMode(GL_MODELVIEW);
}
*/