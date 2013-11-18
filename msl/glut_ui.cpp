//Glut User Interface Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	gl
//	glew
//	glu
//	glui
//	glut/freeglut

//Warning:  Glui is a known memory leaker...well kind of...
//	"I'd call it a static allocation instead of a memory leak." - Dr. Lawlor

//Definitions for "glut_ui.hpp"
#include "glut_ui.hpp"

//Algorithm Header
#include <algorithm>

//Exception Header
#include <stdexcept>

//Button Status Class Declaration (Used to store button pointer, down value, and pressed value)
class button_status
{
	public:
		//Constructor (3 argument version)
		button_status(GLUI_Button* button,bool* value_down,bool* value_pressed);

		//Member Variables
		GLUI_Button* button;
		bool* value_down;
		bool* value_pressed;
};

//Button Status Class Constructor
button_status::button_status(GLUI_Button* button,bool* value_down,bool* value_pressed):
	button(button),value_down(value_down),value_pressed(value_pressed)
{}

//glui Globals
static int glui_window=-1;
static int glui_alignment=ui_left;
static std::vector<GLUI*> glui_panels;
static std::vector<GLUI_Panel*> glui_groups;
static std::vector<GLUI_RadioGroup*> glui_radio_lists;
static std::vector<GLUI_Listbox*> glui_selection_lists;
static int glui_selection_starting_value=0;
static std::vector<button_status> glui_buttons;
static std::vector<GLUI_Scrollbar*> glui_scrollbars;

//glui Check Function (Checks for bad glui starts)
static void glui_check()
{
	//If glui is Not in Use Throw Error
	if(!msl::ui_in_use())
		throw std::runtime_error("msl::glui_check - could not initialize glui!");
}

//UI Setup Function (Sets up glui)
void msl::ui_setup(const int glut_window)
{
	//Set Glut Window to Draw In
	glui_window=glut_window;
}

//UI Stop Function (Stops UI and cleans up glui...as best as it can...because glui is full of memory leaks...not my fault...)
void msl::ui_stop()
{
	//Deallocate Scrollbars
	for(unsigned int ii=0;ii<glui_scrollbars.size();++ii)
		delete glui_scrollbars[ii];

	//Close glui
	GLUI_Master.close_all();
}

//UI In Use Function (Checks if glui is being used...helpful for setting callbacks...because glui has it's own callback functions...)
bool msl::ui_in_use()
{
	return (glui_window>-1);
}

//UI Idle Function (Updates and syncs values)
void msl::ui_idle()
{
	//If UI is Used
	if(ui_in_use())
	{
		//glui Check
		glui_check();

		//Sync UI
		GLUI_Master.sync_live_all();

		//Update button_statuss
		for(unsigned int ii=0;ii<glui_buttons.size();++ii)
		{
			//Pressed
			if(!*glui_buttons[ii].value_down)
				*glui_buttons[ii].value_pressed=glui_buttons[ii].button->currently_inside;

			//Down
			*glui_buttons[ii].value_down=glui_buttons[ii].button->currently_inside;
		}
	}
}

//UI Reset Function (Resets button pressed events...should be done at the very end of the Glut idle function)
void msl::ui_reset()
{
	//glui Check
	glui_check();

	//Update button_status Pressed Values
	for(unsigned int ii=0;ii<glui_buttons.size();++ii)
		*glui_buttons[ii].value_pressed=false;
}

//Panel Begin Command (Creates UI panels, position can be any of the panel position bindings at the top of this document)
void msl::ui_panel_begin(const long position)
{
	//Create a Panel
	glui_panels.push_back(GLUI_Master.create_glui_subwindow(glui_window,position));

	//glui Check
	glui_check();

	//Set Panel to Main Draw Window
	glui_panels.back()->set_main_gfx_window(glui_window);

	//Create Initial Grouping (So when there is no group you make things on the panel)
	glui_groups.push_back((GLUI_Panel*)glui_panels.back());
}

//Panel End Command (Creates UI panels)
void msl::ui_panel_end()
{
	//glui Check
	glui_check();

	//Clear Panels
	glui_panels.clear();

	//Create Initial Grouping (So when there is no group you make things on the panel)
	glui_groups.push_back((GLUI_Panel*)glui_panels.back());

	//Clear Groups
	glui_groups.clear();

	//Clear Radio Lists
	glui_radio_lists.clear();

	//Clear Selection Groups
	glui_selection_lists.clear();

}

//Group Begin Command (Groups UI controls on the panel, the options are faily easy to understand...I hope...)
void msl::ui_group_begin(const std::string& caption,const bool outline)
{
	//Default is Outlined
	int type=GLUI_PANEL_EMBOSSED;

	//If Not Outlined
	if(!outline)
		type=GLUI_PANEL_NONE;

	//glui Check
	glui_check();

	//Add Group
	glui_groups.push_back(glui_panels.back()->add_panel_to_panel(glui_groups.back(),caption.c_str(),type));
}

