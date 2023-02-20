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
Motor intakeMotor(19);
Motor catapultMotor(20);
MotorGroup leftDriveMotors({-8, 9, -10});
MotorGroup rightDriveMotors({3,-4,5});
ControllerButton turnLeftButton(ControllerDigital::Y);
ControllerButton turnRightButton(ControllerDigital::A);
ControllerButton turn180Button(ControllerDigital::B);
ControllerButton driveForwardButton(ControllerDigital::X);

ControllerButton shootButton(ControllerDigital::R1);
ControllerButton outtakeButton(ControllerDigital::L1);
ControllerButton intakeButton(ControllerDigital::L2);
ControllerButton endgameButton(ControllerDigital::up);

ControllerButton autonSwitchLeftButton(ControllerDigital::Y);
ControllerButton autonSwitchRightButton(ControllerDigital::A);
ControllerButton colorSwitchButton(ControllerDigital::X);

/**             Variables           **/
int currentDrive = CHEESY_DRIVE_ID;
int currentAuton = NONE_AUTON_ID;
bool shootingCata = false;

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
float motorToWheelRatio = 1.0/1.0;
okapi::QLength wheelDiameter = 2.75_in;
okapi::QLength centerToCenterWheelTrack = 10.25_in;
std::shared_ptr<ChassisController> drive =
    ChassisControllerBuilder()
        .withMotors(leftDriveMotors, rightDriveMotors)
        .withDimensions({AbstractMotor::gearset::blue, motorToWheelRatio}, {{wheelDiameter, centerToCenterWheelTrack}, imev5BlueTPR})
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
	LED::startupColors();

	GUI::updateColorInfo(LED::getCurrentColorID(), controller);
	GUI::updateDriveInfo(currentDrive, controller);
	GUI::updateAutonInfo(currentAuton, controller);

	GUI::buildStyles();
	GUI::buildMainPage();
	GUI::buildPIDPage();
	GUI::swapPage(HOME_PAGE_ID);

	LED::updateColorStrips({0,43}, 0x15ff00);
	pros::delay(200);
	LED::updateColorStrips({0,43}, 0x00000);
	pros::delay(200);
	LED::updateColorStrips({0,43}, 0x15ff00);
	pros::delay(200);
	LED::updateColorStrips({0,43}, 0x00000);
	pros::delay(200);
	LED::updateColorStrips({0,43}, 0xff6400);
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
	switch(currentAuton){
		case NONE_AUTON_ID:
			// do absolutely nothing. nothing at all. do a thing? you die. a death. died. ead.
			break;
		case SKILLS_AUTON_ID:
			drive->moveDistance(5_in);
			drive->moveDistance(-8_in);
			//spin rollers 
			drive->turnAngle(-120_deg);
			drive->moveDistance(14_in);
			//turning to face disk on the line then intake it 
			drive->turnAngle(30_deg);
			drive->moveDistance(20_in);
			//spin roller 
			drive->moveDistance(12_in);
			//shoot three disks
			drive->turnAngle(45_deg);
			//intake while moving
			drive->moveDistance(62_in);
			drive->turnAngle(-90_deg);\
			//shoot
			drive->turnAngle(90_deg);
			//intake while moving
			drive->moveDistance(50_in);
			drive->turnAngle(-75_deg);
			//shoot  disks
			drive->turnAngle(50_deg);
			drive->moveDistance(36_in);
			drive->turnAngle(90_deg);
			// get read for preloads
			//shoot three times 
			drive->turnAngle(180_deg);
			drive->moveDistance(36_in);
			drive->turnAngle(90_deg);
			//lined up for roller 
			drive->moveDistance(12_in);
			//spin roller
			drive->moveDistance(-12_in);
			// this is the first half of the skills auton
			break;
		case SHOOT_AUTON_ID:
		//starting from left side
			drive->moveDistance(3_in);
			//spin roller
			drive->turnAngle(-45_deg);
			drive->moveDistance(6_in);
			//intake disk
			drive->moveDistance(-12_in);
			drive->turnAngle(-90_deg);
			drive->moveDistance(12_in);
			drive->turnAngle(-75_deg);
			//shoot the three disks
			drive->turnAngle(75_deg);
			//intake while moving
			drive->moveDistance(24_in);
			drive->turnAngle(-15_deg);
			//shoot disks
			drive->turnAngle(15_deg);
			//intake while moving
			drive->moveDistance(60_in);
			drive->turnAngle(90_deg);
			//shoot end auton
			break;
		case AWP_AUTON_ID:
			//starting from left side
			drive->moveDistance(3_in);
			//spin roller
			drive->turnAngle(-45_deg);
			drive->moveDistance(6_in);
			//intake disk
			drive->moveDistance(-12_in);
			drive->turnAngle(-90_deg);
			drive->moveDistance(12_in);
			drive->turnAngle(-75_deg);
			//shoot the three disks
			drive->turnAngle(75_deg);
			//intake while moving
			drive->moveDistance(24_in);
			drive->turnAngle(-15_deg);
			//shoot disks
			drive->turnAngle(15_deg);
			//intake while moving
			drive->moveDistance(60_in);
			//if we have time we can shoot idk if we would have time tho
			drive->turnAngle(45_deg);
			drive->moveDistance(12_in);
			//spin  roller
			break;
		case LEFT_AUTON_ID:
		//starting from left side
			drive->moveDistance(3_in);
			//spin roller
			drive->turnAngle(-45_deg);
			drive->moveDistance(6_in);
			//intake disk
			drive->moveDistance(-12_in);
			drive->turnAngle(-90_deg);
			drive->moveDistance(12_in);
			drive->turnAngle(-75_deg);
			//shoot the three disks
			drive->turnAngle(75_deg);
			//intake while moving
			drive->moveDistance(24_in);
			drive->turnAngle(-15_deg);
			break;
		case RIGHT_AUTON_ID:
			drive->moveDistance(36_in);
			//intake while moving
			drive->turnAngle(170_deg);
			//shoot three disks
			drive->turnAngle(-160_deg);
			drive->moveDistance(12_in);
			//intake while driving
			drive->moveDistance(12_in);
			drive->turnAngle(90_deg);
			//intake while moving
			drive->moveDistance(48_in);
			drive->turnAngle(270_deg);
			//shoot
			drive->turnAngle(-180_deg);
			drive->moveDistance(60_in);
			drive->turnAngle(45_deg);
			//spin roller
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

		// Catapult Shooting/Winding
		if(shootButton.isPressed() && !shootingCata && stopSwitch.isPressed()){
			shootingCata = true;
			LED::updateColorStrips({17,27}, 0x000dff);
		}
		if(!shootingCata && !stopSwitch.isPressed()){
			catapultMotor.moveVoltage(-10000);
		}else if(!shootingCata){
			catapultMotor.moveVoltage(0);
			LED::updateColorStrips({17,27}, 0x15ff00);
		}
		if(shootingCata && stopSwitch.isPressed()){
			catapultMotor.moveVoltage(-12000);
		}else if(shootingCata && !stopSwitch.isPressed()){
			shootingCata = false;
			catapultMotor.moveVoltage(0);
		}
		printf(shootingCata ? "0" : "1");

		// Selector Buttons
		if(autonSwitchLeftButton.isPressed()){
			setAuton(currentDrive == 0 ? 5 : currentDrive - 1);
		}else if(autonSwitchRightButton.isPressed()){
			setAuton(currentDrive == 5 ? 0 : currentDrive + 1);
		}
		if(colorSwitchButton.isPressed()){
			LED::cycleColor();
		}

		// Intake/Outtake Handling
		if(intakeButton.isPressed()){
			intakeMotor.moveVoltage(12000);
		}else if(outtakeButton.isPressed()){
			intakeMotor.moveVoltage(-12000);
		}else{
			intakeMotor.moveVoltage(0);
		}
		if(GUI::getPage() == PID_PAGE_ID){
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