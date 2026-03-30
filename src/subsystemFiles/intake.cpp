#include "main.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subsystemHeaders/global.hpp"

int intakeState = 0;

bool centerDescoreStatus = false;
bool hoodPistonStatus = true;
bool middleGoalMacroTriggered = false;
bool skills = true;
void middleGoalMacro(){
    if(middleGoalMacroTriggered == false && intake_optical.get_proximity() > 150){ // check intake fullness
        lowerIntake.move(-100);
        upperIntake.move(-100);
        middleGoalMacroTriggered = true;
        pros::Task::delay(200);
    }
    lowerIntake.move(100);
    if( skills){
        upperIntake.move(-20);
    }
    else{
        upperIntake.move(-50);
    }
}

void holdIntakeSetting(){
    lowerIntake.move(120);
    if(intake_optical.get_proximity() > 150){
        upperIntake.move(0);
    }else{
        upperIntake.move(120);
    }
}

void setIntake(){
    // R1: hold (close hood)
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        //lowerIntake.move(120);
        //upperIntake.move(120);
        holdIntakeSetting();
        if(hoodPistonStatus==false){
            hoodPiston.set_value(true);
            hoodPistonStatus = true;
        }
    // R2: score (open hood);
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        lowerIntake.move(120);
        upperIntake.move(120);
        if(hoodPistonStatus==true){
            hoodPiston.set_value(false);
            hoodPistonStatus = false;
        }
    // L1: middle goal
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        middleGoalMacro();
    // L2: outtake (all backward)
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        if(skills){
            lowerIntake.move(-55);
        }else{
            lowerIntake.move(-100);
        }
        upperIntake.move(-40);
    }else{
        lowerIntake.brake();
        upperIntake.brake();
        
        middleGoalMacroTriggered = false; // reset macro trigger
    }

    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
        hoodPistonStatus = !hoodPistonStatus;
        hoodPiston.set_value(hoodPistonStatus);
    }
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        centerDescoreStatus = !centerDescoreStatus;
        centerDescore.set_value(centerDescoreStatus);

        if(loaderState == true){
            loaderPiston.set_value(false);
            loaderState=false;
        }
    }
}

void toggleIntake(bool status, double state, int volt){
    if(status){
        if(state==1){
            lowerIntake.move(120);
            upperIntake.move(volt);
        }else if(state==-1){
            lowerIntake.move(-volt);
            upperIntake.move(-volt);
        }else{ 
            lowerIntake.move(volt);
            upperIntake.move(-40);
        }
    }else{
        lowerIntake.brake();
        upperIntake.brake();
    }
    
    intakeState = state; //
}

void toggleIntake(bool status, double state){
    if(status){
        if(state==1){
            lowerIntake.move(120);
            upperIntake.move(127);
            hoodPiston.set_value(false);
        }else if(state==-1){
            lowerIntake.move(-127);
            upperIntake.move(-127);
        }else if(state ==2){
            lowerIntake.move(-100);
            upperIntake.move(-100);
            pros::delay(200);
            lowerIntake.move(120);
            upperIntake.move(-90);
        }else{
            lowerIntake.move(120);
            upperIntake.move(55); // -40 before
            hoodPiston.set_value(true);
        }
    }else{
        lowerIntake.brake();
        upperIntake.brake();
    }
    
    intakeState = state; //
    //intakeState = status;
}


// should remeasure these vals:

// proximity > 150 or 200 = in front of sensor; max 255
// HUE:
// red [0,~20]
// blue [180,210]

void setIntakeState(int state){
    intakeState = state;
}