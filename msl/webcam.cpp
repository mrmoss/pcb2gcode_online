//Webcam Source
//	Created By:		Mike Moss
//	Modified On:	10/16/2013

//Required Libraries:
//	opencv_core
//	opencv_highgui

//Definitions for "webcam.hpp"
#include "webcam.hpp"

//Boolean Operator (Tests if webcam is good)
msl::webcam::operator bool() const
{
	return good();
}

//Not Operator (For boolean operator)
bool msl::webcam::operator!() const
{
	return !good();
}

//Good Function (Tests if webcam is good)
bool msl::webcam::good() const
{
	return (_capture.isOpened()&&!_frame.empty());
}

//Connect Function (Connects to a webcam based on name)
void msl::webcam::connect(const std::string& name)
{
	_capture.open(name);
	update();
}

//Connect Function (Connects to a webcam based on an operating system set index)
void msl::webcam::connect(const int number)
{
	_capture.open(number);
	update();
}

//Close Function (Disconnects from a webcam)
void msl::webcam::close()
{
	_capture.release();
}

//Update Function (Gets current frame from a webcam)
void msl::webcam::update()
{
	_capture>>_frame;
}

//Width Accessor (Returns width of webcam frames)
unsigned int msl::webcam::width() const
{
	return _frame.cols;
}

//Height Accessor (Returns height of webcam frames)
unsigned int msl::webcam::height() const
{
	return _frame.rows;
}

//Data Accessor (Returns pixel data of the current webcam frame, in BGR format)
const void* msl::webcam::data() const
{
	return _frame.ptr();
}