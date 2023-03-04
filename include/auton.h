namespace AUTON{
    void windCatapult();
    void shootCatapult();
    void runIntake();
    void runIntakeReverse();
    void stopIntake();
    void runAuton(std::shared_ptr<ChassisController> drive, int autonID);
}