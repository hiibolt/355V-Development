#include "main.h"
#include "guifuncs.h"
#include "teamlogo.c"
#include "led.h"
#include <string.h>
#include <math.h>

// Home Page
lv_obj_t * mainPage = lv_obj_create(NULL,NULL);

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

lv_obj_t * HOME_autonLabel;
lv_obj_t * HOME_driveLabel;

lv_obj_t * logo;

// PID Page
float PID_increment = 1;
int current_PID_type = 0;

lv_obj_t * PIDPage = lv_obj_create (NULL,NULL);

lv_obj_t * Kp_Label;
lv_obj_t * Ki_Label;
lv_obj_t * Kd_Label;
lv_obj_t * p_Label;
lv_obj_t * i_Label;
lv_obj_t * d_Label;
lv_obj_t * PID_type_label;

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

lv_obj_t * PID_incrementLabel;
lv_obj_t * PID_x10Increment;
lv_obj_t * PID_x10IncrementLabel;
lv_obj_t * PID_div10Increment;
lv_obj_t * PID_div10IncrementLabel;

// General
lv_style_t buttonActive;
lv_style_t buttonInactive;
lv_style_t plaintext;


char tempBuffer[20];
char * floatToChar(float input){
	std::snprintf(tempBuffer, 20, "%.6f", input);
	return tempBuffer;
}
static lv_res_t onClick(lv_obj_t * btn)
{/**
		If you wish to add a new button, 
		 1.) Create a new button and button label object
		 2.) Use the createStandardButton or createImgButton function
		 3.) Add a new constant for the button to the guifuncs.h enum
		 4.) Add a new entry to the below switch

		 PS: If you get stuck on step 2, you need to brush up on data types before you start working.
		 	 I highly recommend reading up on structs and enums, they're used quite a lot.
	*/
    switch(lv_obj_get_free_num(btn)){
		case LEFT_BTN_ID:
			setAuton(LEFT_AUTON_ID);
    		lv_label_set_text(HOME_autonLabel, "Left");
			break;
		case RIGHT_BTN_ID:
			setAuton(RIGHT_AUTON_ID);
    		lv_label_set_text(HOME_autonLabel, "Right");
			break;
		case AWP_BTN_ID:
			setAuton(AWP_AUTON_ID);
    		lv_label_set_text(HOME_autonLabel, "AWP");
			break;
		case SHOOT_BTN_ID:
			setAuton(SHOOT_AUTON_ID);
    		lv_label_set_text(HOME_autonLabel, "Shoot");
			break;
		case SKILLS_BTN_ID:
			setAuton(SKILLS_AUTON_ID);
    		lv_label_set_text(HOME_autonLabel, "Skills");
			break;
		case NOTHING_BTN_ID:
			setAuton(NONE_AUTON_ID);
    		lv_label_set_text(HOME_autonLabel, "None");
			break;
		case DRIVE_BTN_ID:
			rotateDrive();
			switch(getCurrentDrive()){
				case CHEESY_DRIVE_ID:
    				lv_label_set_text(HOME_driveLabel, "Cheesy");
					break;
				case TANK_DRIVE_ID:
    				lv_label_set_text(HOME_driveLabel, " Tank");
					break;
				case EXPONENTIAL_DRIVE_ID:
					lv_label_set_text(HOME_driveLabel, " Expo");
					break;
			}
			break;
		case PID_BTN_ID:
			GUI::swapPage(PID_PAGE_ID);
			break;
		case COLOR_BTN_ID:
			LED::cycleColor();
			break;
		case PID_HOME_BTN_ID:
			GUI::swapPage(HOME_PAGE_ID);
			break;
		case PID_KP_PLUS_BTN_ID:
			PID::setConstant(current_PID_type,PID::Kp,PID::getConstant(current_PID_type,PID::Kp) + PID_increment);
    		lv_label_set_text(Kp_Label, floatToChar(PID::getConstant(current_PID_type,PID::Kp)) ); //Set the label text
			break;
		case PID_KI_PLUS_BTN_ID:
			setConstant(current_PID_type,PID::Ki,PID::getConstant(current_PID_type,PID::Ki) + PID_increment);
    		lv_label_set_text(Ki_Label, floatToChar(PID::getConstant(current_PID_type,PID::Ki)) ); //Set the label text
			break;
		case PID_KD_PLUS_BTN_ID:
			setConstant(current_PID_type,PID::Kd,PID::getConstant(current_PID_type,PID::Kd) + PID_increment);
    		lv_label_set_text(Kd_Label, floatToChar(PID::getConstant(current_PID_type,PID::Kd)) ); //Set the label text
			break;
		case PID_KP_MINUS_BTN_ID:
			setConstant(current_PID_type,PID::Kp,PID::getConstant(current_PID_type,PID::Kp) - PID_increment);
    		lv_label_set_text(Kp_Label, floatToChar(PID::getConstant(current_PID_type,PID::Kp)) ); //Set the label text
			break;
		case PID_KI_MINUS_BTN_ID:
			setConstant(current_PID_type,PID::Ki,PID::getConstant(current_PID_type,PID::Ki) - PID_increment);
    		lv_label_set_text(Ki_Label, floatToChar(PID::getConstant(current_PID_type,PID::Ki)) ); //Set the label text
			break;
		case PID_KD_MINUS_BTN_ID:
			setConstant(current_PID_type,PID::Kd,PID::getConstant(current_PID_type,PID::Kd) - PID_increment);
    		lv_label_set_text(Kd_Label, floatToChar(PID::getConstant(current_PID_type,PID::Kd)) ); //Set the label text
			break;
		case PID_INCREMENT_PLUS_BTN_ID:
			PID_increment *= 10;
    		lv_label_set_text(PID_incrementLabel, floatToChar(PID_increment) ); //Set the label text
			break;
		case PID_INCREMENT_MINUS_BTN_ID:
			PID_increment /= 10;
    		lv_label_set_text(PID_incrementLabel, floatToChar(PID_increment) ); //Set the label text
			break;
		case PID_TYPE_SWAP_BTN_ID:
			current_PID_type = current_PID_type == 2 ? 0 : current_PID_type + 1;
    		lv_label_set_text(Kp_Label, floatToChar(PID::getConstant(current_PID_type,PID::Kp)) ); //Set the label text
    		lv_label_set_text(Ki_Label, floatToChar(PID::getConstant(current_PID_type,PID::Ki)) ); //Set the label text
    		lv_label_set_text(Kd_Label, floatToChar(PID::getConstant(current_PID_type,PID::Kd)) ); //Set the label text
			lv_label_set_text(PID_type_label, PID::getPIDTypeName(current_PID_type)); //Set the label text
			break;
	}
	
    return LV_RES_OK;
}
void createButton(standard_button_options options){
	options.btn_obj = lv_btn_create(options.page, NULL);// Bind the button to specified _page
	lv_btn_set_style(options.btn_obj, LV_BTN_STYLE_PR,  &buttonActive); // Bind the PRESSED look
	lv_btn_set_style(options.btn_obj, LV_BTN_STYLE_REL, &buttonInactive);// Bind the RELEASED look
    lv_obj_set_size(options.btn_obj, options.width, options.height); // Set the button size
	lv_obj_align(options.btn_obj,NULL, options.alignto, options.x_offset, options.y_offset); //Align and offset position
	options.btn_label_obj = lv_label_create(options.btn_obj, NULL); //Create the label text
    lv_label_set_text(options.btn_label_obj, options.text); //Set the label text
	lv_obj_set_free_num(options.btn_obj,options.id); // Set the ID of the button
	lv_btn_set_action(options.btn_obj, LV_BTN_ACTION_CLICK, onClick); // Bind master onClick
}
void createImgButton(img_button_options options){
	options.btn_obj = lv_imgbtn_create(options.page, NULL);
    lv_imgbtn_set_src(options.btn_obj, LV_BTN_STATE_REL, options.src);
    lv_imgbtn_set_src(options.btn_obj, LV_BTN_STATE_PR, options.src);
    lv_imgbtn_set_src(options.btn_obj, LV_BTN_STATE_TGL_REL, options.src);
    lv_imgbtn_set_src(options.btn_obj, LV_BTN_STATE_TGL_PR, options.src);
	lv_obj_align(options.btn_obj,NULL,options.alignto,20,0);
	lv_obj_set_free_num(options.btn_obj,options.id);
	lv_btn_set_action(options.btn_obj, LV_BTN_ACTION_CLICK, onClick); // Bind master onClick
}
namespace GUI{
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

