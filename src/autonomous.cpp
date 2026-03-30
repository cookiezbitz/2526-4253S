#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cmath>
#include <iostream>

//ASSET(testblueright_txt);


int delayTask = 190;
int delayIntake = 200;

/*void colorIsBlue(bool blue){
    if(blue){
        led.set_all(0X0000FF); // blue = 0X0000FF red = 0XFF0000
        led2.set_all(0X0000FF);
        led.update();
        led2.update();
    }else{
        led.set_all(0XFF0000); // blue = 0X0000FF red = 0XFF0000
        led2.set_all(0XFF0000);
        led.update();
        led2.update();
    }
    
}*/

void antiJamTask(){
    int stallCounter = 0; // ms
    int reversalTime = 300; // ms

    while(true){
        if(intakeState != 0){ // if intake isnt supposed to be jamming
            if(fabs(upperIntake.get_actual_velocity()) < 50 || fabs(lowerIntake.get_actual_velocity()) < 50){ // rpm
                stallCounter += 20;
            }else{
                stallCounter = 0;
            }
            
            if(stallCounter > 400){ // in ms
                // jam detected
                if(intakeState == 1){ // intake forward
                    lowerIntake.move(-90);
                    upperIntake.move(-90);
                    pros::Task::delay(reversalTime);
                    lowerIntake.move(127);
                    upperIntake.move(127);
                    pros::Task::delay(200);
                    lowerIntake.move(120);
                    upperIntake.move(-40);
                    stallCounter = 0; 
                }/*else if(intakeState == -1){ // intake reverse
                    lowerIntake.move(70);
                    upperIntake.move(70);
                    pros::Task::delay(reversalTime);
                    lowerIntake.move(-70);
                    upperIntake.move(-70);
                }*/
            }
        }
        pros::Task::delay(20);
    }
}


double backDistanceSensorOffset = 3.3; // inches from center of robot to distance sensor
// returns distance to get to goal position, using the BACK distance sensor.
// use absolute value of coordinates
double getDistanceTo(double readingPosition, double goalPosition){ // coordinates in inches
    double distanceRead = back_distanceSensor.get_distance()/25.4; // mm converted to inches
    // reading pos - current pos = distance read
    // distance diff = reading pos - goal pos
    return distanceRead - (readingPosition - goalPosition) + backDistanceSensorOffset;
    // distance to travel to goal pos = distance read - distance diff + offset of sensor from robot center
}

double getRearOffset(double readingPosition){
    double distanceRead = back_distanceSensor.get_distance()/25.4; // mm converted to inches

    // distance read = distance btwn robot rear and wall
    // reading position - distance read = current "coord"...
    // current coord - sensor offset = robot CENTER coord
    return readingPosition - distanceRead - backDistanceSensorOffset;
}


double rightDistanceSensorOffset = 4.5; // in 
// returns distance relative to reading position
// uses the RIGHT side distance sensor.
// use absolute value of coordinates
double getDistanceRelative(double readingPosition){
    double distanceRead = right_distanceSensor.get_distance()/25.4; // mm converted to inches

    return readingPosition - distanceRead - rightDistanceSensorOffset;
}

void wait(double ms){
    pros::delay(ms);
}

// go forward
double kP = 4, kD = 20;
void translate(double dist, int timeout, double minSpeed, double maxSpeed, double earlyExit){
    // circumfrence = pi * d = 3.14 * 2.0
    double circumfrence = 2.0 * 3.14; // in inches, odom pod
    // rotation = position / 100 / 360 (degrees)
    double currentPos;// = vertPod.get_position()/(100.0*360.0) * circumfrence; // centideg -> deg -> circumfrence (inches)
    double targetPos = currentPos + dist;
    
    int timeElapsed = 0;
    double prevError = dist;
    double error;
    double derivative;
    double output;

    while (true) {
        // current error and output power
        currentPos = vertPod.get_position()/(100.0*360.0) * circumfrence;
        error = targetPos - currentPos;
        derivative = error - prevError;
        output = kP * error + kD * derivative;
        
        prevError = error;

        // limit output powers
        double sign = (output >= 0) ? 1.0 : -1.0;
        double absOut = std::abs(output);
        absOut = std::clamp(absOut, minSpeed, maxSpeed);
        output = sign * absOut;

        driveLeft.move(output);
        driveRight.move(output);

        // check for exit
        if (timeElapsed >= timeout) break; // timeout
        if (earlyExit > 0 && std::abs(error) < earlyExit) break; // early exit
        if (std::abs(error) < 0.1) break; // position reached

        pros::delay(10);
        timeElapsed += 10;
    }

    driveLeft.move(0);
    driveRight.move(0);
}

