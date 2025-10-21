#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

//ASSET(testblueright_txt);


int delayTask = 190;
int delayIntake = 200;

void colorSort(){
    while(true){
        if(optical.get_proximity() > 150){
            if(isBlue){ // blue alliance
                if(optical.get_hue() < 50){  
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                    
                }
            }else{ // red alliance
                if(optical.get_hue() > 150){
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                }
            }
            pros::Task::delay(100);
        }
    }
}

void redRight(){

}

void redLeft(){

}

void blueRight(){

}

void blueLeft(){

}

void basic(){
    chassis.setPose(0,0,0);


    toggleIntake(true,1);
    chassis.moveToPoint(0,40,8000, {.maxSpeed = 80});

   // STOP AFTER A TIME TO NOT BURN OUT THE MOTORS
}

void autonomous() {
    //redRight();
    //redLeft();
    //blueRight();
    //blueLeft();
    basic();
}