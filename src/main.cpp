#include "guifuncs.h"
#include "main.h"
#include "pid.h"
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
std::shared_ptr<ChassisController> drive =
    ChassisControllerBuilder()
        .withMotors(leftMotors, rightMotors)
        .withDimensions({AbstractMotor::gearset::blue, 60.0/36.0}, {{3.25_in, 11.9_in}, imev5BlueTPR})
		.withMaxVelocity(600)
		.withGains(
			{0.01, 0, 0},  // Distance PID
			{getConstant(CONSTANT_KD), getConstant(CONSTANT_KI), getConstant(CONSTANT_KD)}   // Turn     PID
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

/** LED Strips */
/*
 0 - Orange
 1 - Blue 
 2 - c h r i s t m a s
*/
int currentColor = 0;
pros::ADIDigitalOut LED_strip_1_brightness({18,1},1);
pros::ADILed LED_strip_1({18,1}, 44);
void cycleColor() {
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
			for(int i = 0;i < 43;i++){
				LED_strip_1[i] = 0xFF0000;
			}
			LED_strip_1.update();
			break;
		default:
			for(int i = 0;i < 43;i++){
				LED_strip_1[i] = 0xff6400;
			}
			LED_strip_1.update();
			currentColor = -1;
			break;
	}
}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	buildStyles();
	buildMainPage();
	buildPIDPage();
	swapPage(0);
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
