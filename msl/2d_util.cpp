//2D Utilities Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Definitions for "2d_util.hpp"
#include "2d_util.hpp"

//Math Header
#include <math.h>

//Color Class Constructor (Default)
msl::color::color(const float red,const float green,const float blue,const float alpha):r(red),g(green),b(blue),a(alpha)
{}

//Point Direction Function (Returns direction between two points in degrees)
double msl::point_direction(const double x1,const double y1,const double x2,const double y2)
{
	return atan2(y2-y1,x2-x1)*180.0/M_PI;
}

//Point Distance Function (Returns distance between two points)
double msl::point_distance(const double x1,const double y1,const double x2,const double y2)
{
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}