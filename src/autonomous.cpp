#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include <cmath>

//ASSET(testblueright_txt);


int delayTask = 190;
int delayIntake = 200;

void colorIsBlue(bool blue){
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
    
}

void antiJamTask(){
    int stallCounter = 0; // ms
    int reversalTime = 400; // ms

    while(true){
        if(intakeState != 0){ // if intake isnt supposed to be jamming
            if(fabs(upperIntake.get_actual_velocity()) < 100 || fabs(lowerIntake.get_actual_velocity()) < 100){ // rpm
                stallCounter += 15;
            }else{
                stallCounter = 0;
            }
            
            if(stallCounter > 400){ // in ms
                // jam detected
                if(intakeState == 1){ // intake forward
                    lowerIntake.move(-70);
                    upperIntake.move(-70);
                    pros::Task::delay(reversalTime);
                    lowerIntake.move(127);
                    upperIntake.move(127);
                }else if(intakeState == -1){ // intake reverse
                    lowerIntake.move(70);
                    upperIntake.move(70);
                    pros::Task::delay(reversalTime);
                    lowerIntake.move(-70);
                    upperIntake.move(-70);
            }
        }
        pros::Task::delay(15);
        }
    }
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

void basic(){
    chassis.setPose(0,0,0);

    
    chassis.moveToPoint(0,6,5000,{.maxSpeed = 60});

    //chassis.moveToPose(0,24,0,8000, {});

    //chassis.moveToPose(0,0,0,8000, {.forwards = false});

   // STOP AFTER A TIME TO NOT BURN OUT THE MOTORS
}

void pidtuning(){
    chassis.setPose(0,0,0);

    //chassis.moveToPoint(0,24,5000);
    chassis.turnToHeading(90, 5000);
    chassis.turnToHeading(0, 5000);

}

void skills(){ //full field; not done
    /*chassis.setPose(62,-16.427,270); // left center starting pt

    chassis.moveToPoint(22,-24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    toggleIntake(true,0);
    chassis.moveToPoint(22,-22,1500,{.maxSpeed=30},true);
    toggleLoader(true);

    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    toggleLoader(false);

    // 1: SCORE ON CENTER UPPER
    chassis.moveToPose(10,-10,135,1000,{.forwards=false,.maxSpeed=60,.minSpeed=20,.earlyExitRange=1},true);
    
    chassis.waitUntil(4);
    toggleIntake(true,0);

    chassis.waitUntil(12);
    toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(1200); 
   

    // to lower center goal
    chassis.moveToPoint(22,-22,1500,{.forwards=true,.maxSpeed=60,.minSpeed=20,.earlyExitRange=2},false);
    chassis.turnToHeading(0,1000,{.maxSpeed=80,.earlyExitRange=5});
    chassis.moveToPoint(22,22,2000,{},true);
    setScoreHeight(true);
    toggleIntake(true,0);
    chassis.waitUntilDone();
    toggleLoader(true);

    // 2: SCORE ON CENTER LOWER
    chassis.turnToHeading(225,1000,{.maxSpeed=80,.earlyExitRange=5},false);
    chassis.moveToPoint(10,10,1000,{},true);
    toggleLoader(false);
    toggleIntake(true,-1);
    chassis.waitUntilDone();
    //chassis.waitUntil(10);
    //toggleIntake(true,-1);

    pros::delay(1200);*/


    // taken from left AWP
    chassis.setPose(47.5,-16.427,180);


    // 2nd segment - match loader

    chassis.moveToPoint(47.5,47,2000,{.forwards=false,.earlyExitRange=0},true); 
    chassis.waitUntil(16);
    toggleIntake(false,0); // reset intake 
    
    chassis.turnToHeading(90,1500,{.earlyExitRange=5},false);

    toggleLoader(true);
    pros::delay(300);

    // 3: INTO LOADER - QUADRANT I
    chassis.moveToPoint(55+10,47,2000,{.minSpeed=65},true); 
    
    toggleIntake(true, 0);

    pros::delay(2500); // 3 blocks = 1200 ms

    // go to other side
    chassis.moveToPoint(29.5,60,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    toggleLoader(false);
    chassis.moveToPoint(-29.5,60,10000,{.forwards=false,.minSpeed=30},false);

    chassis.moveToPoint(-48,47,2000,{.forwards=false,.minSpeed=30},false);

    // 4: SCORE IN LONG GOAL - QUADRANT II
    chassis.turnToHeading(270,1000,{.maxSpeed=80,.earlyExitRange=5},false);
    chassis.moveToPoint(-29.5-2,47,1600,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(3000);
    chassis.setPose(-29.5,47,chassis.getPose().theta); // fake reset

    toggleIntake(false,0);

    // 5: QUADRANT II LOADER
    toggleLoader(true);
    pros::delay(300);

    chassis.moveToPoint(-55-5,47,2500,{.minSpeed=65},true); 
    
    toggleIntake(true, 0);

    pros::delay(2000); // 3 blocks = 1200 ms
    
    // 6: SCORE AGAIN QUADRANT II LONG GOAL
    chassis.moveToPoint(-29.5,47,1600,{.forwards=false,.minSpeed=30},true);
    toggleLoader(false);
    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2500);

    // section 3
    // 7: TRAVERSE OTHER SIDE, OTHER LONG GOAL
    chassis.moveToPoint(-36,47,2000,{.minSpeed=30},true);
    toggleIntake(true,-1); 
    chassis.turnToHeading(180,1000,{.maxSpeed=80,.earlyExitRange=5},true);
    toggleIntake(true,1);
    chassis.moveToPoint(-36,-47,10000,{.minSpeed=30},false);

    // 8: INTO LOADER, QUADRANT III
    chassis.turnToHeading(270,1200,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},false);
    toggleLoader(true);
    pros::delay(200);
    toggleIntake(true,0);

    chassis.moveToPoint(-55-10,-47,2500,{.minSpeed=65},true); 

    pros::delay(2000); // 3 blocks = 1200 ms

    // mirror, into QIV 


    // 9: TRAVERSE TO QUADRANT IV
    chassis.moveToPoint(-29.5,-60,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    toggleLoader(false);
    
    chassis.moveToPoint(29.5,-60,10000,{.forwards=false,.minSpeed=30},true);
    toggleIntake(false,0);

    chassis.moveToPoint(48,-47,2000,{.forwards=false,.minSpeed=30},true);

    // 10: SCORE IN LONG GOAL - QUADRANT IV
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    chassis.moveToPoint(29.5-2,-47,2000,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2000);
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    toggleIntake(false,0);

    // 11: QUADRANT IV LOADER
    toggleLoader(true);
    pros::delay(300);

    chassis.moveToPoint(55+5,-47,2500,{.minSpeed=65},true); 
    
    toggleIntake(true, 0);

    pros::delay(2000); // 3 blocks = 1200 ms

    // 12: SCORE AGAIN QUADRANT IV LONG GOAL
    chassis.moveToPoint(29.5-1,-47,2000,{.forwards=false,.minSpeed=30},true);
    toggleLoader(false);
    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2500);

    // 13: BACK TO PARKING
    chassis.moveToPoint(40,-47,2000,{.forwards=true,.minSpeed=40,.earlyExitRange=3},false);
    chassis.moveToPose(62,-18,0,4000,{.minSpeed=40},false);
    chassis.moveToPoint(62,0,2000,{.minSpeed=40},false);
    // keep intake on until end...

}

void auton_skills(){
    // 1: taken from left AWP
    chassis.setPose(47.5,16.427,0);


    // 2: towards QI match loader

    chassis.moveToPoint(47.5,47,1500,{.earlyExitRange=0},true); 
    chassis.waitUntil(16);
    
    chassis.turnToHeading(90,1500,{.earlyExitRange=5},false);

    toggleLoader(true);
    pros::delay(500);

    // 3: INTO LOADER - QUADRANT I
    chassis.moveToPoint(55+10,47,1500,{.minSpeed=50},true); 
    
    toggleIntake(true, 0);

    chassis.moveToPoint(55+10,47,500,{.minSpeed=50},false);
    chassis.moveToPoint(50,47,200,{.forwards=false,.minSpeed=50},false);
    //pros::delay(500);
    chassis.moveToPoint(55+10,47,500,{.minSpeed=50},false); 
    chassis.moveToPoint(50,47,200,{.forwards=false,.minSpeed=50},false);
    chassis.moveToPoint(55+10,47,1250,{.minSpeed=50},false); 
    //pros::delay(500);
    //pros::delay(3000); // 3 blocks = 1200 ms


    // go to other side
    chassis.moveToPoint(50,47,1500,{.forwards=false,.minSpeed=30},true);
    toggleLoader(false);
    pros::delay(800);
   
    chassis.turnToHeading(180,1500,{.earlyExitRange=5},false);

    /*chassis.moveToPoint(50,72,1500,{.forwards=false,.maxSpeed=60,.minSpeed=30},true);
    chassis.waitUntilDone();
    chassis.setPose(50,62,chassis.getPose().theta); // reset on field wall */

    chassis.moveToPoint(50,34,2500,{.minSpeed=30},false);
    chassis.turnToHeading(90,2000,{.minSpeed=10},false);

    chassis.moveToPoint(29.5,34,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    
    //chassis.turnToHeading(90,2000,{.earlyExitRange=5},false);

    //chassis.moveToPoint(-29.5,60,10000,{.forwards=false,.maxSpeed=80,.minSpeed=30},false);
    chassis.moveToPoint(-47,34,10000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=2},false);

    chassis.turnToHeading(0,2000,{.earlyExitRange=5},false);
    chassis.moveToPoint(-47,47,2500,{.minSpeed=30},false);

    // 4: SCORE IN LONG GOAL - QUADRANT II
    chassis.turnToHeading(270,2000,{.maxSpeed=80},false);
    chassis.moveToPoint(-29.5+6,47,1600,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    chassis.waitUntilDone();
    //pros::delay(3000);
    chassis.setPose(-29.5,47,chassis.getPose().theta); // fake reset

    toggleIntake(false,0);

    // 5: QUADRANT II LOADER
    toggleLoader(true);
    pros::delay(500);

    chassis.moveToPoint(-55-5,47,1500,{.minSpeed=65},true); 
    
    toggleIntake(true, 0);

    chassis.moveToPoint(-50,47,200,{.forwards=false,.minSpeed=50},false);
    chassis.moveToPoint(-55-10,47,500,{.minSpeed=50},false); 
    chassis.moveToPoint(-50,47,200,{.forwards=false,.minSpeed=50},false);
    chassis.moveToPoint(-55-10,47,1250,{.minSpeed=50},false); 
    //pros::delay(2000); // 3 blocks = 1200 ms
    
    // 6: SCORE AGAIN QUADRANT II LONG GOAL
    chassis.moveToPoint(-29.5+2,47,2000,{.forwards=false,.minSpeed=30},true);
    toggleLoader(false);
    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2500);

    // section 3
    // 7: TRAVERSE OTHER SIDE, OTHER LONG GOAL
    chassis.moveToPoint(-36-4,47,2000,{.minSpeed=30},true);
    toggleIntake(true,-1); 
    chassis.turnToHeading(180,2000,{.maxSpeed=80,.earlyExitRange=5},true);
    //toggleIntake(true,1);
    toggleIntake(false,0);
    chassis.moveToPoint(-36-4,-47,6000,{.minSpeed=30,.earlyExitRange=36},false);
    chassis.moveToPoint(-36-4,-47,6000,{.maxSpeed=65,.minSpeed=30},false);

    // 8: INTO LOADER, QUADRANT III
    chassis.turnToHeading(270,2000,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=5},false);
    toggleLoader(true);
    pros::delay(500);
    toggleIntake(true,0);

    chassis.moveToPoint(-55-10,-47,1500,{.minSpeed=65},true); 

    pros::delay(500);
    chassis.moveToPoint(-55-10,-47,500,{.minSpeed=65},true); 
    pros::delay(500);
    chassis.moveToPoint(-55-10,-47,500,{.minSpeed=65},true); 
    pros::delay(500);

    //pros::delay(2000); // 3 blocks = 1200 ms

    // mirror, into QIV 

    // 9: TRAVERSE TO QUADRANT IV
    //chassis.moveToPoint(-29.5,-60,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    chassis.moveToPoint(-55,-47,2000,{.minSpeed=30},true);
    toggleLoader(false);
    pros::delay(500);
    chassis.moveToPoint(-29.5,-36,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    
    //chassis.moveToPoint(29.5,-60,10000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true);
    toggleIntake(false,0);
    //chassis.moveToPoint(48,-60,10000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true);
    chassis.moveToPoint(48,-36,10000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=12},true);
    chassis.moveToPoint(48,-36,10000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true);
    chassis.turnToHeading(0,2000,{.earlyExitRange=5},false);

    chassis.moveToPoint(48,-47,5000,{.forwards=false,.minSpeed=30},true);

    // 10: SCORE IN LONG GOAL - QUADRANT IV
    chassis.turnToHeading(90,2000,{.maxSpeed=80,.earlyExitRange=0},false);
    chassis.moveToPoint(29.5-6,-47,2000,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    // 11: QUADRANT IV LOADER
    toggleLoader(true);
    pros::delay(2000);
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    toggleIntake(false,0);
    pros::delay(500);

    chassis.moveToPoint(55+5,-47,1500,{.minSpeed=65},true); 
    
    toggleIntake(true, 0);

    pros::delay(500);
    chassis.moveToPoint(55+5,-47,500,{.minSpeed=65},true); 
    pros::delay(500);
    chassis.moveToPoint(55+5,-47,500,{.minSpeed=65},true); 
    pros::delay(500);
    //pros::delay(2000); // 3 blocks = 1200 ms

    // 12: SCORE AGAIN QUADRANT IV LONG GOAL
    chassis.moveToPoint(29.5-1,-47,2000,{.forwards=false,.minSpeed=30},true);
    toggleLoader(false);
    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2500);

    // 13: BACK TO PARKING
    chassis.moveToPoint(40,-47,2000,{.forwards=true,.minSpeed=40,.earlyExitRange=3},false);
    chassis.moveToPose(62,-18,0,4000,{.minSpeed=40},true);
    chassis.waitUntil(30);
    toggleLoader(true);
    chassis.moveToPoint(62,0,2000,{.minSpeed=50},false);
    // keep intake on until end...
}

