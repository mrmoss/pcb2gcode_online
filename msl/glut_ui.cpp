//Glut User Interface Source
//	Created By:		Mike Moss
//	Modified On:	12/18/2013

//Required Libraries:
//	ftgl
//	freetype
//	gl
//	glew
//	glu
//	glut/freeglut

//Definitions for "glut_ui.hpp"
#include "glut_ui.hpp"

//Glut Input Header
#include "glut_input.hpp"

#include "string_util.hpp"

#include "time_util.hpp"

//OpenGL Headers
#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

msl::widget::widget(const double x,const double y,const double width,const double height,
	const bool hover,const bool down,const bool pressed,const bool disabled,const bool readonly,
	const bool visible,const double padding,const msl::color& background_color_from,
	const msl::color& background_color_to,const msl::color& outline_color,const msl::color& outline_color_hover,
	const msl::color& outline_color_disabled,const msl::color& text_color,const msl::color& text_color_disabled):
		x(x),y(y),width(width),height(height),display_width(width),display_height(height),hover(hover),down(down),
		pressed(pressed),disabled(disabled),readonly(readonly),visible(visible),padding(padding),
		background_color_from(background_color_from),background_color_to(background_color_to),
		outline_color(outline_color),outline_color_hover(outline_color_hover),
		outline_color_disabled(outline_color_disabled),text_color(text_color),
		text_color_disabled(text_color_disabled)
{}

msl::widget::~widget()
{}

msl::button::button(const std::string& value,const double x,const double y):widget(x,y),value(value)
{}

void msl::button::loop(const double dt)
{
	//Update Dimensions
	update_dimensions();

	//Only Update If Visible
	if(visible)
	{
		//Determine Mouse Hover
		bool new_hover=(msl::mouse_x>=x&&msl::mouse_x<=x+display_width&&
			msl::mouse_y<=y&&msl::mouse_y>=y-display_height)&&!disabled;

		//Mouse Leave
		if(hover&&!new_hover)
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

		//Mouse Enter
		if(!hover&&new_hover)
			glutSetCursor(GLUT_CURSOR_INFO);

		//Update Hover Status
		hover=new_hover;

		//Update Down/Pressed
		down=hover&&!disabled&&!readonly&&msl::input_check(mb_left);
		pressed=hover&&!disabled&&!readonly&&msl::input_check_released(mb_left);

		//If Disabled, Unpress Button
		if(disabled)
		{
			hover=false;
			down=false;
			pressed=false;
		}
	}
}

void msl::button::draw()
{
	//Only Draw If Visible
	if(visible)
	{
		//Determine Colors
		msl::color but_col_to=background_color_to;
		msl::color but_col_from=background_color_from;
		msl::color out_col=outline_color;
		msl::color tex_col=text_color;

		if(hover)
			out_col=outline_color_hover;

		if(pressed||down)
			std::swap(but_col_from,but_col_to);

		if(disabled)
		{
			but_col_from=but_col_to;
			out_col=outline_color_disabled;
			tex_col=text_color_disabled;
		}

		//Draw Background
		msl::draw_rectangle_gradient(x,y,display_width,display_height,true,
			but_col_from,but_col_from,but_col_to,but_col_to);

		//Draw Outline
		msl::draw_rectangle(x,y,display_width,display_height,false,out_col);

		//Draw Text
		msl::draw_text(x+display_width/2.0,y-display_height/2.0,value,msl::CENTER,msl::MIDDLE,tex_col);
	}
}

void msl::button::update_dimensions()
{
	//Set Display Width
	display_width=width;

	if(width<0)
		display_width=msl::text_width(value)+padding*2;

	//Set Display Height
	display_height=height;

	if(height<0)
		display_height=padding*2+msl::text_height("test");
}

msl::checkbox::checkbox(const bool value,const double x,const double y):widget(x,y),value(value)
{
	//Update Dimensions
	update_dimensions();
}

