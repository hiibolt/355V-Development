#include "guifuncs.h"
#include "main.h"
#include "led.h"
#include "auton.h"
#include "teamlogo.c"
#include <string.h>
#include <math.h>
using namespace LED;
using namespace GUI;
using namespace AUTON;

/** 			Constants 			**/
Controller controller;
auto imu = IMU(13, IMUAxes::z);

ADIButton stopSwitch('A');

Motor intakeMotor(19);
Motor catapultMotor(20);
MotorGroup leftDriveMotors({-8, 9, -10});
MotorGroup rightDriveMotors({3,-4,5});
MotorGroup leftDriveMotorsFlipped({8, -9, 10});
MotorGroup rightDriveMotorsFlipped({-3, 4, -5});
pros::Rotation cataRotation(18);
int rotation_threshold = 32800;
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
int currentAuton = RIGHT_AUTON_ID;
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

// Initiate drive definiton
float motorToWheelRatio = 1.0/1.0;
okapi::QLength wheelDiameter = 2.75_in;
okapi::QLength centerToCenterWheelTrack = 10.25_in;
std::shared_ptr<ChassisController> drive = 
    ChassisControllerBuilder()
        .withMotors(leftDriveMotors, rightDriveMotors)
        .withDimensions({AbstractMotor::gearset::blue, motorToWheelRatio}, {{wheelDiameter, centerToCenterWheelTrack}, imev5BlueTPR})
		.build();
std::shared_ptr<ChassisController> drive_flipped = 
    ChassisControllerBuilder()
        .withMotors(leftDriveMotorsFlipped, rightDriveMotorsFlipped)
        .withDimensions({AbstractMotor::gearset::blue, motorToWheelRatio}, {{wheelDiameter, centerToCenterWheelTrack}, imev5BlueTPR})
		.build();

std::shared_ptr<AsyncMotionProfileController> profileController =
  AsyncMotionProfileControllerBuilder()
    .withLimits({
      2.2, // Maximum linear velocity of the Chassis in m/s
      1.95, // Maximum linear acceleration of the Chassis in m/s/s
      10.0 // Maximum linear jerk of the Chassis in m/s/s/s
    })
    .withOutput(drive)
    .buildMotionProfileController();
std::shared_ptr<AsyncMotionProfileController> profileControllerFlipped =
  AsyncMotionProfileControllerBuilder()
    .withLimits({
      2.2, // Maximum linear velocity of the Chassis in m/s
      1.95, // Maximum linear acceleration of the Chassis in m/s/s
      10.0 // Maximum linear jerk of the Chassis in m/s/s/s
    })
    .withOutput(drive_flipped)
    .buildMotionProfileController();

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

	// Calibrate IMU
	std::cout << "Calibrating IMU...";
	imu.calibrate();
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

	// Build auton pathing
	std::cout << "Building Auton Paths [1/6]...";
	profileController->generatePath({
		{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
		{29_in, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
		"LEFT | 1 | Hit stack" // Profile name
	);
	profileController->generatePath({
		{0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
		{32_in, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
		"LEFT | 2 | Intake stack", // Profile name
		{
			0.45, // Maximum linear velocity of the Chassis in m/s
			1.95, // Maximum linear acceleration of the Chassis in m/s/s
			10.0 // Maximum linear jerk of the Chassis in m/s/s/s
		}
	);
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
	AUTON::runAuton(drive, currentAuton);
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
	while (true){
		global_tick ++;
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
		
		// Catapult Shooting/Winding (Thread safe)
		//if(shootButton.changedToPressed() && !shootingCata && stopSwitch.isPressed()){
		if(shootButton.changedToPressed() && !shootingCata && cataRotation.get_angle() >= 24400){;
			shootingCata = true;
			desiredCataIndicator = 0xA637A9;
		}
		//if(!shootingCata && !stopSwitch.isPressed()){
		if(!shootingCata && cataRotation.get_angle() < rotation_threshold){	
			catapultMotor.moveVoltage(-10000);
		}else if(!shootingCata){
			catapultMotor.moveVoltage(0);
			desiredCataIndicator = getCurrentColorHex();
		}
		if(shootingCata && cataRotation.get_angle() >= rotation_threshold){
			catapultMotor.moveVoltage(-12000);
		}else if(shootingCata && cataRotation.get_angle() < rotation_threshold){
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
		if(intakeButton.isPressed() && cataRotation.get_angle() > 25500){
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