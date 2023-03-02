#include "main.h"
#include "pid.h"
#include <string.h>

/** 
    Returns any constant.

    To create a new constant:
        - Define the constant here
        - Create a new ID enum in pid.h
*/
namespace PID{ 
    okapi::IterativePosPIDController::Gains distancePID = {0.0000,0.0000,0.0000}; // Distance
    okapi::IterativePosPIDController::Gains turnPID = {0.00258,0.0000,0.0000}; // Turn
    okapi::IterativePosPIDController::Gains anglePID = {0.0000,0.0000,0.0000}; // Forward
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
    okapi::IterativePosPIDController::Gains getConstant(int id){
        switch (id){
            case 0:
                return distancePID;
                break;
            case 1:
                return turnPID;
                break;
            case 2:
                return anglePID;
                break;
        }
    }
    void setConstant(int type, int constant, float val){
        switch (type) {
            case 0:
                switch (constant) {
                    case
                    distancePID.kP = val;
                }
                break;
            case 1:
                turnPID.kP = val;
                break;
            case 2:
                anglePID.kP = val;
                break;
        }
    
        (std::dynamic_pointer_cast<ChassisControllerPID> (drive))->setGains(distancePID,turnPID,anglePID);
    }
}