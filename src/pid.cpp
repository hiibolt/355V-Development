#include "pid.h"
#include <string.h>
float Kp = 0.0020;
float Ki = 0;
float Kd = 0.00008;

float prev_Kp = 0;
float prev_Ki = 0;
float prev_Kd = 0;

float getConstant(char* con){
    if(con == "Kp"){
        return Kp;
    }else if(con == "Ki"){
        return Ki;
    }else if(con == "Kd"){
        return Kd;
    }
    return -1;
}
void setConstant(char* con, float val){
    if(con == "prev_Kp"){
        prev_Kp = val;
    }else if(con == "prev_Ki"){
        prev_Ki = val;
    }else if(con == "prev_Kd"){
        prev_Kd = val;
    }else if(con == "Kp"){
        Kp = val;
    }else if(con == "Ki"){
        Ki = val;
    }else if(con == "Kd"){
        Kd = val;
    }
}