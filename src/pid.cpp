#include "pid.h"
#include <string.h>

/** 
    Returns any constant.

    To create a new constant:
        - Define the constant here
        - Create a new ID enum in pid.h
*/
namespace PID{ 
    float AllPID[3][3] = {
        {0.0020,0.0000,0.0800}, // Distance
        {0.0010,0.0000,0.0400}, // Turn
        {0.0005,0.0000,0.0200}, // Angle
    };
    char * DistanceName = (char *)"Distance";
    char * TurnName = (char *)"  Turn  ";
    char * AngleName = (char *)" Angle ";
    char * getPIDTypeName(int index){
        switch (index){
            case 0:
                return PID::DistanceName;
                break;
            case 1:
                return PID::TurnName;
                break;
            case 2:
                return PID::AngleName;
                break;
        }
        return (char *)"wtf";
    };
    float getConstant(int type, int constant){
        return PID::AllPID[type][constant];
        return -1;
    }
    void setConstant(int type, int constant, float val){
        PID::AllPID[type][constant] = val;
    }
}