void leftAuto_CenterGoal(){ // !
    chassis.setPose(62,-16.427,270);

    chassis.moveToPoint(22,-22,1500,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=15},true);

    chassis.moveToPoint(22,-22+1,1300,{.maxSpeed=30,.minSpeed=10},true);
    toggleIntake(true,0);
    toggleLoader(true);
    //chassis.waitUntilDone();
    //pros::delay(500);

    chassis.turnToHeading(135,750,{.maxSpeed=80,.earlyExitRange=10},true);
    toggleIntake(false,0);
    chassis.waitUntilDone();
    //toggleLoader(false);

    /*chassis.moveToPose(10.5,-10.5,135,1000,{.forwards=false,.maxSpeed=60,.minSpeed=20,.earlyExitRange=0},true);
    toggleIntake(false,0);
    chassis.waitUntil(16); //
    toggleIntake(true,1);
    setScoreHeight(false);
    pros::delay(1000); // increased time 800 --> 1000 ms*/

    chassis.moveToPoint(11.5,-10,1000,{.forwards=false,/*.maxSpeed=60,*/.minSpeed=10},true);
    chassis.waitUntil(7);
    toggleIntake(true,-1);

    chassis.waitUntil(9); // 10 --> 9
    //toggleIntake(true,0);
    toggleIntake(true,1,70);

    //toggleLoader(false);

    chassis.waitUntil(12);
    //toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(1600); // increased time 750 --> 1500 ms

    // 2nd segment

    chassis.moveToPoint(48,-47+1,2400,{.earlyExitRange=0},true); // accurate?
    pros::delay(200);
    toggleIntake(true,-1);
    chassis.waitUntil(20);
    setScoreHeight(true);
    toggleIntake(false,0); // reset intake 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10/*,.earlyExitRange=5*/},false);

    toggleLoader(true);
    pros::delay(400);

    // into loader
    chassis.moveToPoint(55+10,-47,1000,{.maxSpeed=70,.minSpeed=50},true); 
    
    toggleIntake(true, 0);

    //pros::delay(1200); // ~optimal intake delay (3 blocks)
    pros::delay(1000); // adj for 3 blocks??  // cut loader time

    chassis.moveToPoint(29.5-2,-47,2000,{.forwards=false,.minSpeed=40,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  

    
    pros::delay(1000); 
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(200);

    // use wing
    chassis.moveToPoint(48,-47,1000,{},true);
    chassis.waitUntil(1.5);
    chassis.cancelMotion();

    chassis.moveToPoint(40,-34.5,2000,{.minSpeed=30},false);
    chassis.turnToHeading(90,1500,{},false);
    chassis.waitUntilDone();
    //chassis.setPose(39,-35,chassis.getPose().theta); // reset
    
    chassis.moveToPoint(12,-34.5,2000,{.forwards=false,.maxSpeed=90,.minSpeed=0}); // max speed editable
    chassis.turnToHeading(135,1000,{},true);
    toggleIntake(false,0);

}

void leftAuto_AWP(){ // 
    chassis.setPose(47.5,-16.427,180);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5 /*48*/,-48,1500,{.maxSpeed=90,.minSpeed=10,.earlyExitRange=1});

    toggleIntake(true,0);
    

    chassis.turnToHeading(90,1000,{.maxSpeed=70,.minSpeed=20,.earlyExitRange=10});


    toggleLoader(true);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+10,-47,1200,{.maxSpeed=70,.minSpeed=50},true); 
   
    pros::delay(850+150); // ~optimal intake delay (3 blocks)

    // long goal score
    chassis.moveToPoint(29.5-3,-47,1500,{.forwards=false,.maxSpeed=70,.minSpeed=40},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);
    toggleLoader(false);

    pros::delay(1700);
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    toggleIntake(false,0);


    // section 2 (center)

    chassis.moveToPoint(36,-47,1000,{.maxSpeed=70,.minSpeed=30,.earlyExitRange=2},true);
    toggleIntake(true,-1); // outtake in case of jam

    chassis.turnToHeading(90,1000,{.maxSpeed=70,.minSpeed=30,.earlyExitRange=10});

    chassis.moveToPoint(22,-22,1500,{.maxSpeed=60,.minSpeed=30,.earlyExitRange=1},true);
    
    chassis.waitUntil(16);
    toggleIntake(true,0);

    chassis.waitUntil(22);
    toggleLoader(true);
    
    chassis.turnToHeading(135,1000,{.earlyExitRange=10},false);
    toggleLoader(false);
    toggleIntake(false,0);

    //center goal

    //chassis.moveToPose(10,-10,135,1000,{.forwards=false,.maxSpeed=60,.minSpeed=20,.earlyExitRange=1},true);
    chassis.moveToPoint(11.5,-10,1000,{.forwards=false,.maxSpeed=60,.minSpeed=10},true);
    //chassis.waitUntil(2);
    toggleIntake(true,0);

    chassis.waitUntil(10);
    toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(1200); // increased time 750 --> 1500 ms


    // section 3 (other goal)

    chassis.moveToPoint(22,-22,1000,{.maxSpeed=70,.minSpeed=40,.earlyExitRange=2},true); 
    toggleIntake(false,0);

    chassis.turnToHeading(0,1000,{.minSpeed=30,.earlyExitRange=10},true);
    setScoreHeight(true);

    //
    chassis.moveToPoint(22,22,1000,{.maxSpeed=70,.minSpeed=40,.earlyExitRange=12-2},true); // other 3 balls
    toggleIntake(true,0);

    chassis.moveToPoint(22,22+3,1000,{.maxSpeed=50,.minSpeed=30,.earlyExitRange=3},true);
    //chassis.waitUntil(6);
    //toggleLoader(true);

    chassis.moveToPose(47,47-0.5,90,1500,{.lead=0.15,.maxSpeed=70,.minSpeed=0,.earlyExitRange=0});
    toggleLoader(false); //

    // long goal score
    chassis.moveToPoint(29.5,47-0.5,1600,{.forwards=false,.maxSpeed=70,.minSpeed=0},true);


    chassis.waitUntil(10);
    toggleIntake(true,1);
    toggleLoader(false);
}

