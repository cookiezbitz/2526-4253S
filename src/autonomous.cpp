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

void leftAuton(){
    chassis.setPose(62,-16.247,270);

    // chassis.moveToPoint(45,-26,5000,{.minSpeed=60, .earlyExitRange=3},false);
    chassis.moveToPoint(45,-26,5000,{.minSpeed=60, .earlyExitRange=3},false);


    chassis.moveToPoint(36,-36,5000,{},false);
    chassis.turnToPoint(0,0,1000,{.minSpeed=40,.earlyExitRange=10},false);

    toggleIntake(true,0);
    chassis.moveToPose(27,-27,315,3000,{.maxSpeed=60, .minSpeed=40, .earlyExitRange = 2.5},false);
    toggleLoader(true);

    chassis.moveToPose(16,-16,315,3000,{.maxSpeed=60, .minSpeed=50},false);

    pros::delay(500);

    chassis.moveToPoint(22,-22,5000,{.forwards = false, .maxSpeed=80,.minSpeed=50,.earlyExitRange=1},false);

    chassis.turnToHeading(135,1000,{.minSpeed=40,.earlyExitRange=10},true);
    pros::delay(500);
    toggleLoader(false);

    chassis.moveToPose(10,-10,135,3000,{.forwards=false, .maxSpeed=40, .minSpeed=40, .earlyExitRange = 1},false);
    toggleIntake(true,1);


}

void basic(){
    chassis.setPose(0,0,0);


    toggleIntake(true,1);
    chassis.moveToPoint(0,40,8000, {.maxSpeed = 80});

   // STOP AFTER A TIME TO NOT BURN OUT THE MOTORS
}

void pidtuning(){
    chassis.setPose(0,0,0);

    //chassis.moveToPoint(0,24,5000);
    chassis.turnToHeading(90, 5000);

}

void autonomous() {
    //redRight();
    //redLeft();
    //blueRight();
    //blueLeft();
    //basic();
    //pidtuning();
    leftAuton();
}