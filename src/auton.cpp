#include "main.h"
#include "auton.h"


namespace AUTON{
    void windCatapult(){
        while (cataRotation.get_angle() < rotation_threshold){
            catapultMotor.moveVoltage(-10000);
            pros::delay(10);
            global_tick++;
        }
        catapultMotor.moveVoltage(0);
    }
    void shootCatapult(){
        while (cataRotation.get_angle() >= rotation_threshold){
            catapultMotor.moveVoltage(-10000);
            pros::delay(10);
            global_tick++;
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
    void fireEndgame(){
		endgamePneumatic.set_value(HIGH);
    }
    void autonLeftCommon(std::shared_ptr<ChassisController> drive){
        std::cout << "autonLeftCommon\n";

        windCatapult();

        // Get to roller
        runIntakeReverse();
        drive->setMaxVelocity(50);
        drive->moveDistanceAsync(8_in);
        int i = 0;
        while(!drive->isSettled()) {
            if(i++ > 20) {
                break;
            }
            pros::delay(15);
        }
        pros::delay(50);
        stopIntake();
        drive->stop();

        // Move and aim at towards goal
        drive->setMaxVelocity(200);
        drive->moveDistance(-6_in);
        drive->turnAngleAsync(-9_deg);

        drive->waitUntilSettled();

        // Shoot and Reset
        shootCatapult();

        // Turn towards 3-stack
        drive->setMaxVelocity(200);
        drive->turnAngleAsync(-130_deg);
        catapultMotor.moveVoltage(-10000);
        drive->waitUntilSettled();
        catapultMotor.moveVoltage(0);

        // Drive to 3-stack
        drive->setMaxVelocity(250);
        drive->moveDistanceAsync(27_in);
        windCatapult();
        drive->waitUntilSettled();

        // Intake discs
        runIntake();
        drive->setMaxVelocity(210);
        drive->moveDistance(32_in);
        
        // Turn towards goal
        drive->setMaxVelocity(200);
        drive->turnAngleAsync(85_deg);
        drive->waitUntilSettled();

        // Drive to half-court and let inertia settle
        drive->setMaxVelocity(200);
        drive->moveDistanceAsync(-3_in);
        stopIntake();
        drive->waitUntilSettled();
        
        shootCatapult();

    }
    void runAuton(std::shared_ptr<ChassisController> drive, int autonID){
        intakeMotor.moveVoltage(0);
        int i=0;
        std::cout << "autonID: " << autonID << "\n";
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
                drive->moveDistance(24_in);
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
                drive->turnAngle(-96_deg);

                // Move to low goal
                drive->setMaxVelocity(300);
                drive->moveDistance(-48_in);

				// Let inertia settle
                pros::delay(50);

				// Shoot
                shootCatapult();
                windCatapult();

                //turning to the diaganial disks
                drive->turnAngle(-42_deg);
                runIntake();
				
                // Drive to diaganial disk
                drive->setMaxVelocity(200);
                drive->moveDistance(30_in);
                drive->setMaxVelocity(150);
                drive->turnAngle(-93_deg);

                // Drive to other disks
                drive->setMaxVelocity(100);
                drive->moveDistance(30_in);

				// Aim at goal
                drive->setMaxVelocity(150);
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
                //back up from shooting
                drive->setMaxVelocity(200);
                drive->moveDistance(2_in);
                drive->turnAngle(135_deg);
                runIntake();
                drive->setMaxVelocity(150);
                drive->moveDistance(24_in);
                drive->setMaxVelocity(100);
                drive->moveDistance(-24_in);
                drive->turnAngle(-136_deg);

                shootCatapult();
                windCatapult();
                drive->turnAngle(-135_deg);
                runIntake();
                drive->setMaxVelocity(150);
                drive->moveDistance(24_in);
                drive->setMaxVelocity(100);
                drive->moveDistance(-24_in);
                drive->turnAngle(136_deg);
                shootCatapult();
                windCatapult();
                
                
                //turning to go towards the triple stack
                //drive->turnAngle(-92_deg);
                //runIntake();
                // driving towards the triple stack
                //drive->setMaxVelocity(150);
                //drive->moveDistance(36_in);
                //turning to get ready to shoot the next 3 shot
                //drive->turnAngle(135_deg);
                //drive to the low goal
                //drive->moveDistance(-52_in);
                //stopIntake();
                //pros::delay(50);
                //shootCatapult();
                //windCatapult();



				// Back off to avoid clipping on the turn
                drive->setMaxVelocity(200);
                drive->moveDistance(2_in);

                // Aim at triple disk set
                drive->setMaxVelocity(200);
                drive->turnAngle(143_deg);

                // Drive to and intake triple disks
                runIntake();
                drive->setMaxVelocity(30);
                drive->moveDistance(26_in);
                stopIntake();

                // Let inertia settle
                pros::delay(50);

                // Drive back to shooting corner
                drive->setMaxVelocity(300);
                drive->moveDistance(-26_in);

                // Aim at barrier goal
                drive->setMaxVelocity(100);
                drive->turnAngle(-145_deg);

                // Move at shooting corner
                drive->setMaxVelocity(200);
                drive->moveDistance(-3_in);

                // Let inertia settle
                pros::delay(50);

                // Shoot
                shootCatapult();
                windCatapult();

                // Return to corner
                drive->setMaxVelocity(200);
                drive->moveDistance(3_in);

                // Turn to triple disc at barrier
                drive->setMaxVelocity(100);
                drive->turnAngle(-152_deg);

                // Drive to and intake triple disks
                runIntake();
                drive->setMaxVelocity(30);
                drive->moveDistance(24_in);
                stopIntake();

                // Drive back to shooting corner
                drive->setMaxVelocity(300);
                drive->moveDistance(-24_in);

                // Aim at barrier goal
                drive->setMaxVelocity(100);
                drive->turnAngle(135_deg);

                // Let inertia settle
                pros::delay(50);

                // Shoot
                shootCatapult();
                windCatapult();

                // Cross field and get first disk
                runIntake();
                drive->setMaxVelocity(400);
                drive->moveDistance(30_in);

                // Turn to other disks
                drive->setMaxVelocity(300);
                drive->turnAngle(-90_deg);

                // Get other two disks
                drive->setMaxVelocity(400);
                drive->moveDistance(68_in);

                // Turn to other disks
                stopIntake();
                fireEndgame();
                //drive->setMaxVelocity(200);
                //drive->turnAngle(-45_deg);


                /**
                // Grab roller
                drive->model().forward(0.1);
                runIntakeReverse();

                // Spin first roller
                pros::delay(350);
                stopIntake();
                drive->stop();
                **/
                break;
            case SHOOT_AUTON_ID:
                // CURRENTLY ACTING AS ALTERNATIVE RIGHT
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
                    if(i++ > 18) {
                        break;
                    }
                    pros::delay(20);
                }
                pros::delay(50);
                stopIntake();
                drive->stop();

                // Square on roller
                drive->model().forward(0.5);
                pros::delay(350);
                drive->stop();

                drive->setMaxVelocity(100);
                drive->moveDistance(-9_in);

                // Turn and shoot
                drive->setMaxVelocity(100);
                drive->turnAngle(10_deg);
                shootCatapult();
                windCatapult();

                // Go back to 1 disk
                drive->setMaxVelocity(200);
                drive->moveDistance(3_in);

                // Turn to disk
                drive->setMaxVelocity(100);
                drive->turnAngle(135_deg);

                // Grab disk
                runIntake();
                drive->setMaxVelocity(200);
                drive->moveDistance(40_in);
                stopIntake();

                // Turn and shoot
                drive->setMaxVelocity(100);
                drive->turnAngle(-115_deg);

                // Get closer
                drive->setMaxVelocity(200);
                drive->moveDistance(14_in);
                shootCatapult();
                windCatapult();
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
                std::cout << "Running LEFT auton...";

                autonLeftCommon(drive);
                catapultMotor.moveVoltage(-10000);
                drive->setMaxVelocity(200);
                drive->turnAngle(-15_deg);
                windCatapult();
                runIntake();

                drive->setMaxVelocity(200);
                drive->moveDistance(12_in);

                drive->setMaxVelocity(200);
                drive->turnAngle(30_deg);
                
                drive->setMaxVelocity(200);
                drive->moveDistance(20_in);
                drive->setMaxVelocity(200);
                drive->turnAngle(-45_deg);
                drive->setMaxVelocity(250);
                drive->moveDistance(-30_in);
                stopIntake();

                shootCatapult();

                break;
            case RIGHT_AUTON_ID:
                // Drive to roller
                drive->setMaxVelocity(200);
                drive->moveDistance(23.1_in);

                drive->setMaxVelocity(200);
                drive->turnAngle(90_deg);

                // go foward to spin roller
                runIntakeReverse();
                drive->setMaxVelocity(100);
                drive->moveDistanceAsync(8_in);
                i = 0;
                while(!drive->isSettled()) {
                    if(i++ > 15) {
                        break;
                    }
                    pros::delay(20);
                }
                pros::delay(50);
                stopIntake();
                drive->stop();

                drive->setMaxVelocity(300);
                drive->moveDistance(-8_in);

                // Turn and shoot
                drive->setMaxVelocity(200);
                drive->turnAngle(10_deg);
                shootCatapult();

                drive->moveDistanceAsync(4_in);
                catapultMotor.moveVoltage(-10000);
                drive->waitUntilSettled();
                drive->turnAngleAsync(134_deg);
                drive->waitUntilSettled();
                catapultMotor.moveVoltage(0);

                runIntake();
                drive->setMaxVelocity(250);
                drive->moveDistanceAsync(70_in);
                windCatapult();
                drive->waitUntilSettled();
                drive->turnAngle(-82_deg);
                drive->setMaxVelocity(400);
                //drive->moveDistance(-7_in);
                pros::delay(50);
                shootCatapult();
                windCatapult();
                stopIntake();

                drive->moveDistance(2_in);

                //bandsPneumatic.set_value(HIGH);
                //pros::delay(200);
                //bandsPneumatic.set_value(LOW);
                //drive->setMaxVelocity(600);
                //drive->turnAngle(180_deg);
               break;
	    }
    }
}