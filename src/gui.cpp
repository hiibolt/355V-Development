#include "main.h"
#include "guifuncs.h"
#include "teamlogo.c"
#include "led.h"
#include <string.h>
#include <math.h>

int current_page = HOME_PAGE_ID;

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
lv_obj_t * HOME_buttonAlt;
lv_obj_t * HOME_buttonAltLabel;
lv_obj_t * HOME_buttonColor;
lv_obj_t * HOME_buttonColorLabel;

lv_obj_t * HOME_autonLabel;
lv_obj_t * HOME_driveLabel;

lv_obj_t * logo;
lv_obj_t * waltuh;

lv_obj_t * altPage = lv_obj_create (NULL,NULL);

lv_obj_t * ALT_buttonHome;
lv_obj_t * ALT_buttonHomeLabel;

lv_obj_t * ALT_buttonWaltuh;
lv_obj_t * ALT_buttonWaltuhLabel;

// General
lv_style_t buttonActive;
lv_style_t buttonInactive;
lv_style_t plaintext;


char tempBuffer[20];
char * floatToChar(float input){
	std::snprintf(tempBuffer, 20, "%.6f", input);
	return tempBuffer;
}
static lv_res_t onClick();
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
			switch(currentDrive){
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
		case ALT_BTN_ID:
			GUI::swapPage(ALT_PAGE_ID);
			break;
		case COLOR_BTN_ID:
			LED::cycleColor();
			break;
		case ALT_HOME_BTN_ID:
			GUI::swapPage(HOME_PAGE_ID);
			break;
	}
	
    return LV_RES_OK;
}
namespace GUI{
	void updateDriveInfo(int driveID, Controller controller){
		controller.clearLine(1);
		pros::delay(50);
		switch (driveID){
			case CHEESY_DRIVE_ID:
				controller.setText(1,0,"Drive  | CHEESY");
    			lv_label_set_text(HOME_driveLabel, "Cheesy");
				break;
			case TANK_DRIVE_ID:
				controller.setText(1,0,"Drive  | TANK");
    			lv_label_set_text(HOME_driveLabel, " Tank");
				break;
			case EXPONENTIAL_DRIVE_ID:
				controller.setText(1,0,"Drive  | EXPONENTIAL");
				lv_label_set_text(HOME_driveLabel, " Expo");
				break;
		}
	};
	void updateAutonInfo(int autonID, Controller controller){
		controller.clearLine(2);
		pros::delay(50);
		switch(autonID){
			case LEFT_AUTON_ID:
				controller.setText(2,0,"Auton | LEFT");
    			lv_label_set_text(HOME_autonLabel, "Left");
				break;
			case RIGHT_AUTON_ID:
				controller.setText(2,0,"Auton | RIGHT");
    			lv_label_set_text(HOME_autonLabel, "Right");
				break;
			case AWP_AUTON_ID:
				controller.setText(2,0,"Auton | AWP");
    			lv_label_set_text(HOME_autonLabel, "AWP");
				break;
			case SHOOT_AUTON_ID:
				controller.setText(2,0,"Auton | SHOOT");
    			lv_label_set_text(HOME_autonLabel, "Shoot");
				break;
			case SKILLS_AUTON_ID:
				controller.setText(2,0,"Auton | SKILLS");
    			lv_label_set_text(HOME_autonLabel, "Skills");
				break;
			case NONE_AUTON_ID:
				controller.setText(2,0,"Auton | NONE");
    			lv_label_set_text(HOME_autonLabel, "None");
				break;
		}
	};
	void updateColorInfo(int colorID, Controller controller){
		controller.clearLine(0);
		pros::delay(50);
		switch(colorID){
			case LED::ORANGE_ID:
				controller.setText(0,0,"Color  | ORANGE");
				break;
			case LED::BLUE_ID:
				controller.setText(0,0,"Color  | BLUE");
				break;
			case LED::RED_ID:
				controller.setText(0,0,"Color  | RED");
				break;
		}
	};
	void swapPage(int page){
		switch(page){
			case 0:
				lv_scr_load(mainPage);
				current_page = HOME_PAGE_ID;
				break;
			case 1:
				lv_scr_load(altPage);
				current_page = ALT_PAGE_ID;
				break;
		}
	}
	int getPage(){
		return current_page;
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
			text: "Other",
			page: mainPage,
			alignto: LV_ALIGN_IN_TOP_RIGHT,
			x_offset: 5,
			y_offset: 58+58+58+15,
			width: 130,
			height: 55,
			id: ALT_BTN_ID,
			btn_obj: HOME_buttonAlt,
			btn_label_obj: HOME_buttonAltLabel
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
	void buildAltPage() {
		static lv_style_t style;
		lv_style_copy(&style,lv_obj_get_style(lv_scr_act()));
		style.body.main_color = LV_COLOR_MAKE(0, 0, 0);
		style.body.grad_color = LV_COLOR_MAKE(0, 0, 0);
		lv_obj_set_style(altPage, &style);

		createButton({
			text: "Home",
			page: altPage,
			alignto: LV_ALIGN_IN_TOP_MID,
			x_offset: 0,
			y_offset: -5,
			width: 130,
			height: 60,
			id: ALT_HOME_BTN_ID,
			btn_obj: ALT_buttonHome,
			btn_label_obj: ALT_buttonHomeLabel
		});
	}
}