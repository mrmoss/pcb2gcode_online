//2D Sprite Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glut/freeglut
//	soil

//Begin Define Guards
#ifndef MSL_SPRITE_H
#define MSL_SPRITE_H

//2D Utilities Header
#include "2d_util.hpp"

//Map Header
#include <map>

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	//Sprite Class Decalaration
	class sprite
	{
		public:
			//Constructor (Default, Raw OpenGL Texture)
			sprite(const unsigned int texture=0,const unsigned int number_of_frames=1);

			//Constructor (String Filename)
			sprite(const std::string& filename,const unsigned int number_of_frames=1);

			//Release Texture Function (Releases OpenGL Memory)
			void release();

			//Number of Frames Accessor
			unsigned int number_of_frames() const;

			//Dimensions Accessors
			unsigned int width() const;
			unsigned int height() const;

			//Sprite Class Set Origin Mutator
			void set_origin(const double x,const double y);

			//Draw Function
			void draw(const double x,const double y,const double rotation=0.0,const unsigned int frame=0,
				const double scale_x=1,const double scale_y=1,
				const msl::color& color=msl::color(1,1,1,1),const bool smooth=true) const;

		private:
			//Member Variables
			unsigned int _texture;
			unsigned int _number_of_frames;
			unsigned int _width;
			unsigned int _height;
			double _origin_x;
			double _origin_y;
	};
}

//End Define Guards
#endif

//Example (You need to make a sprite called sprite.png for this to work)
/*
//Sprite Header
#include "sprite.hpp"

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
msl::sprite my_sprite;

//Main
int main(int argc,char** argv)
{
	//Initialize Glut
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);

	//Create Glut Window
	glutCreateWindow("MSL Sprite");

	//Load Sprite
	my_sprite=msl::sprite("sprite.png");

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

	//Draw Sprite (At (0,100) at a 90 degree angle)
	my_sprite.draw(0,100,90);

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