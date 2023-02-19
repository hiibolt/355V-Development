#include "guifuncs.h"
#include "main.h"
#include "pid.h"
#include "led.h"
#include "teamlogo.c"
#include <string.h>
#include <math.h>

using namespace LED;
using namespace GUI;

/** 			Constants 			**/
Controller controller;
ADIButton stopSwitch('A');
MotorGroup leftMotors({-8, 9, -10});
MotorGroup rightMotors({3,-4,5});
ControllerButton turnLeftButton(ControllerDigital::Y);
ControllerButton turnRightButton(ControllerDigital::A);
ControllerButton turn180Button(ControllerDigital::B);
ControllerButton driveForwardButton(ControllerDigital::X);

ControllerButton shootButton(ControllerDigital::R1);
ControllerButton intakeButton(ControllerDigital::L1);
ControllerButton outtakeButton(ControllerDigital::L2);
ControllerButton endgameButton(ControllerDigital::up);

/**             Variables           **/
int currentDrive = CHEESY_DRIVE_ID;
int currentAuton = NONE_AUTON_ID;

/**         Variable Modifiers      **/
void rotateDrive(){
	currentDrive = currentDrive == DRIVE_COUNT - 1 ? 0 : currentDrive + 1;
	GUI::updateDriveInfo(currentDrive,getControllerObj());
}
int getCurrentDrive(){
	return currentDrive;
}
void setAuton(int auton_id){
	currentAuton = auton_id;
	GUI::updateAutonInfo(currentAuton,getControllerObj());
}
int getCurrentAuton(){
	return currentAuton;
}
Controller getControllerObj(){
	return controller;
}

// Initiate drive definiton
// fix trash code
std::shared_ptr<ChassisController> drive =
    ChassisControllerBuilder()
        .withMotors(leftMotors, rightMotors)
        .withDimensions({AbstractMotor::gearset::blue, 1.0/1.0}, {{2.75_in, 10.25_in}, imev5BlueTPR})
		.withMaxVelocity(600)
		.withGains(
			{PID::getConstant(PID::Distance,PID::Ki), PID::getConstant(PID::Distance,PID::Ki), PID::getConstant(PID::Distance,PID::Ki)},  // Distance PID
			{PID::getConstant(PID::Turn,PID::Ki), PID::getConstant(PID::Turn,PID::Ki), PID::getConstant(PID::Turn,PID::Ki)},              // Turn PID
			{PID::getConstant(PID::Angle,PID::Ki), PID::getConstant(PID::Angle,PID::Ki), PID::getConstant(PID::Angle,PID::Ki)}            // Turn PID
		)
		.build();

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	GUI::updateColorInfo(LED::getCurrentColorID(), controller);
	GUI::updateDriveInfo(currentDrive, controller);
	GUI::updateAutonInfo(currentAuton, controller);

	GUI::buildStyles();
	GUI::buildMainPage();
	GUI::buildPIDPage();
	GUI::swapPage(HOME_PAGE_ID);
	LED::startupColors();
	controller.rumble(".");
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
void autonomous() {
	//NONE_AUTON_ID,SKILLS_AUTON_ID,SHOOT_AUTON_ID,AWP_AUTON_ID,LEFT_AUTON_ID,RIGHT_AUTON_ID,AUTON_COUNT};
	switch(currentAuton){
		case NONE_AUTON_ID:
			// do absolutely nothing. nothing at all. do a thing? you die. a death. died. ead.
			break;
		case SKILLS_AUTON_ID:
			drive->turnAngle(45_deg);
			break;
		case SHOOT_AUTON_ID:
			break;
		case AWP_AUTON_ID:
			break;
		case LEFT_AUTON_ID:
			break;
		case RIGHT_AUTON_ID:
			break;
	}
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
		switch (currentDrive){
			case CHEESY_DRIVE_ID:
				drive->getModel()->curvature(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightX),0.05);	
				break;
			case TANK_DRIVE_ID:
				drive->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY), controller.getAnalog(ControllerAnalog::rightY),0.05);	
				break;
			case EXPONENTIAL_DRIVE_ID:
				float leftStick = controller.getAnalog(ControllerAnalog::leftY) * 127;
				float rightStick = controller.getAnalog(ControllerAnalog::rightX) * 127;
				auto exponential = [&](float input){return (input/127) * std::pow(1.039,std::abs(input));};
				float velocity = exponential(leftStick) / 127;
				float turn     = exponential(rightStick) / 127; 
				drive->getModel()->arcade(velocity, turn, 0.05);	
				break;
		}
		if(GUI::getPage()){
			if(driveForwardButton.isPressed()){
				drive->moveDistance(2_ft);
			}
			if(turnLeftButton.isPressed()){
				//drive->setMaxVelocity(500);
				drive->turnAngle(45_deg);
			}else if(turnRightButton.isPressed()){
				//drive->setMaxVelocity(500);
				drive->turnAngle(-45_deg);
			}else if(turn180Button.isPressed()){
				//drive->setMaxVelocity(500);
				drive->turnAngle(360_deg);
			}
		}
    	// Wait and give up the time we don't need to other tasks.
    	// Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
    	pros::delay(10);
	}
}