//Group End Command (Groups UI controls on the panel)
void msl::ui_group_end()
{
	//glui Check
	glui_check();

	//Remove Group (As long as it isn't the parent panel)
	if(glui_groups.size()>1)
		glui_groups.pop_back();
}

//Rollout Begin Command (Like groups but they can be collapsed and they always have an outline, open is to have to rollout opened or closed when the application starts)
void msl::ui_rollout_begin(const std::string caption,const bool open)
{
	//glui Check
	glui_check();

	//Add Rollout
	glui_groups.push_back(glui_panels.back()->add_rollout_to_panel(glui_groups.back(),caption.c_str(),open,GLUI_PANEL_EMBOSSED));
}

//Rollout Begin Command (Like groups but they can be collapsed and they always have an outline)
void msl::ui_rollout_end()
{
	//Same as Group End
	msl::ui_group_end();
}

//Radio Begin Command (Creates radio buttons, value is the variable glui changes when you change the radio option)
void msl::ui_radio_begin(int& value)
{
	//glui Check
	glui_check();

	//Create Radio List
	GLUI_RadioGroup* group_control=glui_panels.back()->add_radiogroup_to_panel(glui_groups.back(),&value);

	//Set Alignment to Current Alignment
	group_control->set_alignment(glui_alignment);

	//Add to Radiolists
	glui_radio_lists.push_back(group_control);
}

//Radio Add Command (Creates radio buttons)
void msl::ui_radio_add(const std::string& caption)
{
	//glui Check
	glui_check();

	//Add Radio button_status (If there is a radio list)
	if(glui_radio_lists.size()>0)
		glui_panels.back()->add_radiobutton_to_group(glui_radio_lists.back(),caption.c_str());
}

//Radio Add Command (Creates radio buttons)
void msl::ui_radio_end()
{
	//glui Check
	glui_check();

	//Pop Back Radio List (If there are any radio lists)
	if(glui_radio_lists.size()>0)
		glui_radio_lists.pop_back();
}

//Selection Begin Command (Creates a text selection box, value is the variable glui changes when you change the selection option)
void msl::ui_selection_begin(const std::string& name,int& value)
{
	//glui Check
	glui_check();

	//Set Starting Index Value to Zero
	glui_selection_starting_value=0;

	//Create Selection Group
	GLUI_Listbox* group_control=glui_panels.back()->add_listbox_to_panel(glui_groups.back(),name.c_str(),&value);

	//Set Alignment to Current Alignment
	group_control->set_alignment(glui_alignment);

	//Add to Selection Lists
	glui_selection_lists.push_back(group_control);
}

//Selection Add Command (Creates a text selection box)
void msl::ui_selection_add(const std::string caption)
{
	//If There Is A Selection List
	if(glui_selection_lists.size()>0)
	{
		//Add Selection
		glui_selection_lists.back()->add_item(glui_selection_starting_value,caption.c_str());

		//Increment Starting Index
		++glui_selection_starting_value;
	}
}

//Selection End Command (Creates a text selection box)
void msl::ui_selection_end()
{
	//glui Check
	glui_check();

	//Pop Back Selection List (If there are any selection lists)
	if(glui_selection_lists.size()>0)
		glui_selection_lists.pop_back();
}

//Separator Command (Creates a horizontal separator)
void msl::ui_separator_add(const bool visible)
{
	//glui Check
	glui_check();

	//Show/Hide Separator
	if(visible)
		glui_panels.back()->add_separator_to_panel(glui_groups.back());
	else
		msl::ui_add_text("");
}

//Collumn Command (Creates a new column on the panel)
void msl::ui_column_add(const bool visible)
{
	//glui Check
	glui_check();

	//Add Column
	glui_panels.back()->add_column_to_panel(glui_groups.back(),visible);
}

//Alignment Command (Sets horizontal placement alignment for controls..alignment can be any of the alignment bindings at the top of this document)
void msl::ui_set_alignment(const int alignment)
{
	glui_alignment=alignment;
}

//Text Command (Creates text)
void msl::ui_add_text(const std::string& text)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_StaticText* text_control=glui_panels.back()->add_statictext_to_panel(glui_groups.back(),text.c_str());

	//Set Alignment to Current Alignment
	text_control->set_alignment(glui_alignment);
}

//Checkbox Command (Creates Checkboxes)
void msl::ui_add_checkbox(const std::string& caption,bool& value)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Checkbox* checkbox_control=glui_panels.back()->add_checkbox_to_panel(glui_groups.back(),caption.c_str(),reinterpret_cast<int*>(&value));

	//Set Alignment to Current Alignment
	checkbox_control->set_alignment(glui_alignment);
}

//Spinner Command (float) (Creates a text field where there is a number you can edit by typing or use small arrows on the side...value is the variable glui changes when you change the number...
//	min and max are the limits for the number...MUST BE FLOAT NOT DOUBLE!!!)
void msl::ui_spinner_add(const std::string& caption,float& value,const float min,const float max)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Spinner* spinner_control=glui_panels.back()->add_spinner_to_panel(glui_groups.back(),caption.c_str(),GLUI_SPINNER_FLOAT,&value);

	//Set Alignment to Current Alignment
	spinner_control->set_alignment(glui_alignment);

	//Set Limits
	spinner_control->set_int_limits(min,max,GLUI_LIMIT_CLAMP);
}

