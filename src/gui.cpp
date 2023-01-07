#include "main.h"
#include "guifuncs.h"
#include <string.h>
#include <math.h>

lv_obj_t * mainPage = lv_obj_create(NULL,NULL);
lv_obj_t * PIDPage = lv_obj_create (NULL,NULL);

// Home Page
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

lv_obj_t * logo;

// PID Page
lv_obj_t * Kp_Label;
lv_obj_t * Ki_Label;
lv_obj_t * Kd_Label;
lv_obj_t * p_Label;
lv_obj_t * i_Label;
lv_obj_t * d_Label;

lv_obj_t * PID_buttonHome;
lv_obj_t * PID_buttonHomeLabel;

lv_obj_t * PID_Kp_p1Button;
lv_obj_t * PID_Kp_p1ButtonLabel;
lv_obj_t * PID_Ki_p1Button;
lv_obj_t * PID_Ki_p1ButtonLabel;
lv_obj_t * PID_Kd_p1Button;
lv_obj_t * PID_Kd_p1ButtonLabel;

lv_obj_t * PID_Kp_m1Button;
lv_obj_t * PID_Kp_m1ButtonLabel;
lv_obj_t * PID_Ki_m1Button;
lv_obj_t * PID_Ki_m1ButtonLabel;
lv_obj_t * PID_Kd_m1Button;
lv_obj_t * PID_Kd_m1ButtonLabel;



float PID_increment = 1;

lv_obj_t * PID_incrementLabel;
lv_obj_t * PID_x10Increment;
lv_obj_t * PID_x10IncrementLabel;
lv_obj_t * PID_div10Increment;
lv_obj_t * PID_div10IncrementLabel;

lv_style_t buttonActive;
lv_style_t buttonInactive;