void rightAuto_AWP(){
    chassis.setPose(47.5,16.427,0);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5 /*48*/,48,3000,{.earlyExitRange=0});

    toggleIntake(true,0);
    

    chassis.turnToHeading(90,1000,{.minSpeed=20,.earlyExitRange=10});


    toggleLoader(true);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+10,47,1000,{.maxSpeed=50,.minSpeed=20},true); 
   
    //pros::delay(300); // ~optimal intake delay (3 blocks)

    // long goal score
    chassis.moveToPoint(29.5-3,47,1500,{.forwards=false,.minSpeed=40},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);
    toggleLoader(false);

    pros::delay(1200); // cut extra time
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    pros::delay(250);
    //toggleIntake(false,0);


    // section 2 (center)

    chassis.moveToPoint(36,47,1000,{.minSpeed=30,.earlyExitRange=2},true);

    chassis.turnToHeading(225,1000,{.minSpeed=30,.earlyExitRange=10});
    toggleIntake(true,-1); // outtake in case of jam

    chassis.moveToPoint(22,22,1500,{.maxSpeed=60,.minSpeed=20,.earlyExitRange=1},true);
    toggleIntake(true,1);
    
    chassis.waitUntil(16);
    toggleIntake(true,0);

    chassis.waitUntil(22);
    //toggleLoader(true);
    
    chassis.turnToHeading(180,1000,{.minSpeed=10},true);
    
    //toggleLoader(false);

    chassis.moveToPoint(22+2,-22,2000,{.maxSpeed=90,.minSpeed=10,.earlyExitRange=1},true);
    chassis.waitUntil(10);
    toggleIntake(true,-1);
    chassis.waitUntil(16);
    toggleIntake(false,0);
    chassis.waitUntil(24);
    toggleIntake(true,0);

    chassis.waitUntilDone();
    toggleLoader(true);

    chassis.turnToHeading(135, 1000,{.earlyExitRange=5});
    toggleIntake(true,-1);
    pros::delay(250);
    toggleIntake(true,0,80);

