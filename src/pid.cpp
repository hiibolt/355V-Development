#include "pid.h"
#include <string.h>
float Kp = 0.0020;
float Ki = 0;
float Kd = 0.00008;

/** 
    Returns any constant.

    To create a new constant:
        - Define the constant here
        - Create a new ID enum in pid.h
*/
float getConstant(int constant){
    switch (constant){
        case CONSTANT_KP:
            return Kp;
            break;
        case CONSTANT_KI:
            return Ki;
            break;
        case CONSTANT_KD:
            return KD;
            break;
    }
    return -1;
}
void setConstant(int con, float val){
    switch (constant){
        case CONSTANT_KP:
            Kp = val;
            break;
        case CONSTANT_KI:
            Ki = val;
            break;
        case CONSTANT_KD:
            KD = val;
            break;
    }
}