void msl::checkbox::loop(const double dt)
{
	//Update Dimensions
	update_dimensions();

	//Only Update If Visible
	if(visible)
	{
		//Determine Mouse Hover
		bool new_hover=(msl::mouse_x>=x&&msl::mouse_x<=x+display_width&&
			msl::mouse_y<=y&&msl::mouse_y>=y-display_height)&&!disabled;

		//Mouse Leave
		if(hover&&!new_hover)
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

		//Mouse Enter
		if(!hover&&new_hover)
			glutSetCursor(GLUT_CURSOR_INFO);

		//Update Hover Status
		hover=new_hover;

		//Update Down/Pressed
		down=hover&&!disabled&&!readonly&&msl::input_check(mb_left);
		pressed=hover&&!disabled&&!readonly&&msl::input_check_released(mb_left);

		//If Disabled, Unpress Button
		if(disabled)
		{
			hover=false;
			down=false;
			pressed=false;
		}

		//Toggle With a Mouse Click
		if(pressed&&!disabled&&!readonly)
			value=!value;
	}
}

void msl::checkbox::draw()
{
	//Only Draw If Visible
	if(visible)
	{
		//Determine Colors
		msl::color but_col_to=background_color_to;
		msl::color but_col_from=background_color_from;
		msl::color out_col=outline_color;
		msl::color tex_col=text_color;

		if(hover)
			out_col=outline_color_hover;

		if(pressed||down)
			std::swap(but_col_from,but_col_to);

		if(disabled)
		{
			but_col_from=but_col_to;
			out_col=outline_color_disabled;
			tex_col=text_color_disabled;
		}

		//Draw Background
		msl::draw_rectangle_gradient(x,y,display_width,display_height,true,
			but_col_from,but_col_from,but_col_to,but_col_to);

		//Draw Outline
		msl::draw_rectangle(x,y,display_width,display_height,false,out_col);

		//Draw Checkmark
		if(value)
		{
			msl::color col=text_color;

			if(disabled)
				col=text_color_disabled;

			double x_offset=display_width/2.0-0.5;
			double y_offset=-display_height/2.0-0.5;

			msl::draw_circle(x+x_offset-3,y+y_offset,1.5,col);
			msl::draw_circle(x+x_offset-2,y+y_offset-1,1.5,col);

			for(int ii=-1;ii<=3;++ii)
				msl::draw_circle(x+x_offset+ii,y+y_offset+ii-1,1.5,col);
		}
	}
}

void msl::checkbox::update_dimensions()
{
	//Set Display Width
	display_width=width;

	if(width<0)
		display_width=8+padding*2;

	//Set Display Height
	display_height=height;

	if(height<0)
		display_height=8+padding*2;
}

msl::slider::slider(const double min,const double max,const double x,const double y):
		widget(x,y,max-min),value(min),min(min),max(max),vertical(false),
		track_color(msl::color(0.3,0.3,0.3,1)),track_color_disabled(msl::color(0.4,0.4,0.4,1)),
		handle("",x,y),drag_(false)
{
	//Default Handle Size
	handle.width=handle.height=16;

	//Update Dimensions
	update_dimensions();
}

void msl::slider::loop(const double dt)
{
	//Update Dimensions
	update_dimensions();

	//Update Button
	update_button(dt);

	//Only Update If Visible
	if(visible)
	{
		//Initial Click, Enable Drag
		if(handle.down)
			drag_=true;

		//Mouse Up, Disable Drag
		if(msl::input_check_released(mb_left))
			drag_=false;

		//Keep Button Pressed Down While Dragging
		if(drag_)
			down=true;

		//Move Handle With Mouse
		if(drag_)
		{
			if(vertical)
				handle.y=msl::mouse_y+handle.display_height/2.0;
			else
				handle.x=msl::mouse_x-handle.display_width/2.0;
		}
		else
		{
			if(vertical)
				handle.y=y+(value-min)/(max-min)*(display_height-handle.display_height);
			else
				handle.x=x+(value-min)/(max-min)*(display_width-handle.display_width);
		}

		//Limit Handle Position
		if(vertical)
		{
			if(handle.y<y)
				handle.y=y;

			if(handle.y>y+display_height-handle.display_height)
				handle.y=y+display_height-handle.display_height;
		}
		else
		{
			if(handle.x<x)
				handle.x=x;

			if(handle.x>x+display_width-handle.display_width)
				handle.x=x+display_width-handle.display_width;
		}

		//Update Value
		if(vertical)
			value=(handle.y-y)/(display_height-handle.display_height)*(max-min)+min;
		else
			value=(handle.x-x)/(display_width-handle.display_width)*(max-min)+min;

		//Constrain Other Position
		if(vertical)
			handle.x=x;
		else
			handle.y=y;
	}
}