/*
    chassis.moveToPoint(12,-10,1000,{.forwards=false,.maxSpeed=70,.minSpeed=10},true);
    toggleIntake(true,0,100);
    chassis.waitUntil(10);
    //toggleIntake(true,0,100); // score on center
    setScoreHeight(false);
    pros::delay(1500); // increased time 750 --> 1500 ms*/

    // this is left auto unjam strat
    chassis.moveToPoint(12,-10,1000,{.forwards=false,.maxSpeed=70,.minSpeed=10},true);
    toggleIntake(false,0);
    chassis.waitUntil(6);
    toggleIntake(true,-1);

    chassis.waitUntil(9);
    toggleIntake(true,1,100);
    chassis.waitUntil(10);
    setScoreHeight(false);
    pros::delay(1500); // increased time 750 --> 1500 ms



    // section 3 (other goal)

    chassis.moveToPoint(47,-47,1100,{.maxSpeed=120,.minSpeed=10,.earlyExitRange=0.5},true);  // early exit?
    toggleIntake(true,-1);
    toggleLoader(false);
    chassis.waitUntil(12);
    toggleIntake(false,0);
    setScoreHeight(true);

    chassis.turnToHeading(90,1000,{.minSpeed=30,.earlyExitRange=5});
    toggleLoader(true);
    toggleIntake(true,0);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+10,-47+2.5-1.5,850,{.maxSpeed=70,.minSpeed=50},true); 
   
    pros::delay(1000); // loader delay

    // long goal score
    chassis.moveToPoint(29.5-2,-47+2.5-1.5,5000,{.forwards=false,.maxSpeed=100,.minSpeed=30},true);

    chassis.waitUntil(13);
    toggleIntake(true,1);
    toggleLoader(false);
    chassis.moveToPoint(32,-47+2.5,1500,{.forwards=true,.minSpeed=50}); // queue
    pros::delay(2500); // should tune if i need to
    chassis.cancelMotion();
}

