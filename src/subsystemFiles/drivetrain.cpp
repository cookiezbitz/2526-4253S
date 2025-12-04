#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool wingStatus = false;
void setWing(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){ // was right
        wingStatus = !wingStatus;
        wingPiston.set_value(wingStatus);
    }
}

bool parkStatus = false;
bool clampStatus = false;
void setDoublePark(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){ // was x
        parkStatus = !parkStatus;
        doublePark.set_value(parkStatus);
    }
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){ // was a
        clampStatus = !clampStatus;
        doubleParkClamp.set_value(!clampStatus);
    }
}

void setDrive(){ // use controller input
    int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    //int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    chassis.arcade(forward,-turn);

    setWing();
    setDoublePark();
}

