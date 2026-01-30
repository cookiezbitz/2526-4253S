#include "main.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

int intakeState = 0;

//bool intakePistonStatus = true;
bool lowerScoringToggled = false;
int lowerScoringTimer = 0;
void setIntakePiston(){
    /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){ // was y
    //if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){ // nelson
        intakePistonStatus = !intakePistonStatus;
        scoringModifier.set_value(intakePistonStatus);
    }*/
}

void setIntake(){
    // R1: run first and second q 
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        lowerIntake.move(120);
        upperIntake.move(-40);
        //upperIntake.brake();
    // R2: run ALL forward
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        lowerIntake.move(120);
        upperIntake.move(127);
    // L1: run ALL backward
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)/*||controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)*/){
        lowerIntake.move(-127);
        upperIntake.move(-127);
    // L2: weak lower goal
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        lowerIntake.move(-80);
        upperIntake.move(-80);
        if(lowerScoringToggled==false){
            lowerIntakeModifier.set_value(true);
            lowerScoringToggled = true;
        }
    }else{
        lowerIntake.brake();
        upperIntake.brake();
        
        if(lowerScoringToggled){
            lowerScoringTimer += 15;
            if(lowerScoringTimer >= 500){
                lowerIntakeModifier.set_value(false);
                lowerScoringToggled = false;
                lowerScoringTimer = 0;
            }
        }
    }
    setIntakePiston();
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
    //intakeState = status;
}

void toggleIntake(bool status, double state){
    if(status){
        if(state==1){
            lowerIntake.move(120);
            upperIntake.move(127);
        }else if(state==-1){
            lowerIntake.move(-127);
            upperIntake.move(-127);
        }else{
            lowerIntake.move(120);
            upperIntake.move(-55); // -40 before
        }
    }else{
        lowerIntake.brake();
        upperIntake.brake();
    }
    
    intakeState = state; //
    //intakeState = status;
}

void setScoreHeight(bool intakePosition){
    scoringModifier.set_value(intakePosition); 
}

void toggleLowerGoalPiston(bool status){
    lowerIntakeModifier.set_value(status);
}

// should remeasure these vals:

// proximity > 150 or 200 = in front of sensor; max 255
// HUE:
// red [0,~20]
// blue [180,210]
