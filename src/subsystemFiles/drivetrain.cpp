#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool loaderState = false;
bool wingStatus = false;

void setPistons(){ // + loader + intake piston
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){ // nelson
        wingStatus = !wingStatus;
        wingPiston.set_value(wingStatus);
    }

    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){ // nelson
        if(loaderState){
            loaderPiston.set_value(false);
            loaderState=false;
        }else{
            loaderPiston.set_value(true);
            loaderState=true;

            if(centerDescoreStatus == true){
                centerDescore.set_value(false);
                centerDescoreStatus = false;
            }
        }
    }
}

bool parkStatus = false;
bool clampStatus = false;
/*void setDoublePark(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){ // was x
    //if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){ // nelson
        parkStatus = !parkStatus;
        doublePark.set_value(parkStatus);
    }
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){ // was a
    //if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){ // nelson
        clampStatus = !clampStatus;
        doubleParkClamp.set_value(!clampStatus);
    }
}*/

void setDrive(){ // use controller input
    int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int turn = -controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)*0.8; // nelson
    //int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)*0.75; // preston

    chassis.arcade(forward,turn);

    setPistons();
    //setDoublePark();
}

void toggleWing(bool state){
    wingPiston.set_value(state);
}