/*void colorSort(){
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
*/

void right_AWP(){
    chassis.setPose(47.5,16.427,0);

    // goal 1

    chassis.moveToPoint(47.5,47,1500,{.minSpeed=10,.earlyExitRange=0.5-0.5});

    toggleIntake(true,0);
    toggleLoader(true);

    chassis.turnToHeading(90,800,{.maxSpeed=80,.minSpeed=0,.earlyExitRange=0});
    chassis.waitUntilDone();

    chassis.moveToPoint(55+4,47.5+0.5,1000,{.maxSpeed=70,.minSpeed=40},true); 
    wait(850);

    chassis.moveToPoint(29.5-3,48,1500-500,{.forwards=false,.maxSpeed=90,.minSpeed=30},true);
    chassis.waitUntil(20);
    toggleIntake(true,1);
    toggleLoader(false);

    wait(1000);
    chassis.setPose(29.5,47.5,chassis.getPose().theta);
    wait(100);

    // to center

    chassis.moveToPoint(38,47,1000,{.minSpeed=40,.earlyExitRange=2},true);

    chassis.turnToHeading(225,1000,{.minSpeed=40,.earlyExitRange=10});
    toggleIntake(true,-1); // outtake in case of jam

    chassis.moveToPoint(22,22,1500,{.maxSpeed=100,.minSpeed=30,.earlyExitRange=1},true);
    toggleIntake(true,1);
  
    chassis.waitUntil(16);
    toggleIntake(true,0);

    chassis.waitUntil(22);
  
    chassis.turnToHeading(180,1000,{.minSpeed=30,.earlyExitRange=5},false);

    chassis.moveToPoint(22+1,-22,2000,{.maxSpeed=120,.minSpeed=30,.earlyExitRange=24-12},true);
    chassis.waitUntil(10);
    toggleIntake(true,1);
    chassis.waitUntil(16);
    toggleIntake(true,0);

    chassis.moveToPoint(22+1,-24,2000,{.maxSpeed=40,.minSpeed=10,.earlyExitRange=1},true);

    chassis.waitUntil(20-12);
    toggleLoader(true);

    // long goal 2

    chassis.moveToPoint(47,-48,2000,{.maxSpeed=127,.minSpeed=20,.earlyExitRange=0},true);  // early exit?

    chassis.turnToHeading(90,1000,{.minSpeed=30,.earlyExitRange=10});
   
    chassis.moveToPoint(28-3,-47.5-1-1,2000,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(10);
    toggleIntake(true,1);
    wait(1900);
    chassis.setPose(29.5,-47.5,chassis.getPose().theta);
    wait(100);

    // loader
    chassis.moveToPoint(47,-47,3000,{.maxSpeed=120,.minSpeed=50,.earlyExitRange=2});
    toggleIntake(true,0);

    chassis.moveToPoint(55+10,-47,1000-100,{.maxSpeed=50,.minSpeed=30},false);
    
    // center

    chassis.moveToPoint(11,-10,3000,{.forwards=false,.maxSpeed=120,.minSpeed=10/*,.earlyExitRange=24*/},true);
    chassis.waitUntil(33.5);
    chassis.cancelMotion();
    chassis.moveToPoint(10,-9,1500,{.forwards=false,.maxSpeed=60,.minSpeed=10},true);
    toggleIntake(false,0);
    chassis.waitUntil(10);
    toggleIntake(true,-1);
    chassis.waitUntil(16); 

    toggleIntake(true,1);

    chassis.waitUntil(10+4+4);

    setScoreHeight(false);

    wait(500);
    toggleIntake(true,0);
    wait(1000);
    chassis.moveToPoint(12,-12,250,{.minSpeed=50},false);
    setScoreHeight(true);
    chassis.moveToPoint(10,-10,500,{.forwards=false,.minSpeed=40});
}

// right is all non-elim, left is all elim
void right_Long7(){
    //chassis.setPose(62,16.427,270);
    chassis.setPose((48-2.25),(15.75),270);

    chassis.moveToPoint(22,22,1500,{.minSpeed=20,.earlyExitRange=15},true);
    //toggleIntake(true,0);
    chassis.moveToPoint(22,22,1300,{.maxSpeed=30,.minSpeed=10},true);
    toggleIntake(true,0);
    toggleLoader(true);

    chassis.turnToHeading(45,1000,{.maxSpeed=80,.earlyExitRange=10},false);

    // 2nd segment

    //toggleLoader(false);
    chassis.moveToPoint(48,47,2000,{.minSpeed=10,.earlyExitRange=0.5},true); // accurate?
    //pros::delay(200);
    //toggleIntake(true,-1);

    //chassis.waitUntil(16);
    //toggleIntake(true,0); // reset intake 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},true);
   
    // should score first
    /*chassis.moveToPoint(29.5-2,47+1,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(13);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  
    pros::delay(1500); // can tune
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    pros::delay(300);*/

    // then, match load
    //toggleLoader(true);
    //pros::delay(200);

    chassis.moveToPoint(55+5,47,1000,{.maxSpeed=55,.minSpeed=40},true); 
    
    toggleIntake(true, 0);
    //chassis.waitUntil(24);
    //pros::delay(750); // adj for 3 blocks??  // cut loader time

    // score again
    chassis.moveToPoint(29.5-2,47+1,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(20);
    toggleIntake(true,1); // score 
    

    pros::delay(1100+2000); // wait
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    toggleLoader(false);  
    pros::delay(300);

    // use wing
    // 10.5 displaced
    
    /*chassis.moveToPoint(39,58.5-3,1000,{.minSpeed=30},true);

    toggleIntake(false,0);
    chassis.turnToHeading(90,3000,{.minSpeed=15},false);
    
    //chassis.setPose(39,55.5,90);
    //pros::delay(50);

    chassis.moveToPoint(12,55.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=15}); // can change to poses*/

    chassis.moveToPoint(39,38.5+0.5,800,{.minSpeed=30},true);
    toggleIntake(false,0);
    chassis.turnToHeading(270,2500,{.minSpeed=15},false);
    //chassis.setPose(39,38.5,chassis.getPose().theta);
    //pros::delay(50);
    chassis.moveToPoint(10,38.5+0.5,2000,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); // can change to poses
    chassis.turnToHeading(235,2000,{.minSpeed=30},false);
}

void right_LowerLong(){ 
    chassis.setPose((48-2.25),(15.75),270);

    chassis.moveToPoint(22,22,1500,{.minSpeed=20,.earlyExitRange=15},true);
    chassis.moveToPoint(22-2,22,1300,{.maxSpeed=25,.minSpeed=10},true);
    toggleIntake(true,0);
    pros::delay(200);
    toggleLoader(true);

    chassis.moveToPoint(25,22,1000,{.forwards=false,.maxSpeed=40,.minSpeed=0,.earlyExitRange=0.5},true);

    chassis.turnToHeading(220,1000,{.maxSpeed=80,.earlyExitRange=1},false);

    // lower goal
    chassis.moveToPoint(13.5,11,1500,{.maxSpeed=60},true);
    toggleLoader(false);
    //toggleLowerGoalPiston(true);
    chassis.waitUntil(10-2);
    toggleIntake(true,-1);

    pros::delay(400);
    toggleIntake(true,-1,100);
    pros::delay(750-400);

    chassis.moveToPoint(14+3-.5,11+3,500,{.forwards=false,.minSpeed=20},true);
    chassis.moveToPoint(12,12,500,{.minSpeed=30});
    //chassis.moveToPoint(15.5,12.5,500,{.forwards=false,.minSpeed=30});

    // 2nd segment

    chassis.moveToPoint(48,47,2000,{.forwards=false,.earlyExitRange=0.5},true); // accurate?
    chassis.waitUntil(10);
    toggleIntake(true,0);
    
    chassis.turnToHeading(90,1000,{.earlyExitRange=0},true);
    toggleIntake(false,0);
    //toggleLowerGoalPiston(false);
   
    // then, match load
    toggleLoader(true);
    pros::delay(280);

    chassis.moveToPoint(55+5,47,900,{.maxSpeed=50,.minSpeed=30},true); 
    
    toggleIntake(true, 0);


    // score again
    chassis.moveToPoint(29.5-2,47+1,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(20);
    toggleIntake(true,1); // score 

    pros::delay(1200); // wait
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    toggleLoader(false);  
    pros::delay(300);

    // use wing

    // 8.5 displaced
    /*chassis.moveToPoint(39,55.5,1000,{.minSpeed=30},true);
    toggleIntake(false,0);
    chassis.turnToHeading(90,750,{.minSpeed=15},false);
    chassis.moveToPoint(12,55.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=0}); // can change to poses*/

    chassis.moveToPoint(39,38.5,1000,{.minSpeed=30},true);
    toggleIntake(false,0);
    chassis.turnToHeading(270,3000,{.minSpeed=15},false);
    //chassis.setPose(39,38.5,chassis.getPose().theta);
    pros::delay(50);
    chassis.moveToPoint(8,38.5,2000,{.maxSpeed=80}); // can change to poses
    chassis.turnToHeading(235,2000,{.minSpeed=30},false);
}

void right_LongLower(){ 
    chassis.setPose(47.5,16.427,0);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5,47.5+0.3,1000,{.earlyExitRange=0.3});

    toggleIntake(true,0);

    chassis.turnToHeading(90,700,{.earlyExitRange=1});


    toggleLoader(true);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+20,47,1000,{.maxSpeed=50,.minSpeed=30},true); 

    // long goal score
    chassis.moveToPoint(29.5-2,48+0.5,1700,{.forwards=false,.maxSpeed=60,.minSpeed=30},true);
    chassis.waitUntil(24);
    toggleIntake(true,1);
    toggleLoader(false);

    pros::delay(1000); // cut extra time
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    //pros::delay(100);

    // section 2 (center)

    chassis.moveToPoint(47,47,1000,{.minSpeed=40,.earlyExitRange=2},true);

    chassis.turnToHeading(235,1000,{.minSpeed=30,.earlyExitRange=10});
    chassis.moveToPoint(22,24-2,1500,{.minSpeed=20,.earlyExitRange=15},true);
    chassis.moveToPoint(22+2,22-3,1300,{.maxSpeed=30,.minSpeed=10,.earlyExitRange=4},true);
    toggleIntake(true,0);

    toggleLoader(true);
    //chassis.moveToPoint(22+2,22-2,700,{.maxSpeed=30,.minSpeed=10},true);
    
    //chassis.moveToPoint(25,22,1000,{.forwards=false,.maxSpeed=40,.minSpeed=0},true);

    chassis.turnToHeading(210+5,700,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=1},true);
    toggleLoader(false);

    // lower goal
    //chassis.moveToPose(14+1-0.5,11+1,235,1500,{.maxSpeed=60,.minSpeed=20},true);
    chassis.moveToPoint(14+1-0.5,11+1,1500,{.maxSpeed=60},true);


    //chassis.moveToPoint(14+1.5,11+1.5,1500,{.maxSpeed=60},true);
    
    toggleLowerGoalPiston(true);
    chassis.waitUntil(12);
    toggleIntake(true,-1);
    
    //chassis.turnToHeading(235,1000,{.minSpeed=20,.earlyExitRange=1},true);
    pros::delay(1000);
    chassis.moveToPoint(14-1,11-1,400,{.maxSpeed=60,.minSpeed=30});


    // section 3 (descore)

    chassis.moveToPoint(39,38.5-4,1000,{.forwards=false,.maxSpeed=80,.minSpeed=10},true);
    chassis.waitUntil(10);
    toggleIntake(false,0);
    chassis.turnToHeading(270,3000,{.minSpeed=15,.earlyExitRange=2},false);
    //chassis.setPose(39,38.5,chassis.getPose().theta);
    //pros::delay(50);
    chassis.moveToPoint(12,38.5-4,1200,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); // can change to poses
    chassis.turnToHeading(235,1000,{.minSpeed=50,.earlyExitRange=5},false);
}

