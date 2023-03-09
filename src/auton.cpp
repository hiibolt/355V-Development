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
        int i=0;
        switch(autonID){
            case NONE_AUTON_ID:
                // do absolutely nothing. nothing at all. do a thing? you die. a death. died. ead.
                break;
            case SKILLS_AUTON_ID:
                // Get to roller
                drive->model().forward(0.1);
                runIntakeReverse();

                // Spin first roller
                pros::delay(350);
                stopIntake();
                drive->stop();

                // Move backwards
                drive->setMaxVelocity(100);
                drive->moveDistance(-5_in);

                // Turn to first disk
                drive->turnAngle(155_deg);

                // Grab disk
                runIntake();
                drive->setMaxVelocity(150);
                drive->moveDistance(22_in);
                runIntakeReverse();

                // Turn to 2nd roller
                drive->setMaxVelocity(100);
                drive->turnAngle(-60_deg);
                pros::delay(200);

                // Drive to roller
                drive->setMaxVelocity(75);
                drive->moveDistanceAsync(12_in);

                // Spin second roller
                i = 0;
                while(!drive->isSettled()) {
                    if(i++ > 100) {
                        break;
                    }
                    pros::delay(20);
                }
                pros::delay(50);
                stopIntake();
                drive->stop();

                //driving back from the roller then turning to prepare the shooting
                drive->moveDistance(-8_in);
                drive->turnAngle(-98_deg);

                // Move to low goal
                drive->setMaxVelocity(300);
                drive->moveDistance(-52_in);

				// Let inertia settle
                pros::delay(50);

				// Shoot
                shootCatapult();
                windCatapult();

                //turning to the diaganial disks
                drive->turnAngle(-45_deg);
                runIntake();
				
                // Drive to diaganial disk
                drive->setMaxVelocity(200);
                drive->moveDistance(28_in);
                drive->turnAngle(-95_deg);

                // Drive to other disks
                drive->setMaxVelocity(200);
                drive->moveDistance(32_in);

				// Aim at goal
                drive->setMaxVelocity(100);
                drive->turnAngle(92_deg);

				// Get closer to goal and shoot
                drive->setMaxVelocity(200);
                drive->moveDistance(-6_in);

				// Let inertia settle
				stopIntake();
				pros::delay(50);

				// Shoot
                shootCatapult();
                windCatapult();


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
                drive->turnAngle(-11_deg);

                // Shoot and Reset
                shootCatapult();
                windCatapult();

                // Turn towards 3-stack
                drive->setMaxVelocity(100);
                drive->turnAngle(-119_deg);

                // Drive to 3-stack
                runIntakeReverse();
                drive->setMaxVelocity(300);
                drive->moveDistance(29_in);
                stopIntake();
                pros::delay(200);

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

                bandsPneumatic.set_value(HIGH);
                pros::delay(200);
                bandsPneumatic.set_value(LOW);
                drive->setMaxVelocity(600);
                drive->turnAngle(180_deg);
                break;
            case RIGHT_AUTON_ID:
                // Drive to roller
                drive->setMaxVelocity(200);
                drive->moveDistance(23_in);

                drive->setMaxVelocity(100);
                drive->turnAngle(90_deg);

                // go foward to spin roller
                runIntakeReverse();
                drive->setMaxVelocity(50);
                drive->moveDistanceAsync(8_in);
                i = 0;
                while(!drive->isSettled()) {
                    if(i++ > 25) {
                        break;
                    }
                    pros::delay(20);
                }
                pros::delay(50);
                stopIntake();
                drive->stop();

                drive->setMaxVelocity(300);
                drive->moveDistance(-10_in);

                // Turn and shoot
                drive->setMaxVelocity(100);
                drive->turnAngle(10_deg);
                shootCatapult();

                drive->moveDistanceAsync(6_in);
                windCatapult();
                drive->turnAngle(132_deg);
                pros::delay(50);

                runIntake();
                drive->setMaxVelocity(250);
                drive->moveDistance(72_in);
                drive->turnAngle(-82_deg);
                stopIntake();
                drive->setMaxVelocity(400);
                drive->moveDistance(-6_in);
                pros::delay(50);
                shootCatapult();
                windCatapult();

                bandsPneumatic.set_value(HIGH);
                pros::delay(200);
                bandsPneumatic.set_value(LOW);
                drive->setMaxVelocity(600);
                drive->turnAngle(180_deg);
               break;
	    }
    }
}