//Spinner Command (int) (Creates a text field where there is a number you can edit by typing or use small arrows on the side...value is the variable glui changes when you change the number...
//	min and max are the limits for the number...MUST BE SIGNED INT NOT UNSIGNED INT!!!)
void msl::ui_spinner_add(const std::string& caption,int& value,const int min,const int max)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Spinner* spinner_control=glui_panels.back()->add_spinner_to_panel(glui_groups.back(),caption.c_str(),GLUI_SPINNER_INT,&value);

	//Set Alignment to Current Alignment
	spinner_control->set_alignment(glui_alignment);

	//Set Limits
	spinner_control->set_float_limits(min,max,GLUI_LIMIT_CLAMP);
}

//Textbox Command (Creates a text field you can edit by typing...value is the variable glui changes when you change the text...)
void msl::ui_textbox_add(const std::string& caption,std::string& value)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_EditText* edittext_control=glui_panels.back()->add_edittext_to_panel(glui_groups.back(),caption.c_str(),value);

	//Set Alignment to Current Alignment
	edittext_control->set_alignment(glui_alignment);
}

//Button Command (Creates a button you can click on with your mouse...down_value is the variable glui changes when you hold the button down...
//	pressed_value is the variable glui changes when you click the button...
void msl::ui_button_add(const std::string& caption,bool& value_down,bool& value_pressed)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Button* button_control=glui_panels.back()->add_button_to_panel(glui_groups.back(),caption.c_str());

	//Set Alignment to Current Alignment
	button_control->set_alignment(glui_alignment);

	//Add to button_statuss Vector
	glui_buttons.push_back(button_status(button_control,&value_down,&value_pressed));
}

//Scrollbar Horizontal Command (float) (Creates a scrollbar...value is the variable glui changes when you move the scrollbar...MUST BE FLOAT NOT DOUBLE!!!)
void msl::ui_scrollbar_horizontal_add(float& value,const float min,const float max)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Scrollbar* scroll_control=new GLUI_Scrollbar(glui_groups.back(),"test",GLUI_SCROLL_HORIZONTAL,&value,GLUI_SCROLL_FLOAT);

	//Set Alignment to Current Alignment
	scroll_control->set_alignment(glui_alignment);

	//Set Limits
	scroll_control->set_int_limits(min,max,GLUI_LIMIT_CLAMP);

	//Add to Scrollbars Vector
	glui_scrollbars.push_back(scroll_control);
}

//Scrollbar Horizontal Command (int) (Creates a scrollbar...value is the variable glui changes when you move the scrollbar...MUST BE SIGNED INT NOT UNSIGNED INT!!!)
void msl::ui_scrollbar_horizontal_add(int& value,const int min,const int max)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Scrollbar* scroll_control=new GLUI_Scrollbar(glui_groups.back(),"test",GLUI_SCROLL_HORIZONTAL,&value,GLUI_SCROLL_INT);

	//Set Alignment to Current Alignment
	scroll_control->set_alignment(glui_alignment);

	//Set Limits
	scroll_control->set_int_limits(min,max,GLUI_LIMIT_CLAMP);

	//Add to Scrollbars Vector
	glui_scrollbars.push_back(scroll_control);
}

//Scrollbar Vertical Command (float) (Creates a scrollbar...value is the variable glui changes when you move the scrollbar...MUST BE FLOAT NOT DOUBLE!!!)
void msl::ui_scrollbar_vertical_add(float& value,const float min,const float max)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Scrollbar* scroll_control=new GLUI_Scrollbar(glui_groups.back(),"test",GLUI_SCROLL_VERTICAL,&value,GLUI_SCROLL_FLOAT);

	//Set Alignment to Current Alignment
	scroll_control->set_alignment(glui_alignment);

	//Set Limits
	scroll_control->set_int_limits(min,max,GLUI_LIMIT_CLAMP);

	//Add to Scrollbars Vector
	glui_scrollbars.push_back(scroll_control);
}

//Scrollbar Horizontal Command (int) (Creates a scrollbar...value is the variable glui changes when you move the scrollbar...MUST BE SIGNED INT NOT UNSIGNED INT!!!)
void msl::ui_scrollbar_vertical_add(int& value,const int min,const int max)
{
	//glui Check
	glui_check();

	//Create Control
	GLUI_Scrollbar* scroll_control=new GLUI_Scrollbar(glui_groups.back(),"test",GLUI_SCROLL_VERTICAL,&value,GLUI_SCROLL_INT);

	//Set Alignment to Current Alignment
	scroll_control->set_alignment(glui_alignment);

	//Set Limits
	scroll_control->set_int_limits(min,max,GLUI_LIMIT_CLAMP);

	//Add to Scrollbars Vector
	glui_scrollbars.push_back(scroll_control);
}