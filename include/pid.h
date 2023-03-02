#pragma once
namespace PID{
    extern okapi::IterativePosPIDController::Gains distancePID;
    extern okapi::IterativePosPIDController::Gains turnPID;
    extern okapi::IterativePosPIDController::Gains forwardPID;
    char * getPIDTypeName(int index);
    okapi::IterativePosPIDController::Gains getConstant(int type);
    void setConstant(int type, int constant, float val);
    enum PID_IDS{Distance,Turn,Angle};
    enum CONSTANT_IDS{Kp,Ki,Kd};
}