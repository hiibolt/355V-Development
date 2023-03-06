#include "main.h"
#include "auton.h"


namespace AUTON{
    void windCatapult(){
        while(!stopSwitch.isPressed()){
            catapultMotor.moveVoltage(-10000);
            pros::delay(10);
        }
        catapultMotor.moveVoltage(0);
    }
    void shootCatapult(){
        while(stopSwitch.isPressed()){
            catapultMotor.moveVoltage(-10000);
            pros::delay(10);
        }
        catapultMotor.moveVoltage(0);
    }
    void runIntake(){
        intakeMotor.moveVoltage(-12000);
    }
    void runIntakeReverse(){
        intakeMotor.moveVoltage(12000);
    }
    void stopIntake(){
        intakeMotor.moveVoltage(0);
    }
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
                //shoot disks
                drive->turnAngle(50_deg);
                drive->moveDistance(36_in);
                drive->turnAngle(90_deg);
                // get read for preloads
                // shoot three times 
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
                // Get to roller
                drive->model().forward(0.1);
                runIntakeReverse();
                pros::delay(700);

                // Spin roller
                pros::delay(250);
                stopIntake();
                drive->stop();

                // Move backwards
                drive->setMaxVelocity(100);
                drive->moveDistance(-10_in);

                // Turn towards goal
                drive->setMaxVelocity(100);
                drive->turnAngle(-13_deg);

                // Shoot and Reset
                shootCatapult();
                windCatapult();

                // Turn towards 3-stack
                drive->setMaxVelocity(100);
                drive->turnAngle(-115_deg);

                // Drive to 3-stack
                drive->setMaxVelocity(300);
                drive->moveDistance(29_in);

                // Intake discs
                runIntake();
                drive->setMaxVelocity(50);
                drive->moveDistance(24_in);
                
                // Turn towards goal
                drive->setMaxVelocity(100);
                drive->turnAngle(85_deg);

                // Drive to half-court
                drive->setMaxVelocity(200);
                drive->moveDistance(-13_in);

                // Stop intake and let inertia settle
                stopIntake();
                pros::delay(250);

                // Shoot
                shootCatapult();
                windCatapult();
                break;
            case RIGHT_AUTON_ID:
                // Drive to roller
                drive->setMaxVelocity(125);
                drive->moveDistance(22_in);

                drive->setMaxVelocity(100);
                drive->turnAngle(90_deg);
                // go foward to spin roller
                runIntakeReverse();
                drive->moveDistanceAsync(8_in);
                int i = 0;
                while(!drive->isSettled()) {
                    if(i++ > 25) {
                        break;
                    }
                    pros::delay(20);
                }
                pros::delay(50);
                stopIntake();
                drive->stop();
                drive->moveDistance(-8_in);

                // Turn and shoot
                drive->turnAngle(11_deg);
                shootCatapult();
                windCatapult();
                drive->moveDistance(4_in);
                drive->turnAngle(132_deg);
                pros::delay(100);
                runIntake();
                drive->setMaxVelocity(200);
                drive->moveDistance(72_in);
                drive->turnAngle(-80_deg);
                stopIntake();
                drive->setMaxVelocity(200);
                drive->moveDistance(-12_in);
                pros::delay(50);
                shootCatapult();
                windCatapult();
               break;
	    }
		bandsPneumatic.set_value(HIGH);
        pros::delay(200);
		bandsPneumatic.set_value(LOW);
    }
}