void msl::slider::draw()
{
	//Only Draw If Visible
	if(visible)
	{
		//Determine Colors
		msl::color track_col=track_color;

		if(disabled)
			track_col=track_color_disabled;

		//Draw Track
		if(vertical)
			msl::draw_rectangle(x+handle.display_width/2.0-2,y+display_height-handle.display_height,4,display_height,true,track_col);
		else
			msl::draw_rectangle(x,y-handle.display_height/2.0+2,display_width,4,true,track_col);

		//Draw Handle
		handle.draw();
	}
}

void msl::slider::update_button(const double dt)
{
	//Update Button Settings
	handle.down=down;
	handle.pressed=pressed;
	handle.disabled=disabled;
	handle.visible=visible;
	handle.readonly=readonly;

	//Update Button Colors
	handle.background_color_from=background_color_from;
	handle.background_color_to=background_color_to;
	handle.outline_color=outline_color;
	handle.outline_color_hover=outline_color_hover;
	handle.outline_color_disabled=outline_color_disabled;
	handle.text_color=text_color;
	handle.text_color_disabled=text_color_disabled;

	handle.loop(dt);
}

void msl::slider::update_dimensions()
{
	//Update Dimensions
	if(vertical)
	{
		//Update Width
		display_width=handle.display_width;

		//Update Height
		display_height=height;

		if(height<0)
			display_height=max-min;
	}
	else
	{
		//Update Width
		display_width=width;

		if(width<0)
			display_width=max-min;

		//Update Height
		display_height=handle.display_height;
	}
}

msl::textbox::textbox(const std::string& value,const double x,const double y):widget(x,y,-1,-1),focus(false),
	value(value),max_length(-1),cursor(0),view_start(1),view_end(1),background_color(1,1,1,1),
	background_color_disabled(0.8,0.8,0.8,1),blink_timer_(0),blink_wait_(500),blink_show_(false),repeat_timer_(0),
	repeat_initial_wait_(500),repeat_key_wait_(80),repeat_key_(-1)
{}

