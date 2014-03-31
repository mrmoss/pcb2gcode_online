//Glut Input Header
//	Created By:		Mike Moss
//	Modified On:	11/23/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut

//Begin Define Guards
#ifndef MSL_GLUT_INPUT_H
#define MSL_GLUT_INPUT_H

//Regular Keyboard Bindings
#define kb_bang					'!'
#define kb_quote				'\"'
#define kb_she					'#'
#define kb_dollar				'$'
#define kb_percent				'%'
#define kb_ampersand			'&'
#define kb_apostrophe			'\''
#define kb_open_parentheses		'('
#define kb_close_parentheses	')'
#define kb_asterisk				'*'
#define kb_plus					'+'
#define kb_comma				','
#define kb_minus				'-'
#define kb_period				'.'
#define kb_forward_slash		'/'
#define kb_0					'0'
#define kb_1					'1'
#define kb_2					'2'
#define kb_3					'3'
#define kb_4					'4'
#define kb_5					'5'
#define kb_6					'6'
#define kb_7					'7'
#define kb_8					'8'
#define kb_9					'9'
#define kb_colon				':'
#define kb_semi_colon			';'
#define kb_less_than			'<'
#define kb_equal				'='
#define kb_greater_than			'>'
#define kb_question				'?'
#define kb_at					'@'
#define kb_A					'A'
#define kb_B					'B'
#define kb_C					'C'
#define kb_D					'D'
#define kb_E					'E'
#define kb_F					'F'
#define kb_G					'G'
#define kb_H					'H'
#define kb_I					'I'
#define kb_J					'J'
#define kb_K					'K'
#define kb_L					'L'
#define kb_M					'M'
#define kb_N					'N'
#define kb_O					'O'
#define kb_P					'P'
#define kb_Q					'Q'
#define kb_R					'R'
#define kb_S					'S'
#define kb_T					'T'
#define kb_U					'U'
#define kb_V					'V'
#define kb_W					'W'
#define kb_X					'X'
#define kb_Y					'Y'
#define kb_Z					'Z'
#define kb_open_square			'['
#define kb_back_slash			'\\'
#define kb_close_square			']'
#define kb_carrot				'^'
#define kb_under_score			'_'
#define kb_accent				'`'
#define kb_a					'a'
#define kb_b					'b'
#define kb_c					'c'
#define kb_d					'd'
#define kb_e					'e'
#define kb_f					'f'
#define kb_g					'g'
#define kb_h					'h'
#define kb_i					'i'
#define kb_j					'j'
#define kb_k					'k'
#define kb_l					'l'
#define kb_m					'm'
#define kb_n					'n'
#define kb_o					'o'
#define kb_p					'p'
#define kb_q					'q'
#define kb_r					'r'
#define kb_s					's'
#define kb_t					't'
#define kb_u					'u'
#define kb_v					'v'
#define kb_w					'w'
#define kb_x					'x'
#define kb_y					'y'
#define kb_z					'z'
#define kb_open_bracket			'{'
#define kb_pipe					'|'
#define kb_close_bracket		'}'
#define kb_tilde				'~'

//Advanced Keyboard Bindings
#define kb_backspace			8
#define kb_tab					9
#define kb_enter				13
#define kb_escape				27
#define kb_space				32
#define kb_insert				108
#define kb_numlock				109
#define kb_delete				127

//Special Keyboard Bindings
#define kb_BASE					500
#define kb_f1					(kb_BASE+1)
#define kb_f2					(kb_BASE+2)
#define kb_f3					(kb_BASE+3)
#define kb_f4					(kb_BASE+4)
#define kb_f5					(kb_BASE+5)
#define kb_f6					(kb_BASE+6)
#define kb_f7					(kb_BASE+7)
#define kb_f8					(kb_BASE+8)
#define kb_f9					(kb_BASE+9)
#define kb_f10					(kb_BASE+10)
#define kb_f11					(kb_BASE+11)
#define kb_f12					(kb_BASE+12)
#define kb_left					(kb_BASE+100)
#define kb_up					(kb_BASE+101)
#define kb_right				(kb_BASE+102)
#define kb_down					(kb_BASE+103)
#define kb_page_up				(kb_BASE+104)
#define kb_page_down			(kb_BASE+105)
#define kb_home					(kb_BASE+106)
#define kb_end					(kb_BASE+107)

//Mouse Bindings
#define mb_BASE					800
#define mb_left					(mb_BASE+0)
#define mb_middle				(mb_BASE+1)
#define mb_right				(mb_BASE+2)
#define mb_scroll_up			(mb_BASE+3)
#define mb_scroll_down			(mb_BASE+4)

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