		lv_style_copy(&plaintext, &lv_style_plain);
		plaintext.text.color = LV_COLOR_MAKE(255,121,0);
	}
	void buildMainPage() {
		static lv_style_t style;
		lv_style_copy(&style,lv_obj_get_style(lv_scr_act()));
		style.body.main_color = LV_COLOR_MAKE(0, 0, 0);
		style.body.grad_color = LV_COLOR_MAKE(0, 0, 0);
		lv_obj_set_style(mainPage, &style);


		createImgButton({
			src: &teamlogo_50x50, 
			page: mainPage, 
			alignto: LV_ALIGN_IN_LEFT_MID, 
			x_offset: 20, 
			y_offset: 0, 
			id: COLOR_BTN_ID, 
			btn_obj: logo
		});

		createButton({
			text: "Left",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: 5,
			y_offset: -5,
			width: 130,
			height: 55,
			id: LEFT_BTN_ID,
			btn_obj: HOME_buttonLeft,
			btn_label_obj: HOME_buttonLeftLabel
		});
		createButton({
			text: "Right",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: -128+5,
			y_offset: -5,
			width: 130,
			height: 55,
			id: RIGHT_BTN_ID,
			btn_obj: HOME_buttonRight,
			btn_label_obj: HOME_buttonRightLabel
		});
		createButton({
			text: "AWP",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: -128+5,
			y_offset: 48-5,
			width: 130,
			height: 55,
			id: AWP_BTN_ID,
			btn_obj: HOME_buttonAWP,
			btn_label_obj: HOME_buttonAWPLabel
		});
		createButton({
			text: "Shoot",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: 5,
			y_offset: 48-5,
			width: 130,
			height: 55,
			id: SHOOT_BTN_ID,
			btn_obj: HOME_buttonShoot,
			btn_label_obj: HOME_buttonShootLabel
		});
		createButton({
			text: "Skills",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: -128+5,
			y_offset: 48+48-5,
			width: 130,
			height: 55,
			id: SKILLS_BTN_ID,
			btn_obj: HOME_buttonSkills,
			btn_label_obj: HOME_buttonSkillsLabel
		});
		createButton({
			text: "Nothing",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: 5,
			y_offset: 48+48-5,
			width: 130,
			height: 55,
			id: NOTHING_BTN_ID,
			btn_obj: HOME_buttonNothing,
			btn_label_obj: HOME_buttonNothingLabel
		});

		createButton({
			text: "Drive",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: -128+5,
			y_offset: 58+58+58+15,
			width: 130,
			height: 55,
			id: DRIVE_BTN_ID,
			btn_obj: HOME_buttonDrive,
			btn_label_obj: HOME_buttonDriveLabel
		});
		createButton({
			text: "PID",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: 5,
			y_offset: 58+58+58+15,
			width: 130,
			height: 55,
			id: PID_BTN_ID,
			btn_obj: HOME_buttonPID,
			btn_label_obj: HOME_buttonPIDLabel
		});
		HOME_autonLabel = lv_label_create(mainPage, NULL);
		HOME_driveLabel = lv_label_create(mainPage, NULL);
		lv_label_set_text(HOME_autonLabel, "None");
		lv_label_set_text(HOME_driveLabel, "Cheesy");
		lv_obj_align(HOME_autonLabel,NULL,LV_ALIGN_IN_RIGHT_MID,-170,46.5);
		lv_obj_align(HOME_driveLabel,NULL,LV_ALIGN_IN_RIGHT_MID,-28,46.5);
		lv_label_set_style(HOME_autonLabel,&plaintext);
		lv_label_set_style(HOME_driveLabel,&plaintext);
	}
	void buildPIDPage() {
		createButton({
			text: "Home",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: 0,
			y_offset: -5,
			width: 130,
			height: 60,
			id: PID_HOME_BTN_ID,
			btn_obj: PID_buttonHome,
			btn_label_obj: PID_buttonHomeLabel
		});

		createButton({
			text: "+",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: -100,
			y_offset: 115,
			width: 80,
			height: 40,
			id: PID_KP_PLUS_BTN_ID,
			btn_obj: PID_Kp_p1Button,
			btn_label_obj: PID_Kp_p1ButtonLabel
		});
		createButton({
			text: "+",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: 0,
			y_offset: 115,
			width: 80,
			height: 40,
			id: PID_KI_PLUS_BTN_ID,
			btn_obj: PID_Ki_p1Button,
			btn_label_obj: PID_Ki_p1ButtonLabel
		});
		createButton({
			text: "+",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: 100,
			y_offset: 115,
			width: 80,
			height: 40,
			id: PID_KD_PLUS_BTN_ID,
			btn_obj: PID_Kd_p1Button,
			btn_label_obj: PID_Kd_p1ButtonLabel
		});
		createButton({
			text: "-",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: -100,
			y_offset: 150,
			width: 80,
			height: 40,
			id: PID_KP_MINUS_BTN_ID,
			btn_obj: PID_Kp_m1Button,
			btn_label_obj: PID_Kp_m1ButtonLabel
		});
		createButton({
			text: "-",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: 0,
			y_offset: 150,
			width: 80,
			height: 40,
			id: PID_KI_MINUS_BTN_ID,
			btn_obj: PID_Ki_m1Button,
			btn_label_obj: PID_Ki_m1ButtonLabel
		});
		createButton({
			text: "-",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: 100,
			y_offset: 150,
			width: 80,
			height: 40,
			id: PID_KD_MINUS_BTN_ID,
			btn_obj: PID_Kd_m1Button,
			btn_label_obj: PID_Kd_m1ButtonLabel
		});

		Kp_Label = lv_label_create(PIDPage, NULL); //Create the label text
		Ki_Label = lv_label_create(PIDPage, NULL); //Create the label text
		Kd_Label = lv_label_create(PIDPage, NULL); //Create the label text
		p_Label = lv_label_create(PIDPage, NULL); //Create the label text
		i_Label = lv_label_create(PIDPage, NULL); //Create the label text
		d_Label = lv_label_create(PIDPage, NULL); //Create the label text
		char * Kp = floatToChar(PID::getConstant(current_PID_type,PID::Kp));
		lv_label_set_text(Kp_Label, Kp); //Set the label text
		char * Ki = floatToChar(PID::getConstant(current_PID_type,PID::Ki));
		lv_label_set_text(Ki_Label, Ki); //Set the label text
		char * Kd = floatToChar(PID::getConstant(current_PID_type,PID::Kd));
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

		createButton({
			text: "+",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_LEFT,
			x_offset: -5,
			y_offset: 30,
			width: 80,
			height: 40,
			id: PID_INCREMENT_PLUS_BTN_ID,
			btn_obj: PID_x10Increment,
			btn_label_obj: PID_x10IncrementLabel,
		});
		createButton({
			text: "-",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_LEFT,
			x_offset: -5,
			y_offset: 65,
			width: 80,
			height: 40,
			id: PID_INCREMENT_MINUS_BTN_ID,
			btn_obj: PID_div10Increment,
			btn_label_obj: PID_div10IncrementLabel,
		});
		
		createButton({
			text: "",
			page: PIDPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: 5,
			y_offset: 30,
			width: 100,
			height: 40,
			id: PID_TYPE_SWAP_BTN_ID,
			btn_obj: PID_x10Increment,
			btn_label_obj: PID_x10IncrementLabel,
		});

		PID_type_label = lv_label_create(PIDPage, NULL);//Create the label text
		lv_label_set_text(PID_type_label, PID::getPIDTypeName(current_PID_type)); //Set the label text
		lv_obj_align(PID_type_label,NULL,LV_ALIGN_IN_TOP_RIGHT,-8,40);
	}
}