void cheese_skills(){ // just a more complex left auto
    chassis.setPose(62,-16.427,270);

    chassis.moveToPoint(22,-24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    //toggleIntake(true,0);
    chassis.moveToPoint(22,-22,1300,{.maxSpeed=30,.minSpeed=10},true);
    toggleIntake(true,0);
    toggleLoader(true);



    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},true);
    toggleIntake(false,0);
    chassis.waitUntilDone();
    toggleLoader(false);
    //chassis.setPose(22,-22,chassis.getPose().theta); // fake reset

    chassis.moveToPoint(11.5-1,-10+1,1000,{.forwards=false,.maxSpeed=60,.minSpeed=10},true);
    chassis.waitUntil(4);
    toggleIntake(true,0);

    chassis.waitUntil(12);
    toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(3000); // increased time 750 --> 1500 ms
    
    // 2nd segment

    chassis.moveToPoint(48,-47,2000,{.earlyExitRange=0},true); // accurate?
    pros::delay(200);
    toggleIntake(true,-1);

    chassis.waitUntil(16);
    setScoreHeight(true);
    toggleIntake(false,0); // reset intake 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},false);
    //setScoreHeight(true);
    //chassis.waitUntilDone();

    //chassis.setPose(48,-47,chassis.getPose().theta); // correct drift ?

    toggleLoader(true);
    pros::delay(400);

    // into loader
    chassis.moveToPoint(55+5,-47,1000,{.minSpeed=60},true); 
    
    toggleIntake(true, 0);

    pros::delay(900+200); // adj for 3 blocks??  // cut loader time

    chassis.moveToPoint(29.5,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  

    
    pros::delay(1100+1000); 
    //chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(200);

    // loader 2nd time
    chassis.moveToPoint(55+5,-47,3000,{.minSpeed=60},true); 
    
    toggleIntake(true, 0);

    //pros::delay(1200); // ~optimal intake delay (3 blocks)
    pros::delay(900+200+200); // adj for 3 blocks??  // cut loader time

    chassis.moveToPoint(29.5,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  

    
    pros::delay(1100+1000); 
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(200);

    // use wing
    chassis.moveToPoint(39,-37.5,1000,{.minSpeed=30},false);
    chassis.turnToHeading(90,750,{.minSpeed=15},false);
    chassis.moveToPoint(10,-37.5,2000,{.forwards=false,.maxSpeed=60,.minSpeed=0}); // can change to poses

    pros::delay(1000);

    // towards park
    chassis.moveToPoint(40,-37.5,3000); 

    chassis.turnToHeading(0,2000);

    chassis.moveToPoint(40,0,3000);

    chassis.turnToHeading(90,2000); 

    toggleIntake(true,1);
    chassis.moveToPoint(65,0,5000,{.minSpeed=40});




}

void halfField_skills(){
    chassis.setPose(62,-16.427,270); // left auto starting pt
    
    chassis.moveToPoint(24,-24,1500,{.minSpeed=20,.earlyExitRange=15},true);

    chassis.moveToPoint(24,-24,1300,{.maxSpeed=30,.minSpeed=10},true);
    toggleIntake(true,0);
    toggleLoader(true);
    
    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},true);
    toggleIntake(false,0);
    chassis.waitUntilDone();
    toggleLoader(false);
    
    // 1 score on center upper
    chassis.moveToPoint(11.5-1,-10+1,1500,{.forwards=false,.maxSpeed=60,.minSpeed=10},true);
    chassis.waitUntil(4);
    toggleIntake(true,0);
    
    chassis.waitUntil(12);
    toggleIntake(true,1); // score
    setScoreHeight(false);
    pros::delay(3000); // no need to rush
    
    // 2 long goal, near
    chassis.moveToPoint(48,-47,2000,{.earlyExitRange=0},true); // accurate?
    pros::delay(200);
    toggleIntake(true,-1);
    
    chassis.waitUntil(16);
    setScoreHeight(true);
    toggleIntake(false,0); // reset intake 
        
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},false);
    
    // 3 match loader
    toggleLoader(true);
    pros::delay(400);
    
    chassis.moveToPoint(55+5,-47,1000,{.minSpeed=60},true); 
        
    toggleIntake(true, 0);
    
    pros::delay(900+200); // adj for 3 blocks??  // cut loader time
    
    // 4 score long goal
    chassis.moveToPoint(29.5-2,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam 
    //toggleLoader(false);  
    
    pros::delay(1500); 
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    //toggleLoader(true);
    pros::delay(200);
    
    // 5 loader 2nd time
    chassis.moveToPoint(55+5,-47,3000,{.minSpeed=60},true); 
        
    toggleIntake(true, 0);
    chassis.waitUntil(24);
    pros::delay(1500); // adj for 3 blocks?? 
    
    // 6 score again
    chassis.moveToPoint(29.5,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  
    
    pros::delay(1100+1000); 
    /*chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(200);*/
    

    // 7 go other side (empty intake)
    chassis.moveToPoint(40,-47,2000,{.earlyExitRange=3},true);
    toggleIntake(false,0);
    chassis.turnToHeading(315,1500,{.minSpeed=30, .earlyExitRange=5},false);
    chassis.moveToPoint(29.5,-36,2000,{.earlyExitRange=3},false);

    chassis.moveToPoint(-29.5,-36,10000,{.earlyExitRange=3},false);
    chassis.turnToHeading(225,1500,{.minSpeed=30, .earlyExitRange=5},false);
    chassis.moveToPoint(-48,-47,2000,{.earlyExitRange=3},false);
    chassis.turnToHeading(270,1200,{.maxSpeed=80,.earlyExitRange=0},false);

    // 8 loader
    toggleLoader(true);
    pros::delay(400);
    
    chassis.moveToPoint(-55+5,-47,1000,{.minSpeed=60},true); 
        
    toggleIntake(true, 0);
    
    pros::delay(900+200); // adj for 3 blocks??  // cut loader time

    // 9 score long goal
    chassis.moveToPoint(-29.5+2,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score
    
    pros::delay(1500); 
    chassis.setPose(-29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(200);

    // 10 loader again
    chassis.moveToPoint(-55+5,-47,3000,{.minSpeed=60},true); 
        
    toggleIntake(true, 0);
    chassis.waitUntil(24);
    pros::delay(1500); // adj for 3 blocks??  // cut loader time

    // 11 long goal again
    chassis.moveToPoint(-29.5+2,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score
    toggleLoader(false);
    pros::delay(1500); 

    // 12 go back to parking
    chassis.moveToPoint(-40,-47,2000,{.earlyExitRange=3},true);
    chassis.turnToHeading(45,1500,{.minSpeed=30, .earlyExitRange=5},false);
    chassis.moveToPoint(-24,-36,2000,{.earlyExitRange=3},false);
    chassis.moveToPoint(24,-36,10000,{.earlyExitRange=3},false);
    chassis.turnToHeading(0,1500,{.minSpeed=30, .earlyExitRange=5},false);

    chassis.moveToPoint(36,0,4000,{.earlyExitRange=3},false);
    chassis.turnToHeading(90,2000,{.minSpeed=30},false);
    chassis.moveToPoint(62,0,5000,{.minSpeed=50},false);
}

void halfField_AWP_skills(){ // right AWP modified
    chassis.setPose(47.5,16.427,0);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5,48,1400,{.earlyExitRange=0});

    toggleIntake(true,0);

    chassis.turnToHeading(90,1000,{.minSpeed=20,.earlyExitRange=10});

    toggleLoader(true);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+10,47,1200,{.minSpeed=30},true); 
   
    pros::delay(2000); // ~optimal intake delay (3 blocks)

    // long goal score
    chassis.moveToPoint(29.5-3,47,1500,{.forwards=false,.minSpeed=40},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2000); // cut extra time
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    //toggleIntake(false,0);

    // loader + score part 2
    toggleIntake(true,0);
    chassis.moveToPoint(55+5,47,1500,{},false); 

    chassis.moveToPoint(29.5-3,47,1500,{.forwards=false,.minSpeed=40},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);
    toggleLoader(false);

    pros::delay(2000); // cut extra time

    // section 2 (center)

    chassis.moveToPoint(36,47,1000,{.minSpeed=30,.earlyExitRange=2},true);

    chassis.turnToHeading(225,1000,{.minSpeed=30,.earlyExitRange=10});

    chassis.moveToPoint(22,22,1500,{.maxSpeed=40,.minSpeed=0,.earlyExitRange=0},true);
    
    chassis.waitUntil(16);
    toggleIntake(true,0);

    chassis.waitUntil(22);

    chassis.turnToHeading(180,1500,{.minSpeed=20});
    //pros::delay(100);

    chassis.moveToPoint(22+2,-22,2000,{.maxSpeed=90,.minSpeed=20,.earlyExitRange=0},true);
    pros::delay(500);
    toggleIntake(true,-1);
    chassis.waitUntil(16);
    toggleIntake(false,0);
    chassis.waitUntil(24);
    toggleIntake(true,0);

    chassis.waitUntilDone();
    toggleLoader(true);
    chassis.moveToPoint(22,-22,1200,{.forwards=false});

    chassis.turnToHeading(135, 1500,{.minSpeed=10});
    

    chassis.moveToPoint(12,-10+1.5,1000,{.forwards=false,.maxSpeed=60,.minSpeed=10,.earlyExitRange=1},true);
    toggleIntake(true,-1);
    pros::delay(200);
    toggleIntake(true,0);

    chassis.waitUntil(10);
    toggleIntake(true,0); // score on center
    setScoreHeight(false);
    pros::delay(3000); 
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(true,0);
    chassis.setPose(12,-10,chassis.getPose().theta); // fake reset
    pros::delay(1500);


    // section 3 (other goal)

    chassis.moveToPoint(47,-47,3000,{.maxSpeed=100},true); 
    toggleIntake(true,1);
    toggleLoader(false);
    chassis.waitUntil(12);
    setScoreHeight(true);

    chassis.turnToHeading(90,1000,{.minSpeed=30});
    toggleLoader(true);
    toggleIntake(true,0);
    pros::delay(500);

    // into loader
    chassis.moveToPoint(55+10,-47+2.5,1200,{.minSpeed=30},true); 
   
    pros::delay(3000); // 

    // long goal score
    chassis.moveToPoint(29.5-2,-47+2.5,1500,{.forwards=false,.maxSpeed=100,.minSpeed=30},true);

    chassis.waitUntil(16);
    toggleIntake(true,1);
    pros::delay(2000);

    // loader + goal again
    chassis.moveToPoint(55+10,-47+2.5,3000,{.maxSpeed=120,.minSpeed=60},true); 
    toggleIntake(true,0);
    pros::delay(3000);

    chassis.moveToPoint(29.5-2,-47+2.5,2000,{.forwards=false,.maxSpeed=100,.minSpeed=30},true);

    chassis.waitUntil(16);
    toggleIntake(true,1);
    toggleLoader(false);
    pros::delay(2000);
    //chassis.setPose(29.5,-47,chassis.getPose().theta); //  reset
    toggleIntake(false,0);
   

    // PARK
    chassis.moveToPoint(40,-47,2000,{});
    /*chassis.turnToHeading(45,1500,{.minSpeed=30},false);
    chassis.moveToPose(62.5,-18,0,4000,{.minSpeed=20},false);
    toggleLoader(true);
    toggleIntake(true,1);

    chassis.moveToPoint(62.5,0,5000,{.minSpeed=50},false);*/

    chassis.turnToHeading(0,2000,{.minSpeed=30},false);
    chassis.moveToPoint(40,0,4000,{},false);
    chassis.turnToHeading(90,2000,{},false);
    chassis.moveToPoint(62.5,0,5000,{.minSpeed=50},false);
}

void slowed_halfField_AWP_skills(){ // added max speed and INCREASE TIMEOUTS
    chassis.setPose(47.5,16.427,0);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5,48,5000,{.maxSpeed=80});

    toggleIntake(true,0);

    chassis.turnToHeading(90,5000,{.maxSpeed=80,.minSpeed=20});

    toggleLoader(true);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+10,47,1200,{.maxSpeed=80,.minSpeed=30},true); 
   
    pros::delay(2000); // ~optimal intake delay (3 blocks)

    // long goal score
    chassis.moveToPoint(29.5-3,47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=40},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(4000); // cut extra time
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset

    // loader + score part 2
    toggleIntake(true,0);
    chassis.moveToPoint(55+10,47,3000,{.maxSpeed=80,.minSpeed=40},false); 

    chassis.moveToPoint(29.5-3,47,1500,{.forwards=false,.maxSpeed=90,.minSpeed=40},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);
    toggleLoader(false);

    pros::delay(2000);

    // section 2 (center)

    chassis.moveToPoint(36,47,2000,{.maxSpeed=80,.minSpeed=30},true);

    chassis.turnToHeading(225,5000,{.maxSpeed=80,.minSpeed=30});

    chassis.moveToPoint(22,22,5000,{.maxSpeed=40,.minSpeed=0},true);
    
    chassis.waitUntil(16);
    toggleIntake(true,0);

    chassis.waitUntil(22);

    chassis.turnToHeading(180,5000,{.maxSpeed=80,.minSpeed=20});
    //pros::delay(100);

    chassis.moveToPoint(22+2,-22,5000,{.maxSpeed=90,.minSpeed=20,.earlyExitRange=0},true);
    pros::delay(500);
    toggleIntake(true,-1);
    chassis.waitUntil(16);
    toggleIntake(false,0);
    chassis.waitUntil(24);
    toggleIntake(true,0);

    chassis.waitUntilDone();
    toggleLoader(true);
    chassis.moveToPoint(22,-22,5000,{.forwards=false,.maxSpeed=80});

    chassis.turnToHeading(135, 5000,{.minSpeed=10});
    
    // center

    chassis.moveToPoint(12,-10+1.5,5000,{.forwards=false,.maxSpeed=60,.earlyExitRange=1},true);
    toggleIntake(true,-1);
    pros::delay(200);
    toggleIntake(false,0);

    chassis.waitUntil(8);
    setScoreHeight(false);
    //toggleIntake(true,0); // score on center
    toggleIntake(true,1,60);
    pros::delay(3000); 
    toggleIntake(true,-1);
    pros::delay(500);
    //toggleIntake(true,0);
    toggleIntake(true,1,60);
    chassis.setPose(12,-10,chassis.getPose().theta); // fake reset
    pros::delay(1500);


    // section 3 (other goal)

    chassis.moveToPoint(47,-47,5000,{.maxSpeed=80},true); 
    toggleIntake(true,1);
    toggleLoader(false);
    chassis.waitUntil(12);
    setScoreHeight(true);

    chassis.turnToHeading(90,5000,{.minSpeed=30});
    toggleLoader(true);
    toggleIntake(true,0);
    pros::delay(500);

    // into loader
    chassis.moveToPoint(55+10,-47,5000,{.maxSpeed=80,.minSpeed=30},true); 
   
    pros::delay(3000); // 

    // long goal score
    chassis.moveToPoint(29.5-2,-47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true);

    chassis.waitUntil(16);
    toggleIntake(true,1);
    pros::delay(2000);
    chassis.setPose(29.5,-47,chassis.getPose().theta);

    // loader + goal again
    chassis.moveToPoint(55+10,-47,5000,{.maxSpeed=80,.minSpeed=30},true); 
    toggleIntake(true,0);
    pros::delay(3000);

    chassis.moveToPoint(29.5-2,-47,5000,{.forwards=false,.maxSpeed=100,.minSpeed=30},true);

    chassis.waitUntil(16);
    toggleIntake(true,1);
    toggleLoader(false);
    pros::delay(2000);
    toggleIntake(false,0);
   

    // PARK
    chassis.moveToPoint(40,-47,5000,{.maxSpeed=80});

    chassis.turnToHeading(0,5000,{.maxSpeed=80,.minSpeed=30},false);
    chassis.moveToPoint(40,0,4000,{.maxSpeed=80},false);
    chassis.turnToHeading(90,2000,{.maxSpeed=80},false);
    chassis.moveToPoint(62.5,0,5000,{.minSpeed=70},false);
}

