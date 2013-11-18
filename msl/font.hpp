//2D Font Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut
//	soil

//Needs an image strip that has monospaced font in ascii order starting with ascii value ' ' (space) and ending with ascii value '~' (tilde).

//Begin Define Guards
#ifndef MSL_FONT_H
#define MSL_FONT_H

//2d Utility Header
#include "2d_util.hpp"

//Sprite Header
#include "sprite.hpp"

//String Header
#include <string>

//Alignment Bindings
#define font_left         0
#define font_center       1
#define font_right        2
#define font_top          0
#define font_middle       1
#define font_bottom       2

//MSL Namespace
namespace msl
{
	//Font Class Decalaration
	class font
	{
		public:
			//Constructor (Default and 1 argument version)
			font(const std::string& filename="");

			//Draw Function
			void draw(const double x,const double y,const std::string& text,const int h_align=font_left,
				const int v_align=font_top,const msl::color& color=msl::color(1,1,1,1)) const;

		private:
			//Member Variables
			msl::sprite _sprite;
	};
}

//End Define Guards
#endif

//Example (You need to make a font image to the specifications at the top of this file called font.png for this to work)
/*
//Font Header
#include "font.hpp"

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
msl::font my_font;

//Main
int main(int argc,char** argv)
{
	//Initialize Glut
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);

	//Create Glut Window
	glutCreateWindow("MSL Font");

	//Load Font
	my_font=msl::font("font.png");

	//Glut Settings
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);

	//Glut Callbacks
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

	//Draw Some Text (At (0,0) centered both horizontally and vertically)
	my_font.draw(0,0,"Hello world!\n123\n456",font_center,font_middle);

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