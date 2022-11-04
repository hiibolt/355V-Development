#include "main.h"
#include <string.h>
#include <math.h>

/** 			Constants 			**/
//Controller constructor
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

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();

	master.set_text(0,0,"Init Inertia Sensor");
	inertia.reset(true);

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

/**
	Accepts a pointer to a signed char voltage (-127-127)
		[Coincidentally the max/min voltage the motors accept]
	Sets that voltage to -30 or 30 respectively if it's close to zero
	This fixes non-functioning low power when it should be moving
**/
float voltageToPercent(int32_t voltage){
	return (float)voltage / 127;
}
void removeLowPowerIssues(signed char* value){
	if(*value < 30 && *value > 5){
		*value =  30;
	}else if(*value > -30 && *value < -5){
		*value =  -30;
	}
}
void removeLowPowerIssues(signed short* value){
	if(*value < 30 && *value > 5){
		*value =  30;
	}else if(*value > -30 && *value < -5){
		*value =  -30;
	}
}
void drive(float distance){
	//Distance is measured in CM, cry about it

}
void turn(double aim){
	while (std::round(abs(aim - inertia.get_rotation())) > 0.5){
		pros::delay(2);
		std::int32_t speed = 200 * (abs(aim - inertia.get_rotation()) / 100);
		if(aim < inertia.get_rotation()){
			//distance to = abs(aim - inertia.get_rotation())
			//higher distance = more turn
			//100 / distance
			left_mtr1.move(-speed);
			left_mtr2.move(-speed);
			left_mtr3.move(-speed);
			right_mtr1.move(speed);
			right_mtr2.move(speed);
			right_mtr3.move(speed);
		}else{
			left_mtr1.move(speed);
			left_mtr2.move(speed);
			left_mtr3.move(speed);
			right_mtr1.move(-speed);
			right_mtr2.move(-speed);
			right_mtr3.move(-speed);
		}
		master.set_text(0,0,std::to_string(aim) + " | " + std::to_string(inertia.get_rotation()));
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
	master.clear_line(0);
	/**
		0: Tank
		1: Split Arcade [High precision, low rotation speed]
		2: Split Arcade [High speed, low rotation speed]
		3: Split Arcade [RC Car-type controls, no point turns]
		4: Split Arcade [Scalar Split Arcade, Kind Medium]
	**/
	unsigned char driveType = 0;
	while (true) {
		//Autonmous Functions
		if(master.get_digital_new_press(DIGITAL_UP)){
			drive(100);
		}
		if(master.get_digital_new_press(DIGITAL_DOWN)){
			drive(-100);
		}
		if(master.get_digital_new_press(DIGITAL_LEFT)){
			turn(inertia.get_rotation() - 90);
		}
		if(master.get_digital_new_press(DIGITAL_RIGHT)){
			turn(inertia.get_rotation() + 90);
		}

		//Switching drive types
		if(master.get_digital_new_press(DIGITAL_A)){
			driveType ++;
		}
		if(master.get_digital_new_press(DIGITAL_B)){
			driveType = driveType == 0 ? 5 : driveType - 1;
		}

		//Will be printed later
		std::string info = "";
		switch(driveType){
			case 0: {
				//Tank
				info = "Tank";
				signed char left = master.get_analog(ANALOG_LEFT_Y);
				signed char right = master.get_analog(ANALOG_RIGHT_Y);
				removeLowPowerIssues(&left);
				removeLowPowerIssues(&right);

				left_mtr1.move(left);
				left_mtr2.move(left);
				left_mtr3.move(left);
				right_mtr1.move(right);
				right_mtr2.move(right);
				right_mtr3.move(right);
				break;
			}
			case 1: {
				//Split Arcade (High Turn, Half Speed)
				info = "Split Arcade [HT]";
				signed char left = (master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X)) / 2;
				signed char right = (master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X)) / 2;
				signed char left_delta = (master.get_analog(ANALOG_RIGHT_X) * (1 - abs(master.get_analog(ANALOG_LEFT_Y)) / 127)) / 2;
				signed char right_delta = (-master.get_analog(ANALOG_RIGHT_X) * (1 - abs(master.get_analog(ANALOG_LEFT_Y)) / 127)) / 2;
				removeLowPowerIssues(&left);
				removeLowPowerIssues(&right);

				left_mtr1.move(left + left_delta);
				left_mtr2.move(left + left_delta);
				left_mtr3.move(left + left_delta);
				right_mtr1.move(right + right_delta);
				right_mtr2.move(right + right_delta);
				right_mtr3.move(right + right_delta);
				break;
			}
			case 2: {
				//Split Arcade (High Speed, Half Turn)
				info = "Split Arcade [HS]";
				signed char left = (master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X)) / 2;
				signed char right = (master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X)) / 2;
				signed char up = (master.get_analog(ANALOG_LEFT_Y) * (1 - (abs(master.get_analog(ANALOG_LEFT_X)) / 127)) ) / 2;
				removeLowPowerIssues(&left);
				removeLowPowerIssues(&right);

				left_mtr1.move(left + up);
				left_mtr2.move(left + up);
				left_mtr3.move(left + up);
				right_mtr1.move(right + up);
				right_mtr2.move(right + up);
				right_mtr3.move(right + up);
				break;
			}
			case 3: {
				//Split Arcade (CAR)
				info = "Split Arcade [CAR]";
				signed char up = master.get_analog(ANALOG_LEFT_Y);
				float left_delta = master.get_analog(ANALOG_RIGHT_X) < 0 ? 1 - abs(master.get_analog(ANALOG_RIGHT_X)) / 127 : 1;
				float right_delta = master.get_analog(ANALOG_RIGHT_X) > 0 ? 1 - master.get_analog(ANALOG_RIGHT_X) / 127 : 1;
				removeLowPowerIssues(&up);

				left_mtr1.move(up * left_delta);
				left_mtr2.move(up * left_delta);
				left_mtr3.move(up * left_delta);
				right_mtr1.move(up * right_delta);
				right_mtr2.move(up * right_delta);
				right_mtr3.move(up * right_delta);
				break;
			}
			case 4: {
				//Split Arcade (Scalar)
				info = "Split Arcade [SC]";
				signed short left = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X);
				signed short right = master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X);
				if(abs(left) > 127){
					right = right * (127 / abs(left));
					left = left > 0 ? 127 : -127;
				}else if(abs(right) > 127){
					left = left * (127 / abs(right));
					right = right > 0 ? 127 : -127;
				}
				removeLowPowerIssues(&left);
				removeLowPowerIssues(&right);

				left_mtr1.move(left);
				left_mtr2.move(left);
				left_mtr3.move(left);
				right_mtr1.move(right);
				right_mtr2.move(right);
				right_mtr3.move(right);
				break;
			}
			case 5: {
				//Cheesy drive (Oli's favorite)
				info = "Cheesy Drive";
				float throttle = voltageToPercent(master.get_analog(ANALOG_LEFT_Y));
				float turn = voltageToPercent(master.get_analog(ANALOG_RIGHT_X)) * std::abs(throttle);
				float left = voltageToPercent(master.get_analog(ANALOG_RIGHT_X));
				float right = -voltageToPercent(master.get_analog(ANALOG_RIGHT_X));

				if(std::abs(throttle) > 0.01){
					left = throttle + turn;
					right = throttle - turn;
				}
				//removeLowPowerIssues(&left);
				//removeLowPowerIssues(&right);

				left_mtr1.move(left * 127);
				left_mtr2.move(left * 127);
				left_mtr3.move(left * 127);
				right_mtr1.move(right * 127);
				right_mtr2.move(right * 127);
				right_mtr3.move(right * 127);
				break;
			}
			default:
				driveType = 0;
		}

		//Pilot/Debug Information
		pros::lcd::set_text(0,"Drive Type: " + info);
		pros::lcd::set_text(1,"Rotation: " + std::to_string(inertia.get_rotation()) + " (deg)");


		master.set_text(0,0,info);

		//Delay to prevent code overflow
		pros::delay(2);
	}
}
