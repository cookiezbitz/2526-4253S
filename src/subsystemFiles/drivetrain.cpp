#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

void setDrive(){ // use controller input
    int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)*0.85;
    int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)*0.5;

    chassis.arcade(forward,-turn);
}