void ambitious_skills(){ // starting pose = (47.5,16.427,0); keep jamming in Q3
    chassis.setPose(47.5,16.427,0);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5,48,1400,{.maxSpeed=80});

    toggleIntake(true,0);
    toggleWing(true);

    chassis.turnToHeading(90,2000,{.minSpeed=20});

    toggleLoader(true);
    pros::delay(300);

    // into loader
    chassis.moveToPoint(55+10,47,1200,{.maxSpeed=80,.minSpeed=30},false); 
   
    chassis.moveToPoint(50,47,250,{.forwards=false,.maxSpeed=80}); // back

    chassis.moveToPoint(55+10,47,1000,{.maxSpeed=80,.minSpeed=30},false); 

    // out and other side

    chassis.moveToPoint(47,47,2000,{.forwards=false,.maxSpeed=80});
    toggleLoader(false);

    chassis.turnToHeading(180,2000);
    toggleIntake(false,0);

    chassis.moveToPoint(47,34,3000,{.maxSpeed=80});

    chassis.turnToHeading(90,3000);

    chassis.moveToPoint(0,34,5000,{.forwards=false,.maxSpeed=80,.earlyExitRange=12});

    chassis.moveToPoint(-47,34,5000,{.forwards=false,.maxSpeed=80});

    chassis.turnToHeading(180,2000);

    chassis.moveToPoint(-47,47,5000,{.forwards=false,.maxSpeed=80});

    chassis.turnToHeading(270,2000);

    chassis.moveToPoint(-25,47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true); // long goal position

    chassis.waitUntil(15);
    toggleIntake(true,1);
    pros::delay(850);
    toggleIntake(true,-1);
    pros::delay(150);
    toggleIntake(true,1);
    pros::delay(1200);
    chassis.setPose(-29.5,47,chassis.getPose().theta);
    pros::delay(100);

    // loader q2
    chassis.moveToPoint(-47.5,47,1400,{.maxSpeed=80});
    
    toggleLoader(true);
    toggleIntake(true,0);

    chassis.moveToPoint(-55-10,47,1200,{.maxSpeed=70,.minSpeed=30},false); 
   
    chassis.moveToPoint(-50,47,250,{.forwards=false,.maxSpeed=80}); // back

    chassis.moveToPoint(-55-10,47,1000,{.maxSpeed=80,.minSpeed=30},false); 

    // score again
    chassis.moveToPoint(-47,47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=40,.earlyExitRange=1});
    chassis.moveToPoint(-25,47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true); // long goal position

    chassis.waitUntil(15);
    toggleIntake(true,1);
    pros::delay(850);
    toggleIntake(true,-1);
    pros::delay(150);
    toggleIntake(true,1);
    pros::delay(1200);
    chassis.setPose(-29.5,47,chassis.getPose().theta);
    pros::delay(100);

    // traverse

    chassis.moveToPoint(-40,47,2000,{.maxSpeed=80},false);

    chassis.turnToHeading(0,2000);

    chassis.moveToPoint(-40,0,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=12});

    chassis.moveToPoint(-40,-47,5000,{.forwards=false,.maxSpeed=80});

    chassis.moveToPoint(-40,-70,1500,{.forwards=false,.maxSpeed=80},false); // hit wall

    chassis.setPose(chassis.getPose().x,-62.205, chassis.getPose().theta); // reset Y coord on wall
    pros::delay(100);

    chassis.moveToPoint(-40,-47,2000,{.maxSpeed=80});

    chassis.turnToHeading(270,2000);

    // load, double load, move to other side

    toggleLoader(true);
    toggleIntake(true,0);
    pros::delay(400);

    chassis.moveToPoint(-55-10,-47,1200,{.maxSpeed=80,.minSpeed=30},false); 
   
    chassis.moveToPoint(-50,-47,250,{.forwards=false,.maxSpeed=80}); // back

    chassis.moveToPoint(-55-10,-47,1000,{.maxSpeed=80,.minSpeed=30},false); 

    // traverse

    chassis.moveToPoint(-47,-47,2000,{.forwards=false,.maxSpeed=80});
    toggleLoader(false);

    chassis.turnToHeading(180,2000);
    toggleIntake(false,0);

    //chassis.moveToPoint(-47,-34,3000,{.forwards=false,.maxSpeed=80});
    //chassis.turnToHeading(269,2000,{.minSpeed=30}); //// keeps freezing here
    chassis.moveToPoint(-29.5,-34,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=2});

    chassis.moveToPoint(0,-34,5000,{.forwards=false,.maxSpeed=80,.earlyExitRange=10});

    chassis.moveToPoint(-47,-34,5000,{.forwards=false,.maxSpeed=80});

    chassis.turnToHeading(0,2000);

    chassis.moveToPoint(47,-47,5000,{.forwards=false,.maxSpeed=80});

    chassis.turnToHeading(90,2000);

    // score in q4

    chassis.moveToPoint(25,-47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true); // long goal position

    chassis.waitUntil(15);
    toggleIntake(true,1);
    pros::delay(1000);
    toggleIntake(true,-1);
    pros::delay(200);
    toggleIntake(true,1);
    pros::delay(1200);
    chassis.setPose(29.5,-47,chassis.getPose().theta);
    pros::delay(100);

    // loader q4

    chassis.moveToPoint(47.5+2,47,1400,{.maxSpeed=80,.earlyExitRange=2});
    toggleLoader(true);
    toggleIntake(true,0);

    chassis.moveToPoint(55+10,-47,1200,{.maxSpeed=80,.minSpeed=30},false); 
   
    chassis.moveToPoint(50,-47,250,{.forwards=false,.maxSpeed=80}); // back

    chassis.moveToPoint(55+10,-47,1000,{.maxSpeed=80,.minSpeed=30},false); 

    // score again

    chassis.moveToPoint(47,-47,5000,{.forwards=false,.maxSpeed=80,.earlyExitRange=3});
    chassis.moveToPoint(25,-47,5000,{.forwards=false,.maxSpeed=80,.minSpeed=30},true);

    chassis.waitUntil(15);
    toggleIntake(true,1);
    pros::delay(1000);
    toggleIntake(true,-1);
    pros::delay(200);
    toggleIntake(true,1);
    pros::delay(1200);
    chassis.setPose(29.5,-47,chassis.getPose().theta);
    //pros::delay(100);

    // PARK
    chassis.moveToPoint(40,-47,2000,{});

    chassis.turnToHeading(0,2000,{.minSpeed=30,.earlyExitRange=8},false);
    chassis.moveToPoint(40,0,4000,{.minSpeed=50,.earlyExitRange=3},false);
    chassis.turnToHeading(90,2000,{.minSpeed=30,.earlyExitRange=8},false);
    chassis.moveToPoint(62.5,0,8000,{.minSpeed=70},false);
}