void msl::textbox::loop(const double dt)
{
	//Limit Size
	if(max_length>=0&&value.size()>(unsigned int)max_length)
		value.resize(max_length);

	//Get Rid of Newlines...
	for(unsigned int ii=0;ii<value.size();++ii)
			if(value[ii]=='\n')
				value.erase(ii,1);

	//Figure Out Dimensions
	display_width=width+padding*2;

	if(width<0)
		display_width=msl::text_width(value)+padding*2;

	display_height=height+padding*2;

	if(height<0)
		display_height=msl::text_height(value)+padding*2;

	//Determine Mouse Hover
	bool new_hover=(msl::mouse_x>=x&&msl::mouse_x<=x+display_width&&
		msl::mouse_y<=y&&msl::mouse_y>=y-display_height)&&!disabled;

	//Mouse Leave
	if(hover&&!new_hover)
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	//Mouse Enter
	if(!hover&&new_hover)
		glutSetCursor(GLUT_CURSOR_TEXT);

	//Update Hover Status
	hover=new_hover;

	//Only Work When Enabled and Writable
	if(!disabled&&!readonly)
	{
		//Determine Down and Pressed
		down=hover&&msl::input_check(mb_left);
		pressed=hover&&msl::input_check_released(mb_left);

		//Determine Repeat Keys
		bool repeat=false;

		if(msl::millis()>repeat_timer_)
		{
			repeat_timer_=msl::millis()+repeat_key_wait_;
			repeat=true;
		}

		//Check for Mouse Clicks Inside of Textbox
		if(msl::input_check_pressed(mb_left))
		{
			//Set Focus
			focus=hover;

			//If Focusing, Put Cursor in the Right Spot
			if(focus)
			{
				//Get Cursor Difference
				double text_start_x=x+padding;
				double cursor_difference=mouse_x-text_start_x;

				//Put Cursor All the Way Back
				cursor=-1;

				//Find Cursor Position
				for(int ii=0;ii<view_end-view_start;++ii)
				{
					if(msl::text_width(value.substr(view_start,ii))>=cursor_difference)
					{
						cursor=view_start+ii;
						break;
					}
				}

				//If Cursor is -1, then Cursor is at the End
				if(cursor<0)
					cursor=view_end;
			}
		}

		//Check for Mouse Clicks Outside of Textbox
		if(!hover&&(msl::input_check_released(mb_left)||msl::input_check_released(mb_middle)||msl::input_check_released(mb_right)))
			focus=false;

		//If Focused
		if(focus)
		{
			//Blink Cursor
			if(msl::millis()>blink_timer_)
			{
				blink_timer_=msl::millis()+blink_wait_;
				blink_show_=!blink_show_;
			}

			//Move Cursor Right
			if(msl::input_check_pressed(kb_right)||(repeat_key_==kb_right&&repeat==true))
			{
				++cursor;
				repeat_check(kb_right);
			}

			//Move Cursor Left
			if(msl::input_check_pressed(kb_left)||(repeat_key_==kb_left&&repeat==true))
			{
				--cursor;
				repeat_check(kb_left);
			}

			//Move Cursor to Start
			if(msl::input_check_pressed(kb_home)||(repeat_key_==kb_home&&repeat==true))
			{
				cursor=0;
				repeat_check(kb_home);
			}

			//Move Cursor to End
			if(msl::input_check_pressed(kb_end)||(repeat_key_==kb_end&&repeat==true))
			{
				cursor=value.size();
				repeat_check(kb_end);
			}

			//Backspace
			if(msl::input_check_pressed(kb_backspace)||(repeat_key_==kb_backspace&&repeat==true))
			{
				if(cursor-1>=0)
				{
					--cursor;
					value.erase(cursor,1);
					find_end();
					repeat_check(kb_backspace);
				}
			}

			//Delete
			if(msl::input_check_pressed(kb_delete)||(repeat_key_==kb_delete&&repeat==true))
			{
				if((unsigned int)cursor<value.size())
				{
					value.erase(cursor,1);
					find_end();
					repeat_check(kb_delete);
				}
			}

			//Key Input
			for(int ii=32;ii<=126;++ii)
			{
				if(msl::input_check_pressed(ii)||(repeat_key_==ii&&repeat==true))
				{
					if(cursor>=0&&(unsigned int)cursor<=value.size())
					{
						value.insert(value.begin()+cursor,(char)ii);
						++cursor;
						find_end();
						repeat_check(ii);
					}
				}
			}
		}

		//If Not Focused
		else
		{
			cursor=0;
			find_end();
		}
	}

	//Update Size if Disabled
	else
	{
		cursor=0;
		find_end();
	}

	//Check For Repeat Key Release
	if(msl::input_check_released(repeat_key_))
		repeat_key_=-1;

	//Limit Cursor
	if(cursor<0)
		cursor=0;

	if((unsigned int)cursor>value.size())
		cursor=value.size();

	//Limit View Start
	if(view_start>cursor)
	{
		view_start=cursor;
		find_end();
	}

	//Limit View End
	if(cursor>view_end)
	{
		view_end=cursor;
		find_start();
	}

	if((unsigned int)view_end>value.size())
		view_end=value.size();
}

void msl::textbox::draw()
{
	//To draw or not to draw...
	if(visible)
	{
		//Figure Out Colors
		msl::color bg_col=background_color;
		msl::color out_col=outline_color;
		msl::color tex_col=text_color;

		if(hover)
			out_col=outline_color_hover;

		if(disabled)
		{
			bg_col=background_color_disabled;
			out_col=outline_color_disabled;
			tex_col=text_color_disabled;
		}

		//Draw Background
		msl::draw_rectangle(x,y,display_width,display_height,true,bg_col);

		//Draw Border
		msl::draw_rectangle(x,y,display_width,display_height,false,out_col);

		//Draw Text
		std::string display_text=value.substr(view_start,view_end-view_start);
		double display_text_x=x+padding;
		double display_text_y=y-display_height/2.0;
		msl::draw_text(display_text_x,display_text_y,display_text,msl::LEFT,msl::MIDDLE,tex_col);

		//Draw Cursor
		if(focus&&blink_show_)
		{
			std::string cursor_x_text=value.substr(view_start,cursor-view_start);
			double cursor_x=x+padding+msl::text_width(cursor_x_text);
			double cursor_y=y-padding;
			double cursor_height=msl::text_height(display_text);
			msl::draw_line(cursor_x,cursor_y,cursor_x,cursor_y-cursor_height,tex_col);
		}
	}
}

