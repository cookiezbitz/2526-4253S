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
    
    //chassis.moveToPoint(45,-26,5000,{.minSpeed=60, .earlyExitRange=4},false);
    //chassis.moveToPose(45,-26,235,5000,{.minSpeed=60, .earlyExitRange=5},false);


    chassis.moveToPoint(36,-36,5000,{.earlyExitRange = 2},false);
    chassis.turnToPoint(0,0,1000,{.maxSpeed=60,.minSpeed=10,.earlyExitRange=10},false);

    toggleIntake(true,0);
    chassis.moveToPose(27,-27,315,3000,{.maxSpeed=60, .minSpeed=30, .earlyExitRange = 2.5},false);
    toggleLoader(true);

    //chassis.moveToPose(16,-16,315,3000,{.maxSpeed=60, .minSpeed=50},false);
    chassis.moveToPoint(16,-16,3000,{.maxSpeed=60, .minSpeed=30, .earlyExitRange=1},false);

    pros::delay(500);

    chassis.moveToPoint(22,-22,5000,{.forwards = false, .maxSpeed=80,.minSpeed=50,.earlyExitRange=2},false);

    chassis.turnToHeading(135,1000,{.maxSpeed=60,.earlyExitRange=15},true);
    pros::delay(500);
    //toggleIntake(false,0);

    // try to ensure intake:
    /*chassis.moveToPoint(28,-28,3000,{.maxSpeed=60,.minSpeed=60},false);
    toggleLoader(false);*/



    //chassis.moveToPose(10,-10,135,3000,{.forwards=false, .maxSpeed=40, .minSpeed=40, .earlyExitRange = 1},false);
    
    chassis.moveToPose(14,-12,135,3000,{.forwards=false, .maxSpeed=40, .minSpeed=40, .earlyExitRange = 1},false);
    
    toggleIntake(true,1);
    pros::delay(1500);

    toggleIntake(false,0);
    chassis.moveToPoint(42,-48,5000,{.forwards = true, .maxSpeed=80,.minSpeed=50,.earlyExitRange=2},false);


}

void altLeftAuton(){
    chassis.setPose(62,-16.247,270);

    // intake directly into blocks
    chassis.moveToPoint(36,-24,5000,{.minSpeed=30,.earlyExitRange = 3},false);
    toggleIntake(true, 0);

    chassis.moveToPoint(22,-22,5000,{.maxSpeed=60,.minSpeed=40,.earlyExitRange = 1},false);
    toggleLoader(true);
    chassis.moveToPoint(15,-22,3000,{.maxSpeed=60,.minSpeed=40,.earlyExitRange = 0},false);

    chassis.moveToPoint(22,-22,5000,{.forwards = false, .maxSpeed=80,.minSpeed=50},false);

    chassis.turnToHeading(135,1000,{.maxSpeed=60,.earlyExitRange=10},true);

    // 24,24 or something
    // back into 10,-10 OR ADJUST to score

    chassis.moveToPoint(12+3,-12,3000,{.forwards=false, .maxSpeed=40, .minSpeed=40, .earlyExitRange = 1},false);
    toggleIntake(true,1);


    pros::delay(600);
    toggleIntake(true, -1);
    pros::delay(200);
    toggleIntake(false, 0);
    pros::delay(500);


    //toggleLoader(false);
    // go to x, -48 to set up for nelson
    chassis.moveToPoint(48,-48,5000,{.forwards = true, .maxSpeed=80,.minSpeed=50,.earlyExitRange=2},false);
    toggleIntake(false,0);
    chassis.turnToPoint(72,-48,2000,{.maxSpeed=60},false);
    chassis.moveToPoint(30, -48, 2000,{.forwards = false,.minSpeed=60},false);
    toggleIntake(true, 1);
    pros::delay(2000);
    toggleIntake(false,0);
    //chassis.moveToPoint(60,-48,3000,{.maxSpeed=60,.minSpeed=40},false);
}

void basic(){
    chassis.setPose(0,0,0);

    chassis.moveToPoint(0,6,5000);

    //chassis.moveToPose(0,24,0,8000, {});

    //chassis.moveToPose(0,0,0,8000, {.forwards = false});

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
    
    //leftAuton();
    altLeftAuton();

}