void left_CenterLong(){ 
    chassis.setPose(38.5+8,-(4+13.625/2.0),270);

    chassis.moveToPoint(22,-22,1500,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=15},true);

    chassis.moveToPoint(22-2,-22-2,1000,{.maxSpeed=30,.minSpeed=10,.earlyExitRange=1},true);
    toggleIntake(true,0);
    pros::delay(300);
    toggleLoader(true);

    chassis.moveToPoint(22,-22,500,{.forwards=false,.minSpeed=30});

    chassis.turnToHeading(135,750,{.maxSpeed=80,.minSpeed=0,.earlyExitRange=10},true);

    chassis.moveToPoint(10,-10+1,1000,{.forwards=false,.minSpeed=20},true);
    toggleIntake(false,0);
    chassis.waitUntil(4);
    toggleIntake(true,-1);

    chassis.waitUntil(9); // 10 --> 9
    toggleIntake(true,1,60);

    chassis.waitUntil(10);
    setScoreHeight(false);
    pros::delay(1300);

    chassis.moveToPoint(12,-12,500,{.minSpeed=30});
    chassis.waitUntil(3);
    setScoreHeight(true);
    chassis.moveToPoint(8,-8,500,{.forwards=false,.minSpeed=40});

    // 2nd segment

    chassis.moveToPoint(48,-47.5,2000,{.maxSpeed=100,.minSpeed=10,.earlyExitRange=0.5},true); // accurate?
    pros::delay(200);
    toggleIntake(true,-1);
    setScoreHeight(false);
    chassis.waitUntil(20);
    setScoreHeight(true);
    toggleIntake(false,0); // reset intake 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=15,.earlyExitRange=5},true);

    //toggleLoader(true);
    //pros::delay(400);

    // into loader
    chassis.moveToPoint(55+10,-47.5,800,{.maxSpeed=50,.minSpeed=30},true); 

    toggleIntake(true, 0);

    chassis.moveToPoint(29.5-2,-47.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=40,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1);
    toggleLoader(false);  

    pros::delay(800); 
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset

    // use wing
    chassis.moveToPoint(48,-47,1000,{.minSpeed=15},true);
    chassis.waitUntil(1.5);
    chassis.cancelMotion();

    chassis.moveToPoint(40,-35.5,2000,{.minSpeed=30,.earlyExitRange=0.5},false);
    chassis.turnToHeading(90,1500,{.minSpeed=10,.earlyExitRange=1},false);
    chassis.waitUntilDone();
    
    chassis.moveToPoint(12,-35.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=20,.earlyExitRange=1}); // max speed editable
    chassis.turnToHeading(135,1000,{.minSpeed=30,.earlyExitRange=5},true);
    toggleIntake(false,0);
}