void msl::textbox::find_end()
{
	//Limit Start
	if(view_start<0)
		view_start=0;

	if((unsigned int)view_start>value.size())
		view_start=value.size();

	//Set End to Start
	view_end=value.size();

	//Determine Max Text Width
	double max_text_width=display_width-padding*2;

	//Find View End
	while(msl::text_width(value.substr(view_start,view_end-view_start))>max_text_width&&view_end>view_start)
		--view_end;
}

void msl::textbox::find_start()
{
	//Limit End
	if(view_end<0)
		view_end=0;

	if((unsigned int)view_end>value.size())
		view_end=value.size();

	//Set Start to End
	view_start=0;

	//Determine Max Text Width
	double max_text_width=display_width-padding*2;

	//Find View Start
	while(msl::text_width(value.substr(view_start,view_end-view_start))>max_text_width&&view_start<view_end)
		++view_start;
}

void msl::textbox::repeat_check(const int key)
{
	//If Not Currently Repeating a Key
	if(repeat_key_==-1)
	{
		//Set Repeat Key and Set Timer
		repeat_key_=key;
		repeat_timer_=msl::millis()+repeat_initial_wait_;
	}
}

msl::dock::dock(const double x,const double y):widget(x,y)
{
	//No Gradients By Default...
	background_color_from=background_color_to;
}

msl::dock::~dock()
{}

void msl::dock::draw()
{
	//Draw Background
	msl::draw_rectangle_gradient(x,y,display_width,display_height,true,background_color_from,
		background_color_from,background_color_to,background_color_to);

	//Draw Outline
	msl::draw_rectangle(x,y,display_width,display_height,false,outline_color);

	//Draw Widgets
	for(unsigned int ii=0;ii<widgets.size();++ii)
		if(widgets[ii]!=NULL)
			widgets[ii]->draw();
}

msl::hdock::hdock(const double x,const double y):dock(x,y)
{}

msl::hdock::~hdock()
{}

void msl::hdock::loop(const double dt)
{
	//Initial Dimensions
	display_width=padding;
	display_height=0;

	//Go Through Widgets
	for(unsigned int ii=0;ii<widgets.size();++ii)
	{
		//Check for Bad Pointers
		if(widgets[ii]!=NULL)
		{
			//Set Widget Position
			widgets[ii]->x=x+display_width;
			widgets[ii]->y=y;

			//Increase Width
			display_width+=widgets[ii]->display_width+padding;

			//Find Max Height
			if(widgets[ii]->display_height>display_height)
				display_height=widgets[ii]->display_height;
		}
	}

	//Go Through Widgets Again (Need to Find Max Height First...)
	for(unsigned int ii=0;ii<widgets.size();++ii)
	{
		//Check for Bad Pointers
		if(widgets[ii]!=NULL)
		{
			//Set Widget Y Position
			widgets[ii]->y-=display_height/2.0;
			widgets[ii]->y+=widgets[ii]->display_height/2.0;

			//Update Widget
			widgets[ii]->loop(dt);
		}
	}
}

msl::vdock::vdock(const double x,const double y):dock(x,y)
{}

msl::vdock::~vdock()
{}

void msl::vdock::loop(const double dt)
{
	//Initial Dimensions
	display_width=0;
	display_height=padding;

	//Go Through Widgets
	for(unsigned int ii=0;ii<widgets.size();++ii)
	{
		//Check for Bad Pointers
		if(widgets[ii]!=NULL)
		{
			//Set Widget Position
			widgets[ii]->x=x;
			widgets[ii]->y=y-display_height;

			//Increase Height
			display_height+=widgets[ii]->display_height+padding;

			//Find Max Width
			if(widgets[ii]->display_width>display_width)
				display_width=widgets[ii]->display_width;
		}
	}

	//Go Through Widgets Again (Need to Find Max Width First...)
	for(unsigned int ii=0;ii<widgets.size();++ii)
	{
		//Check for Bad Pointers
		if(widgets[ii]!=NULL)
		{
			//Set Widget X Position
			widgets[ii]->x+=display_width/2.0;
			widgets[ii]->x-=widgets[ii]->display_width/2.0;

			//Update Widget
			widgets[ii]->loop(dt);
		}
	}
}