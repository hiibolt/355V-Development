#include "guifuncs.h"
#include "main.h"
#include "led.h"
#include "auton.h"
#include "teamlogo.c"
#include <string.h>
#include <math.h>
#include "lemlib/api.hpp"
using namespace LED;
using namespace GUI;
using namespace AUTON;

/** 			Constants 			**/
Controller controller;

ADIButton stopSwitch('A');

Motor intakeMotor(19);
Motor catapultMotor(20);
pros::MotorGroup left_side_motors({-8, 9, -10});
pros::MotorGroup right_side_motors({3, -4, 5});
pros::Rotation cataRotation(18);
pros::IMU imu(13);
int endgamePneumaticState = LOW;
int bandsPneumaticState = LOW;
pros::ADIDigitalOut endgamePneumatic('B', endgamePneumaticState);
pros::ADIDigitalOut bandsPneumatic('C', bandsPneumaticState);

ControllerButton turnLeftButton(ControllerDigital::Y);
ControllerButton turnRightButton(ControllerDigital::A);
ControllerButton turn180Button(ControllerDigital::B);
ControllerButton driveForwardButton(ControllerDigital::X);

ControllerButton shootButton(ControllerDigital::R1);
ControllerButton outtakeButton(ControllerDigital::L2);
ControllerButton intakeButton(ControllerDigital::L1);

ControllerButton endgameButton(ControllerDigital::up);
ControllerButton bandsButton(ControllerDigital::down);

ControllerButton autonSwitchLeftButton(ControllerDigital::Y);
ControllerButton autonSwitchRightButton(ControllerDigital::A);
ControllerButton colorSwitchButton(ControllerDigital::X);

/**             Variables           **/
int currentDrive = EXPONENTIAL_DRIVE_ID;
int currentAuton = LEFT_AUTON_ID;
bool shootingCata = false;
int desiredCataIndicator = 0xBCB502;
int currentCataIndicator = 0x000000;
int tick = 500;
int global_tick = 0;

/**         Variable Modifiers      **/
void rotateDrive(){
	currentDrive = currentDrive == DRIVE_COUNT - 1 ? 0 : currentDrive + 1;
	GUI::updateDriveInfo(currentDrive,controller);
}
void setAuton(int auton_id){
	currentAuton = auton_id;
	GUI::updateAutonInfo(currentAuton,controller);
}

