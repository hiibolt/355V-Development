#include "main.h"
#include "guifuncs.h"
#include <string.h>
#include <math.h>

lv_obj_t * mainPage = lv_obj_create(NULL,NULL);
lv_obj_t * PIDPage = lv_obj_create (NULL,NULL);

lv_obj_t * HOME_buttonLeft;
lv_obj_t * HOME_buttonLeftLabel;
lv_obj_t * HOME_buttonRight;
lv_obj_t * HOME_buttonRightLabel;	
lv_obj_t * HOME_buttonAWP;
lv_obj_t * HOME_buttonAWPLabel;
lv_obj_t * HOME_buttonShoot;
lv_obj_t * HOME_buttonShootLabel;
lv_obj_t * HOME_buttonSkills;
lv_obj_t * HOME_buttonSkillsLabel;
lv_obj_t * HOME_buttonNothing;
lv_obj_t * HOME_buttonNothingLabel;
lv_obj_t * HOME_buttonDrive;
lv_obj_t * HOME_buttonDriveLabel;
lv_obj_t * HOME_buttonPID;
lv_obj_t * HOME_buttonPIDLabel;
lv_obj_t * HOME_buttonColor;
lv_obj_t * HOME_buttonColorLabel;
lv_obj_t * PID_buttonHome;
lv_obj_t * PID_buttonHomeLabel;

lv_obj_t * logo;

lv_style_t buttonActive;
lv_style_t buttonInactive;

static lv_res_t onClick(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    switch(id){
		case 8://PID Button
			lv_scr_load(PIDPage);
			break;
		case 9://Color Selector Button
			updateColor();
			break;
	}

    return LV_RES_OK;
}
void swapPage(int page){
	switch(page){
		case 0:
			lv_scr_load(mainPage);
			break;
	}
}
void createButton(const char * _text,lv_obj_t * _page, lv_align_t _alignto,int _x_offset, int _y_offset, int _width, int _height, int _id, lv_obj_t * _TEMPBUTTON, lv_obj_t * _TEMPBUTTONLABEL){
	_TEMPBUTTON = lv_btn_create(_page, NULL);// Bind the button to specified _page
	lv_btn_set_style(_TEMPBUTTON, LV_BTN_STYLE_PR,  &buttonActive); // Bind the PRESSED look
	lv_btn_set_style(_TEMPBUTTON, LV_BTN_STYLE_REL, &buttonInactive);// Bind the RELEASED look
    lv_obj_set_size(_TEMPBUTTON,_width,_height); // Set the button size
	lv_obj_align(_TEMPBUTTON,NULL,_alignto,_x_offset,_y_offset); //Align and offset position
	_TEMPBUTTONLABEL = lv_label_create(_TEMPBUTTON, NULL); //Create the label text
    lv_label_set_text(_TEMPBUTTONLABEL, _text); //Set the label text
	lv_obj_set_free_num(_TEMPBUTTON,_id); // Set the ID of the button
	lv_btn_set_action(_TEMPBUTTONLABEL, LV_BTN_ACTION_CLICK, onClick); // Bind master onClick
}
void buildStyles(){
	lv_style_copy(&buttonActive, &lv_style_plain);
    buttonActive.body.main_color = LV_COLOR_MAKE(100,100,100);
    buttonActive.body.grad_color = LV_COLOR_MAKE(100,100,100);
    buttonActive.body.radius = 0;
	buttonActive.body.border.color = LV_COLOR_MAKE(255,113,2);
	buttonActive.body.border.width = 3;
	buttonActive.text.color = LV_COLOR_MAKE(255,113,2);

	lv_style_copy(&buttonInactive, &lv_style_plain);
    buttonInactive.body.main_color = LV_COLOR_MAKE(0,0,0);
    buttonInactive.body.grad_color = LV_COLOR_MAKE(0,0,0);
    buttonInactive.body.radius = 0;
	buttonInactive.body.border.color = LV_COLOR_MAKE(255,113,2);
	buttonInactive.body.border.width = 3;
    buttonInactive.text.color = LV_COLOR_MAKE(255,113,2);
}
void buildMainPage() {
	createButton("Left",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,-5,130,60,1,HOME_buttonLeft,HOME_buttonLeftLabel);

	createButton("Right",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,-5,130,60,2,HOME_buttonRight,HOME_buttonRightLabel);

	createButton("AWP",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,58-5,130,60,3,HOME_buttonAWP,HOME_buttonAWPLabel);

	createButton("Shoot",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,58-5,130,60,4,HOME_buttonShoot,HOME_buttonShootLabel);

	createButton("Skills",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,58+58-5,130,60,5,HOME_buttonSkills,HOME_buttonSkillsLabel);

	createButton("Nothing",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,58+58-5,130,60,6,HOME_buttonNothing,HOME_buttonNothingLabel);

	createButton("Drive",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,58+58+58+10,130,60,7,HOME_buttonDrive,HOME_buttonDriveLabel);

	createButton("PID",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,58+58+58+10,130,60,8,HOME_buttonPID,HOME_buttonPIDLabel);

	createButton("Color",mainPage,LV_ALIGN_IN_BOTTOM_LEFT,65,10,130 * 0.75,60 * 0.75,9,HOME_buttonColor,HOME_buttonColorLabel);
}
void buildPIDPage() {
	createButton("Home",PIDPage,LV_ALIGN_IN_TOP_MID,0,-5,130,60,10,PID_buttonHome,PID_buttonHomeLabel);
}
