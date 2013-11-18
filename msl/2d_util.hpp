//2D Utilities Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Begin Define Guards
#ifndef MSL_2D_UTIL_H
#define MSL_2D_UTIL_H

//MSL Namespace
namespace msl
{
	//Color Class Declaration
	class color
	{
		public:
			//Friend Classes
			friend class sprite;

			//Constructor (Default)
			color(const float red=1,const float green=1,const float blue=1,const float alpha=1);

			//Member Variables
			float r;
			float g;
			float b;
			float a;
	};

	//Point Direction Function (Returns direction between two points in degrees)
	double point_direction(const double x1,const double y1,const double x2,const double y2);

	//Point Distance Function (Returns distance between two points)
	double point_distance(const double x1,const double y1,const double x2,const double y2);
}

//End Define Guards
#endif

//Example
/*
#include "2d_util.hpp"
#include <iostream>
#include "sprite.hpp"
using namespace msl;

int main()
{
	double ax=10;
	double ay=0;
	double bx=10;
	double by=20;

	std::cout<<"Direction between points a and b is:\t"<<point_direction(ax,ay,bx,by)<<std::endl;
	std::cout<<"Distance between points a and b is:\t"<<point_distance(ax,ay,bx,by)<<std::endl;

	return 0;
}
*/