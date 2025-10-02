#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool intakeOn = false;
bool intakeReversed = false;
int power = 127;




void setIntake(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeUpper.move(int(power));
        intakeLower.move(int(power));
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeUpper.move(int(-1*power));
        intakeLower.move(int(-1*power));
    }else{
        intakeUpper.brake();
        intakeLower.brake();
    }

}

void toggleIntake(bool status, double direction){
    if(status){
        intakeUpper.move(int(power*direction));
        intakeLower.move(int(power*direction));
    }else{
        intakeUpper.brake();
        intakeLower.brake();
    }
}


// should remeasure these vals:

// proximity > 150 or 200 = in front of sensor; max 255
// HUE:
// red [0,~20]
// blue [180,210]
