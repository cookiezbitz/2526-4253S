#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool loaderState = false;
bool doinkerDown = false;

void setLoader(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        loaderPiston.set_value(!loaderState);
        loaderState = !loaderState;
    }
}

void toggleGrabber(bool status){
    
}


void switchGrabber(){
    //
}