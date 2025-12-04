#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool loaderState = false;
bool doinkerDown = false;

void setLoader(){
    /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){ 
        loaderPiston.set_value(!loaderState);
        loaderState = !loaderState;
    }*/
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        loaderPiston.set_value(true);
    }else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        loaderPiston.set_value(false);
    }
}

void toggleLoader(bool status){
    loaderPiston.set_value(status);
}