void left_LongCenter(){
    chassis.setPose((48-2.25),-(15.75),270);

    chassis.moveToPoint(22,-22,1500,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=15},true);

    chassis.moveToPoint(22-2,-22-2,1000,{.maxSpeed=30,.minSpeed=10,.earlyExitRange=1},true);
    toggleIntake(true,0);
    pros::delay(300);
    toggleLoader(true);

    chassis.turnToHeading(135,750,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=10},true);

    // 2nd segment

    chassis.moveToPoint(48-10,-47-1,2000,{.minSpeed=10,.earlyExitRange=0},true); 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=0},false);

    // long goal first
    chassis.moveToPoint(29.5-2,-47.5-0.5,1300,{.forwards=false,.maxSpeed=80,.minSpeed=40,.earlyExitRange=1},true);
    chassis.waitUntil(20-10-2);
    toggleIntake(true,1); // score; might jam 

    pros::delay(700); 
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(300);

    // into loader
    chassis.moveToPoint(47,-46.5,1000,{.maxSpeed=80,.minSpeed=40,.earlyExitRange=2},true);
    chassis.moveToPoint(55+10,-47,800+50,{.maxSpeed=40,.minSpeed=30},true); 
    toggleIntake(true,0);

    // go center
    //chassis.moveToPoint(47-1,-47,1000,{.forwards=false,.maxSpeed=80,.minSpeed=10,.earlyExitRange=1},true);
    //chassis.turnToHeading(135,1000,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=5},false);

    chassis.moveToPoint(11,-10,2000+1000,{.forwards=false,.maxSpeed=90,.minSpeed=10},true);
    chassis.waitUntil(33.5+7+3);
    chassis.cancelMotion();

    chassis.moveToPoint(11-1,-10+1,1000,{.forwards=false,.maxSpeed=50,.minSpeed=10},true);
    toggleIntake(false,0);
    chassis.waitUntil(4);
    toggleIntake(true,-1);
    chassis.waitUntil(9); // 10 --> 9

    toggleIntake(true,2);

    chassis.waitUntil(10);

    pros::delay(1000); 
    
    /*chassis.moveToPoint(13,-12,500,{.minSpeed=30});
    toggleLoader(false);
    toggleIntake(true,-1);
    chassis.waitUntil(1);
    chassis.cancelMotion();
    chassis.moveToPoint(9+1,-8,500,{.forwards=false,.minSpeed=40});
    toggleIntake(false,0);*/

    toggleLoader(false);
    toggleIntake(false,0);
    

    // use wing
    chassis.moveToPoint(40-6,-34.5+0.5,2000,{.minSpeed=20+10},true);
    //chassis.waitUntil(0.5);
    //setScoreHeight(true);
    chassis.turnToHeading(90,1500,{.minSpeed=10,.earlyExitRange=5},true);
    toggleIntake(false,0);

    
    chassis.moveToPoint(12,-34.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); // max speed editable
    chassis.turnToHeading(135-30,1000,{.minSpeed=30,.earlyExitRange=2},true);
    toggleIntake(false,0);
}

