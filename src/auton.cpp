#include "main.h"
#include "auton.h"

void windCatapult(Motor catapultMotor, ADIButton stopSwitch){
    while(!stopSwitch.isPressed()){
		catapultMotor.moveVoltage(-10000);
        pros::delay(10);
	}
}
void shootCatapult(Motor catapultMotor, ADIButton stopSwitch){
    while(stopSwitch.isPressed()){
		catapultMotor.moveVoltage(-10000);
        pros::delay(10);
	}
}
void runIntake(Motor intakeMotor){
	intakeMotor.moveVoltage(12000);
}
void runIntakeReverse(Motor intakeMotor){
	intakeMotor.moveVoltage(-12000);
}
void stopIntake(Motor intakeMotor){
	intakeMotor.moveVoltage(0);
}
namespace AUTON{
    void runAuton(std::shared_ptr<ChassisController> drive, int autonID){
        switch(autonID){
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
                drive->turnAngle(-90_deg);
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
}