void rightAuto_UseWing(){ // change match load time? (keeps getting 1 extra other color ball for some rzn)
    chassis.setPose(62,16.427,270);

    chassis.moveToPoint(22,24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    //toggleIntake(true,0);
    chassis.moveToPoint(22,22,1300,{.maxSpeed=30,.minSpeed=10},true);
    toggleIntake(true,0);
    toggleLoader(true);

    chassis.turnToHeading(45,1000,{.maxSpeed=80,.earlyExitRange=10},false);

    // 2nd segment

    toggleLoader(false);
    chassis.moveToPoint(48,47,2000,{.earlyExitRange=0},true); // accurate?
    //pros::delay(200);
    //toggleIntake(true,-1);

    //chassis.waitUntil(16);
    //toggleIntake(true,0); // reset intake 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},false);
   
    // should score first
    chassis.moveToPoint(29.5-2,47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(13);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  
    pros::delay(1500); // can tune
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    pros::delay(300);

    // then, match load
    toggleLoader(true);
    pros::delay(200);

    chassis.moveToPoint(55+5,47,1500,{.maxSpeed=80,.minSpeed=40},true); 
    
    toggleIntake(true, 0);
    chassis.waitUntil(24);
    pros::delay(650); // adj for 3 blocks??  // cut loader time

    // score again
    chassis.moveToPoint(29.5-2,47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(20);
    toggleIntake(true,1); // score 
    toggleLoader(false);  

    pros::delay(1100); // wait
    chassis.setPose(29.5,47,chassis.getPose().theta); // fake reset
    pros::delay(300);

    // use wing
    //chassis.moveToPoint(48,-47,1000,{.earlyExitRange=16},false);
    // 10.5 displaced
    chassis.moveToPoint(39,58.5-3,1000,{.minSpeed=30},true);
    toggleIntake(false,0);
    chassis.turnToHeading(90,750,{.minSpeed=15},false);
    chassis.moveToPoint(12,58.5-3,2000,{.forwards=false,.maxSpeed=80,.minSpeed=0}); // can change to poses
}

void leftAuto_NoCenter(){ //
    chassis.setPose(62,-16.427,270);

    chassis.moveToPoint(22,-24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    chassis.moveToPoint(22,-22,1300,{.maxSpeed=30,.minSpeed=10},true);
    toggleIntake(true,0);
    toggleLoader(true);

    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},false);

    // 2nd segment

    toggleLoader(false);
    chassis.moveToPoint(48,-47,2000,{.earlyExitRange=0},true); // accurate?
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},false);
   
    // should score first
    chassis.moveToPoint(29.5-2,-47,1500,{.forwards=false,.minSpeed=50,.earlyExitRange=1},true);
    chassis.waitUntil(13);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  
    pros::delay(1500); // can tune
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(300);

    // then, match load
    toggleLoader(true);
    pros::delay(200);

    chassis.moveToPoint(55+5,-47,1500,{.minSpeed=65},true); 
    
    toggleIntake(true, 0);
    chassis.waitUntil(24);
    pros::delay(1000); // adj for 3 blocks??  // cut loader time

    // score again
    chassis.moveToPoint(29.5-2,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(20);
    toggleIntake(true,1); // score 
    toggleLoader(false);  

    pros::delay(1100); // wait
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(300);

    // use wing
    //chassis.moveToPoint(48,-47,1000,{.earlyExitRange=16},false);
    // 10.5 displaced
    chassis.moveToPoint(39,-38,1000,{.minSpeed=30},true);
    toggleIntake(false,0);
    chassis.turnToHeading(90,750,{.minSpeed=15},false);
    chassis.moveToPoint(15,-38,2000,{.forwards=false,.maxSpeed=90,.minSpeed=0}); // change max speed
}

void test(){
    chassis.setPose(0,0,0);
    //chassis.moveToPoint(0,24,5000, {.maxSpeed=80});
    //chassis.moveToPose(0,24,0,5000);
    //chassis.turnToHeading(270,2000);
    setScoreHeight(false);

    toggleIntake(true,1,60);
}

void autonomous() {
    //pidtuning();
    colorIsBlue(true);

    pros::Task antiJam(antiJamTask);

    //////leftAuto_AWP(); // 
    //rightAuto_AWP(); // MAIN ONE, 1 and 5

    leftAuto_CenterGoal(); // 2
    
    //rightAuto_UseWing(); // 3

    //leftAuto_NoCenter(); // 4 

    //halfField_AWP_skills(); // the good one, also 5
    //slowed_halfField_AWP_skills(); // 5

    //ambitious_skills(); // 6 UPLOAD

    //cheese_skills(); // 
    //halfField_skills(); // 6

    // skills(); // 8　for nelson

    //test(); // 7

    antiJam.remove();

    //leftAuto_UseWing();
    //rightAuto_UseWing();
    
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

        pros::Task::delay(15);
        time_elapsed += 15;
    }
}