void left_Long9(){ //
    chassis.setPose(38.5+8,-(4+13.625/2.0),270);

    chassis.moveToPoint(22,-22,1500,{.minSpeed=20,.earlyExitRange=15},true);
    chassis.moveToPoint(22,-22,1300,{.maxSpeed=30,.minSpeed=30},true);
    toggleIntake(true,0);

    chassis.moveToPoint(14,-30,1000,{.maxSpeed=80});
    chassis.moveToPoint(8,-42,2000,{.maxSpeed=50,.minSpeed=15},false);
    toggleLoader(true);

    //chassis.moveToPose(9.25,-42,200,5000,{.maxSpeed=120,.minSpeed=30},true);
    //chassis.moveToPose(7,-42-3-3,210,5000,{.maxSpeed=120,.minSpeed=30},true);
    //chassis.waitUntilDone();
    //pros::delay(10000);

    chassis.moveToPoint(36,-47,2000,{.forwards=false,.maxSpeed=80,.minSpeed=20});

    //chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},false);

    // 2nd segment

    //toggleLoader(false);
    //chassis.moveToPoint(48,-47.5,2000,{.minSpeed=10,.earlyExitRange=0.5},true); // accurate?
    
    chassis.turnToHeading(90,1000+500,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=2},true);

    // then, match load
    toggleLoader(true);
    pros::delay(200);

    chassis.moveToPoint(55+5,-47,900,{.maxSpeed=50,.minSpeed=40},true); 
    
    toggleIntake(true, 0);
    //chassis.waitUntil(24);
    //pros::delay(1000); // adj for 3 blocks??  // cut loader time

    // score again
    chassis.moveToPoint(29.5-2,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(20);
    toggleIntake(true,1); // score 
    toggleLoader(false);  

    pros::delay(1100); // wait
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(300+900);

    // use wing
    // 10.5 displaced
    chassis.moveToPoint(39,-38-1,1000,{.minSpeed=30},true);
    toggleIntake(false,0);
    chassis.turnToHeading(90,750,{.minSpeed=15,.earlyExitRange=2},false);
    chassis.moveToPoint(15,-38-1,2000,{.forwards=false,.maxSpeed=90,.minSpeed=0}); // change max speed
}

