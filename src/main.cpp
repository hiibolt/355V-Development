#include "main.h"
#include <string.h>
#include <math.h>

/** 			Constants 			**/
//Controller constructor
/**
pros::Controller master(pros::E_CONTROLLER_MASTER);
//Inertial Sensor
pros::Imu inertia(19);
//Motor Array
pros::Motor left_mtr1(1, true);
pros::Motor left_mtr2(4, true);
pros::Motor left_mtr3(5, true);
pros::Motor right_mtr1(8, false);
pros::Motor right_mtr2(9, false);
pros::Motor right_mtr3(10, false);
*/

// Chassis Controller - lets us drive the robot around with open- or closed-loop control
MotorGroup leftMotors = MotorGroup({-1, -4, -5});
MotorGroup rightMotors = MotorGroup({8, 9, 10});
const float Kp = 0.0020;
const float Ki = 0;
const float Kd = 0.00008;
std::shared_ptr<ChassisController> drive =
    ChassisControllerBuilder()
        .withMotors(leftMotors, rightMotors)
        .withDimensions({AbstractMotor::gearset::blue, 60.0/36.0}, {{3.25_in, 11.9_in}, imev5BlueTPR})
		.withMaxVelocity(600)
		.withGains(
			{0.01, 0, 0},  // Distance PID
			{Kp, Ki, Kd}   // Turn     PID
		)
		.build();
Controller controller;
ControllerButton turnLeftButton(ControllerDigital::A);
ControllerButton turnRightButton(ControllerDigital::B);
ControllerButton driveForwardButton(ControllerDigital::X);

//UI
/*
 - 0 = Home Page
 - 1 = Drive
 - 2 = PID
*/
lv_obj_t * mainPage = lv_obj_create(NULL,NULL);
lv_obj_t * PIDPage = lv_obj_create (NULL,NULL);

lv_obj_t * logo;

lv_style_t buttonActive;
lv_style_t buttonInactive;

/** LED Strips */
/*
 0 - Orange
 1 - Blue 
 2 - c h r i s t m a s
*/
int currentColor = 0;
pros::ADIDigitalOut LED_strip_1_brightness({18,1},1);
pros::ADILed LED_strip_1({18,1}, 44);
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

static lv_res_t onClick(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    switch(id){
		case 8://PID Button
			lv_scr_load(PIDPage);
			break;
		case 9://Color Selector Button
			currentColor ++;
			switch(currentColor) {
				case 0:
					for(int i = 0;i < 43;i++){
						LED_strip_1[i] = 0xff6400;
					}
					LED_strip_1.update();
					break;
				case 1:
					for(int i = 0;i < 43;i++){
						LED_strip_1[i] = 0x1745ff;
					}
					LED_strip_1.update();
					break;
				case 2:
					break;
				default:
					for(int i = 0;i < 43;i++){
						LED_strip_1[i] = 0xff6400;
					}
					LED_strip_1.update();
					currentColor = -1;
					break;
			}
			break;
	}

    return LV_RES_OK;
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
	lv_obj_t * HOME_buttonLeft;
	lv_obj_t * HOME_buttonLeftLabel;
	createButton("Left",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,-5,130,60,1,HOME_buttonLeft,HOME_buttonLeftLabel);

	lv_obj_t * HOME_buttonRight;
	lv_obj_t * HOME_buttonRightLabel;
	createButton("Right",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,-5,130,60,2,HOME_buttonRight,HOME_buttonRightLabel);

	lv_obj_t * HOME_buttonAWP;
	lv_obj_t * HOME_buttonAWPLabel;
	createButton("AWP",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,58-5,130,60,3,HOME_buttonAWP,HOME_buttonAWPLabel);

	lv_obj_t * HOME_buttonShoot;
	lv_obj_t * HOME_buttonShootLabel;
	createButton("Shoot",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,58-5,130,60,4,HOME_buttonShoot,HOME_buttonShootLabel);

	lv_obj_t * HOME_buttonSkills;
	lv_obj_t * HOME_buttonSkillsLabel;
	createButton("Skills",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,58+58-5,130,60,5,HOME_buttonSkills,HOME_buttonSkillsLabel);

	lv_obj_t * HOME_buttonNothing;
	lv_obj_t * HOME_buttonNothingLabel;
	createButton("Nothing",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,58+58-5,130,60,6,HOME_buttonNothing,HOME_buttonNothingLabel);

	lv_obj_t * HOME_buttonDrive;
	lv_obj_t * HOME_buttonDriveLabel;
	createButton("Drive",mainPage,LV_ALIGN_IN_TOP_RIGHT,-128+5,58+58+58+10,130,60,7,HOME_buttonDrive,HOME_buttonDriveLabel);

	lv_obj_t * HOME_buttonPID;
	lv_obj_t * HOME_buttonPIDLabel;
	createButton("PID",mainPage,LV_ALIGN_IN_TOP_RIGHT,5,58+58+58+10,130,60,8,HOME_buttonPID,HOME_buttonPIDLabel);

	lv_obj_t * HOME_buttonColor;
	lv_obj_t * HOME_buttonColorLabel;
	createButton("Color",mainPage,LV_ALIGN_IN_BOTTOM_LEFT,65,10,130 * 0.75,60 * 0.75,9,HOME_buttonColor,HOME_buttonColorLabel);
}
void buildPIDPage() {
	lv_obj_t * PID_buttonHome;
	lv_obj_t * PID_buttonHomeLabel;
	createButton("Home",PIDPage,LV_ALIGN_IN_TOP_MID,0,-5,130,60,10,PID_buttonHome,PID_buttonHomeLabel);
}
void initialize() {
	buildStyles();
	buildMainPage();
	buildPIDPage();
	lv_scr_load(mainPage);
	LED_strip_1.clear_all();
	for(int i = 0;i < 43;i++){
		LED_strip_1[i] = 0xff6400;
		LED_strip_1.update();
		pros::delay(40);
	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}
void turn(){
}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true){
		if(currentColor == 2){
			for(int i = 0;i < 43;i+=3){
				if(i % 2 == 0){
					LED_strip_1[i] = 0xed5026;
					LED_strip_1[i+1] = 0xed5026;
					LED_strip_1[i+2] = 0xed5026;
				}else{
					LED_strip_1[i] = 0x22f02f;
					LED_strip_1[i+1] = 0xed5026;
					LED_strip_1[i+2] = 0xed5026;
				}
			}
			LED_strip_1.update();
			pros::delay(300);
			for(int i = 0;i < 43;i+=3){
				if(i % 2 != 0){
					LED_strip_1[i] = 0xed5026;
					LED_strip_1[i+1] = 0xed5026;
					LED_strip_1[i+2] = 0xed5026;
				}else{
					LED_strip_1[i] = 0x22f02f;
					LED_strip_1[i+1] = 0xed5026;
					LED_strip_1[i+2] = 0xed5026;
				}
			}
			LED_strip_1.update();
			pros::delay(300);
		}
		// Cheesy Drive (for Oli)
    	drive->getModel()->curvature(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightX));
		if(driveForwardButton.isPressed()){
			drive->moveDistance(2_ft);
		}
		if(turnLeftButton.isPressed()){
			//drive->setMaxVelocity(500);
			drive->turnAngle(45_deg);
			drive->setMaxVelocity(600);
		}else if(turnRightButton.isPressed()){
			//drive->setMaxVelocity(500);
			drive->turnAngle(-45_deg);
			drive->setMaxVelocity(600);
		}
    	// Wait and give up the time we don't need to other tasks.
    	// Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
    	pros::delay(10);
	}
}
