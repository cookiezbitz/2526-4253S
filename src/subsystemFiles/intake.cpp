#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool intakeOn = false;
bool intakeReversed = false;
int power = 127;

void setIntake(){
    // R1: run first and second q 
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)||controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        lowerIntake.move(127);
        midIntake.move(127);
        upperIntake.move(-70);
    // R2: run ALL forward
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        lowerIntake.move(127);
        midIntake.move(127);
        upperIntake.move(127);
    // L1: run ALL backward
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        lowerIntake.move(-127);
        midIntake.move(-127);
        upperIntake.move(-127);
    }else{
        lowerIntake.brake();
        midIntake.brake();
        upperIntake.brake();
    }
}

void toggleIntake(bool status, double direction){
    if(status){
        lowerIntake.move(127);
        midIntake.move(127);
        upperIntake.move(-70);
    }else{
        lowerIntake.brake();
        midIntake.brake();
        upperIntake.brake();
    }
}


// should remeasure these vals:

// proximity > 150 or 200 = in front of sensor; max 255
// HUE:
// red [0,~20]
// blue [180,210]