void left_Long7(){ //
    chassis.setPose((48-2.25),-(15.75),270);

    chassis.moveToPoint(22,-22,1500,{.minSpeed=20,.earlyExitRange=15},true);
    chassis.moveToPoint(22,-22,1300,{.maxSpeed=30,.minSpeed=30},true);
    toggleIntake(true,0);
    toggleLoader(true);

    chassis.turnToHeading(135,1000,{.maxSpeed=80,.minSpeed=15,.earlyExitRange=10},false);

    // 2nd segment

    //toggleLoader(false);
    chassis.moveToPoint(48,-47.5,2000,{.minSpeed=10,.earlyExitRange=0.5},true); // accurate?
    
    chassis.turnToHeading(90,1000+500,{.maxSpeed=80,.minSpeed=15,.earlyExitRange=0},false);
    pros::delay(200);

    // then, match load

    chassis.moveToPoint(55+5,-47,900,{.maxSpeed=50,.minSpeed=40},true); 
    
    toggleIntake(true, 0);
    //chassis.waitUntil(24);
    pros::delay(500); // adj for 3 blocks??  // cut loader time

    // score again
    chassis.moveToPoint(29.5-2,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(20);
    toggleIntake(true,1); // score 
    toggleLoader(false);  

    pros::delay(1100); // wait
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(900);

    // use wing
    // 10.5 displaced
    chassis.moveToPoint(39,-38-1,1000,{.minSpeed=40},true);
    toggleIntake(false,0);
    chassis.turnToHeading(90,750,{.minSpeed=15+10,.earlyExitRange=2+3},false);
    chassis.moveToPoint(15-5,-38-1,5000,{.forwards=false,.maxSpeed=90,.minSpeed=30}); // change max speed
    chassis.turnToHeading(90+20, 1000, {.minSpeed=20,.earlyExitRange=5}, false);
}

void test(){
    chassis.setPose(0,0,0);
    chassis.turnToHeading(90, 5000);
    // test rotate
    //chassis.turnToHeading(90,5000,{},true);

    // test translations
    //chassis.moveToPoint(0,24,5000);


    // TEST TRAVERSING AROUND THE FIELD
}

void autonomous() {
    //pros::Task antiJam(antiJamTask);
    //antiJam.remove();

    //right_Long7(); //done
    //right_LowerLong(); //done
    //right_LongLower();
    
    //left_CenterLong();
    left_LongCenter(); //done
    //left_Long9();
    //left_Long7(); //done
    //test();  
}


// pseudocode for odom/pp/boomerang

double x_global = 0;
double y_global = 0;
double theta_global = 0;

double prev_heading = 0;
double prev_rotation = 0;

void odom(){
    while(true){
        double current_heading = inertial.get_heading();
        double delta_heading = (current_heading - prev_heading) * M_PI / 180.0; // needs to be in radians for trig
        prev_heading = current_heading; // save current heading as prev for next loop

        double current_rotation = vertPod.get_position()/100.0; // centidegrees --> degrees
        double delta_rotation = current_rotation - prev_rotation;
        prev_rotation = current_rotation; // save current rotation reading as prev for next loop

        double distance_travelled = delta_rotation/360.0 * 2.0 * std::numbers::pi * 1.0; // 2 * pi * 1 inch radius

        double radius = distance_travelled / delta_heading; // in inches

        double dx_local = radius * (sin(delta_heading));
        double dy_local = radius * (1 - cos(delta_heading));

        double dx_global = dx_local * cos(theta_global) - dy_local * sin(theta_global);
        double dy_global = dx_local * sin(theta_global) + dy_local * cos(theta_global);

        // update global coordinates and heading
        x_global += dx_global;
        y_global += dy_global;
        theta_global = inertial.get_heading();

        pros::Task::delay(20); 
    }
}

//HELPER FCT
int drivePID(int goal_distance){
    static double kP = 0.2;
    static double kD = 2;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    error = goal_distance - getAverageEncoderVal();

    double power = error*kP + (error - prevError)*kD;

    if (power > 100) power = 100;

    return power;
}

double track_width = 11.5;
double time_elapsed = 0;
void move_to_point(double goal_x, double goal_y, double timeout, double early_exit_range){
    time_elapsed = 0; // reset time elapsed at start of movement

    double total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2))); // also equal to lookahead distance

    double error_x = goal_x - x_global;
    double error_y = goal_y - y_global;

    double target_angle = atan2(error_y, error_x); // radians
    double alpha = target_angle - theta_global;
    alpha = atan2(sin(alpha), cos(alpha)); // into [-pi, pi]

    double curvature = (2.0 * sin(alpha)) / (total_error); // radius of the circular path to follow

    double direct_power;
    double left_power;
    double right_power;

    while(total_error >= early_exit_range && time_elapsed < timeout){
        // update errors
        error_x = goal_x - x_global;
        error_y = goal_y - y_global; 
        total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2)));

        target_angle = atan2(error_y, error_x);
        theta_global = inertial.get_heading() * M_PI / 180.0; // convert to radians
        alpha = target_angle - theta_global;
        alpha = atan2(sin(alpha), cos(alpha));
    
        curvature = (2.0 * sin(alpha)) / total_error;

        // calculate power
        direct_power = drivePID(total_error); // [0, 100]

        left_power = direct_power * (1.0 - (curvature * track_width)/2.0);
        right_power = direct_power * (1.0 + (curvature * track_width)/2.0);
        
        driveLeft.move(left_power);
        driveRight.move(right_power);

        pros::Task::delay(15);
        time_elapsed += 15;
    }
}