lemlib::Drivetrain_t drivetrain {
    &left_side_motors, // left drivetrain motors
    &right_side_motors, // right drivetrain motors
    10.25, // track width
    2.75, // wheel diameter
    600 // wheel rpm
};
lemlib::OdomSensors_t sensors {
    nullptr, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
    &imu // inertial sensor
};
lemlib::ChassisController_t lateral_controller {
    11, // kP
    65, // kD
    1, // smallErrorRange
    5000, // smallErrorTimeout
    3, // largeErrorRange
    5000, // largeErrorTimeout
};
lemlib::ChassisController_t angular_controller {
    16, // kP
    61, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    40 // slew rate
};
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Lamborghini color startup
	std::cout << "Starting color sequence...";
	LED::startupColors();
	std::cout << "Done" << std::endl;

	// Calibrate chassis
	std::cout << "Calibrating chassis...";
	chassis.calibrate();
	std::cout << "Done" << std::endl;

	// Build brain HUD
	std::cout << "Initializing brain HUD...";
	GUI::buildStyles();
	GUI::buildMainPage();
	GUI::buildAltPage();
	GUI::swapPage(HOME_PAGE_ID);
	std::cout << "Done" << std::endl;

	// Update controller HUD
	std::cout << "Updating controller HUD...";
	GUI::updateColorInfo(LED::getCurrentColorID(), controller);
	GUI::updateDriveInfo(currentDrive, controller);
	GUI::updateAutonInfo(currentAuton, controller);
	std::cout << "Done" << std::endl;

	// Enable catapult braking
	std::cout << "Configuring catapult...";
	catapultMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
	std::cout << "Done" << std::endl;

	// Flash green
	std::cout << "Flashing complete...";
	pros::delay(100);
	LED::updateColorStrips({0,43}, 0x15ff00);
	pros::delay(100);
	LED::updateColorStrips({0,43}, 0x00000);
	pros::delay(100);
	LED::updateColorStrips({0,43}, 0x15ff00);
	pros::delay(100);
	LED::updateColorStrips({0,43}, 0x00000);
	pros::delay(100);
	std::cout << "Done" << std::endl;

	// Alert Pilot
	std::cout << "Calibrating IMU...";
	controller.rumble(".");
	std::cout << "Done" << std::endl;

	std::cout << std::endl << "Initialized" << std::endl;
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
	chassis.setPose(0, 0, 0);
	chassis.moveTo(0, -10, 8000);
	//AUTON::runAuton(drive, currentAuton);
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
	global_tick = 0;
	chassis.setPose(0, 0, 0); 
	while (true){
		global_tick ++;
		if(global_tick % 100 == 0){
			lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        	std::cout << pose.x << std::endl; // print the x position
        	std::cout << pose.y << std::endl; // print the x position
        	std::cout << pose.theta << std::endl; // print the x position
            std::cout << std::endl;
		}
		switch (currentDrive){
			case TANK_DRIVE_ID:
				left_side_motors.move(controller.getAnalog(ControllerAnalog::leftY) * 127);
				right_side_motors.move(controller.getAnalog(ControllerAnalog::rightX) * 127);
				break;
			case EXPONENTIAL_DRIVE_ID:
				float leftStick = controller.getAnalog(ControllerAnalog::leftY) * 127;
				float rightStick = controller.getAnalog(ControllerAnalog::rightX) * 127;
				auto exponential = [&](float input){return (input/127) * std::pow(1.039,std::abs(input));};
				float velocity = exponential(leftStick);
				float turn     = exponential(rightStick); 

    			int left = velocity + turn;
    			int right = velocity - turn;
				left_side_motors.move(left);
				right_side_motors.move(right);
				break;
		}
		
		// Catapult Shooting/Winding (Thread safe)
		//if(shootButton.changedToPressed() && !shootingCata && stopSwitch.isPressed()){
		if(shootButton.changedToPressed() && !shootingCata && cataRotation.get_angle() >= 25800){;
			shootingCata = true;
			desiredCataIndicator = 0xA637A9;
		}
		//if(!shootingCata && !stopSwitch.isPressed()){
		if(!shootingCata && cataRotation.get_angle() < 25800){	
			catapultMotor.moveVoltage(-10000);
		}else if(!shootingCata){
			catapultMotor.moveVoltage(0);
			desiredCataIndicator = getCurrentColorHex();
		}
		if(shootingCata && cataRotation.get_angle() >= 25800){
			catapultMotor.moveVoltage(-12000);
		}else if(shootingCata && cataRotation.get_angle() < 25800){
			shootingCata = false;
			catapultMotor.moveVoltage(0);
		}

		// Catapult and endgame Notifications
		if(global_tick > 10500){
			if(global_tick % 75 == 0){
				LED::updateColorStrips({0,43}, desiredCataIndicator);
				pros::delay(10);
			}else if(global_tick % 25 == 0){
				LED::updateColorStrips({0,43}, 0x32CD32);
				pros::delay(10);
			}
		}else if(currentCataIndicator != desiredCataIndicator){
			LED::updateColorStrips({0,43}, desiredCataIndicator);
			currentCataIndicator = desiredCataIndicator;
		}

		// Endgame
		if(endgameButton.changedToPressed()){
			endgamePneumaticState = endgamePneumaticState == 1 ? 0 : 1;
			endgamePneumatic.set_value(endgamePneumaticState);
		}

		// Band Release
		if(bandsButton.changedToPressed()){
			bandsPneumaticState = bandsPneumaticState == 1 ? 0 : 1;
			bandsPneumatic.set_value(bandsPneumaticState);
		}

		// Selector Buttons
		if(autonSwitchLeftButton.changedToPressed()){
			setAuton(currentAuton == 0 ? 5 : currentAuton - 1);
		}else if(autonSwitchRightButton.changedToPressed()){
			setAuton(currentAuton == 5 ? 0 : currentAuton + 1);
		}
		if(colorSwitchButton.changedToPressed()){
			LED::cycleColor();
		}

		// Intake/Outtake Handling
		if(intakeButton.isPressed() && cataRotation.get_angle() < 24400){
            intakeMotor.moveVoltage(-12000);
        }else if(outtakeButton.isPressed()){
            intakeMotor.moveVoltage(12000);
        }else{
            intakeMotor.moveVoltage(0);
        }

	
    	// Wait and give up the time we don't need to other tasks.
    	// Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
    	pros::delay(10);
	}
}