char tempBuffer[20];
char * floatToChar(float input){
	std::snprintf(tempBuffer, 20, "%.6f", input);
	return tempBuffer;
}
static lv_res_t onClick(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    switch(id){
		case 8://PID Button
			// Switch to PID page
			swapPage(1);
			break;
		case 9://Color Selector Button
			updateColor();
			break;
		case 10:
			// Return home
			swapPage(0);
			break;
		case 11:
			setConstant((char *)"prev_Kp",getConstant((char *)"Kp"));
			setConstant((char *)"Kp",getConstant((char *)"Kp") + PID_increment);
    		lv_label_set_text(Kp_Label, floatToChar(getConstant((char *)"Kp")) ); //Set the label text
			break;
		case 12:
			setConstant((char *)"prev_Ki",getConstant((char *)"Ki"));
			setConstant((char *)"Ki",getConstant((char *)"Ki") + PID_increment);
    		lv_label_set_text(Ki_Label, floatToChar(getConstant((char *)"Ki")) ); //Set the label text
			break;
		case 13:
			setConstant((char *)"prev_Kd",getConstant((char *)"Kd"));
			setConstant((char *)"Kd",getConstant((char *)"Kd") + PID_increment);
    		lv_label_set_text(Kd_Label, floatToChar(getConstant((char *)"Kd")) ); //Set the label text
			break;
		case 14:
			setConstant((char *)"prev_Kp",getConstant((char *)"Kp"));
			setConstant((char *)"Kp",getConstant((char *)"Kp") - PID_increment);
    		lv_label_set_text(Kp_Label, floatToChar(getConstant((char *)"Kp")) ); //Set the label text
			break;
		case 15:
			setConstant((char *)"prev_Ki",getConstant((char *)"Ki"));
			setConstant((char *)"Ki",getConstant((char *)"Ki") - PID_increment);
    		lv_label_set_text(Ki_Label, floatToChar(getConstant((char *)"Ki")) ); //Set the label text
			break;
		case 16:
			setConstant((char *)"prev_Kd",getConstant((char *)"Kd"));
			setConstant((char *)"Kd",getConstant((char *)"Kd") - PID_increment);
    		lv_label_set_text(Kd_Label, floatToChar(getConstant((char *)"Kd")) ); //Set the label text
			break;
		case 17:
			PID_increment *= 10;
    		lv_label_set_text(PID_incrementLabel, floatToChar(PID_increment) ); //Set the label text
			break;
		case 18:
			PID_increment /= 10;
    		lv_label_set_text(PID_incrementLabel, floatToChar(PID_increment) ); //Set the label text
			break;
	}
	
    return LV_RES_OK;
}
void swapPage(int page){
	switch(page){
		case 0:
			lv_scr_load(mainPage);
			break;
		case 1:
			lv_scr_load(PIDPage);
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
	lv_btn_set_action(_TEMPBUTTON, LV_BTN_ACTION_CLICK, onClick); // Bind master onClick
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

	createButton("+",PIDPage,LV_ALIGN_IN_TOP_MID,-100,115,80,40,11,PID_Kp_p1Button,PID_Kp_p1ButtonLabel);
	createButton("+",PIDPage,LV_ALIGN_IN_TOP_MID,0,115,80,40,12,   PID_Ki_p1Button,PID_Ki_p1ButtonLabel);
	createButton("+",PIDPage,LV_ALIGN_IN_TOP_MID,100,115,80,40,13, PID_Ki_p1Button,PID_Kd_p1ButtonLabel);

	createButton("-",PIDPage,LV_ALIGN_IN_TOP_MID,-100,150,80,40,14,PID_Kp_m1Button,PID_Kp_m1ButtonLabel);
	createButton("-",PIDPage,LV_ALIGN_IN_TOP_MID,0,150,80,40,15,   PID_Ki_m1Button,PID_Ki_m1ButtonLabel);
	createButton("-",PIDPage,LV_ALIGN_IN_TOP_MID,100,150,80,40,16, PID_Kd_m1Button,PID_Kd_m1ButtonLabel);

	Kp_Label = lv_label_create(PIDPage, NULL); //Create the label text
	Ki_Label = lv_label_create(PIDPage, NULL); //Create the label text
	Kd_Label = lv_label_create(PIDPage, NULL); //Create the label text
	p_Label = lv_label_create(PIDPage, NULL); //Create the label text
	i_Label = lv_label_create(PIDPage, NULL); //Create the label text
	d_Label = lv_label_create(PIDPage, NULL); //Create the label text
	char * Kp = floatToChar(getConstant((char *)"Kp"));
    lv_label_set_text(Kp_Label, Kp); //Set the label text
	char * Ki = floatToChar(getConstant((char *)"Ki"));
    lv_label_set_text(Ki_Label, Ki); //Set the label text
	char * Kd = floatToChar(getConstant((char *)"Kd"));
    lv_label_set_text(Kd_Label, Kd); //Set the label text
	lv_obj_align(Kp_Label,NULL,LV_ALIGN_IN_TOP_MID,-100,90); //Align and offset position
	lv_obj_align(Ki_Label,NULL,LV_ALIGN_IN_TOP_MID,0,90); //Align and offset position
	lv_obj_align(Kd_Label,NULL,LV_ALIGN_IN_TOP_MID,100,90); //Align and offset position

    lv_label_set_text(p_Label, "Kp"); //Set the label text
	lv_label_set_text(i_Label, "Ki"); //Set the label text
	lv_label_set_text(d_Label, "Kd"); //Set the label text
	lv_obj_align(p_Label,NULL,LV_ALIGN_IN_TOP_MID,-100,70);
	lv_obj_align(i_Label,NULL,LV_ALIGN_IN_TOP_MID,0,70);
	lv_obj_align(d_Label,NULL,LV_ALIGN_IN_TOP_MID,100,70);
	

	PID_incrementLabel = lv_label_create(PIDPage,NULL);
    lv_label_set_text(PID_incrementLabel, floatToChar(PID_increment)); //Set the label text
	lv_obj_align(PID_incrementLabel,NULL,LV_ALIGN_IN_TOP_LEFT,3,5);

	createButton("+",PIDPage,LV_ALIGN_IN_TOP_LEFT,-5,30,80,40,17, PID_x10Increment,PID_x10IncrementLabel);
	createButton("-",PIDPage,LV_ALIGN_IN_TOP_LEFT,-5,65,80,40,18,PID_div10Increment,PID_div10IncrementLabel);
}