double kTheta = 2;

void move_to_pose(double goal_x, double goal_y, double goal_theta, double lead_distance, double timeout, double early_exit_range){
    time_elapsed = 0; // reset time elapsed at start of movement

    double total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2))); // also equal to lookahead distance

    double error_x = goal_x - x_global;
    double error_y = goal_y - y_global;

    /*double error_theta = goal_theta - theta_global;
    error_theta = atan2(sin(error_theta), cos(error_theta));*/

    double carrot_x = goal_x - (total_error * lead_distance * sin(goal_theta)); // NEW
    double carrot_y = goal_y - (total_error * lead_distance * cos(goal_theta)); // NEW

    double carrot_error = sqrt((pow(carrot_x - x_global, 2) + pow(carrot_y - y_global, 2)));; // NEW

    double target_angle = atan2(carrot_y - y_global, carrot_x - x_global); // radians
    double alpha = target_angle - theta_global;
    alpha = atan2(sin(alpha), cos(alpha)); // into [-pi, pi]

    double curvature = (2.0 * sin(alpha)) / (carrot_error); // radius of the circular path to follow

    double direct_power;
    double left_power;
    double right_power;
    //double turn_power;

    while(total_error >= early_exit_range && time_elapsed < timeout){
        // update errors
        error_x = goal_x - x_global;
        error_y = goal_y - y_global; 
        total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2))); // TO GOAL POINT
        
        theta_global = inertial.get_heading() * M_PI / 180.0; // convert to radians
        //error_theta = goal_theta - theta_global;
        //error_theta = atan2(sin(error_theta), cos(error_theta));
        
        // update carrot point + errors
        carrot_x = goal_x - (total_error * lead_distance * sin(goal_theta)); //
        carrot_y = goal_y - (total_error * lead_distance * cos(goal_theta)); //

        carrot_error = sqrt((pow(carrot_x - x_global, 2) + pow(carrot_y - y_global, 2))); // TO CARROT POINT

        target_angle = atan2(carrot_y - y_global, carrot_x - x_global); // radians
        alpha = target_angle - theta_global;
        alpha = atan2(sin(alpha), cos(alpha)); // into [-pi, pi]

        curvature = (2.0 * sin(alpha)) / (carrot_error); // radius of the circular path to follow
        if (curvature > 1.0) curvature = 1.0;
        if (curvature < -1.0) curvature = -1.0;

        // calculate power
        direct_power = drivePID(carrot_error); // [0, 100]  edited

        // heading correction (should not break curvature)
        //turn_power = kTheta * error_theta;

        left_power = direct_power * (1.0 - (curvature * track_width)/2.0); //- turn_power;
        right_power = direct_power * (1.0 + (curvature * track_width)/2.0); //+ turn_power;
        
        driveLeft.move(left_power);
        driveRight.move(right_power);
    }

        pros::Task::delay(15);
        time_elapsed += 15;
    }