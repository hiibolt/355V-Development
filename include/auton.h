void windCatapult(Motor catapultMotor, ADIButton stopSwitch);
void shootCatapult(Motor catapultMotor, ADIButton stopSwitch);
void runIntake(Motor intakeMotor);
void runIntakeReverse(Motor intakeMotor);
void stopIntake(Motor intakeMotor);
namespace AUTON{
    void runAuton(std::shared_ptr<ChassisController> drive, int autonID);
}