#pragma once
namespace PID{
    char * getPIDTypeName(int index);
    float getConstant(int type, int constant);
    void setConstant(int type, int constant, float val);
    enum PID_IDS{Distance,Turn,Angle};
    enum CONSTANT_IDS{Kp,Ki,Kd};
}