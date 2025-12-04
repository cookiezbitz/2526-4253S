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

    while(true){
        if(intakeState != 0){ // if intake isnt supposed to be jamming
            if(fabs(upperIntake.get_actual_velocity()) < 10 || fabs(lowerIntake.get_actual_velocity()) < 10){ // rpm
                stallCounter += 15;
            }else{
                stallCounter = 0;
            }
            
            if(stallCounter > 500){ // in ms
                // jam detected
                if(intakeState == 1){ // intake forward
                    lowerIntake.move(-70);
                    upperIntake.move(-70);
                    pros::Task::delay(500);
                    lowerIntake.move(127);
                    upperIntake.move(127);
                }else if(intakeState == -1){ // intake reverse
                    lowerIntake.move(70);
                    upperIntake.move(70);
                    pros::Task::delay(500);
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

//bad
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

void originalLeftAuton(){
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

    toggleLoader(false);

    chassis.turnToHeading(135,1000,{.maxSpeed=60,.earlyExitRange=10},true);

    // 24,24 or something
    // back into 10,-10 OR ADJUST to score

    chassis.moveToPoint(12+3+2,-12-1,3000,{.forwards=false, .maxSpeed=70, .minSpeed=40, .earlyExitRange = 1},false);
    toggleIntake(true,1);


    pros::delay(600);
    toggleIntake(true, -1);
    pros::delay(200);
    toggleIntake(false, 0);
    pros::delay(500);



    //toggleLoader(false);
    // go to x, -48 to set up for nelson
    chassis.moveToPoint(48,-48,5000,{.forwards = true, .maxSpeed=80/*,.minSpeed=50,.earlyExitRange=2*/},false);
    toggleIntake(false,0);
    chassis.turnToPoint(72,-48,1000,{.maxSpeed=60},true);

    // "reset " pos
    //chassis.setPose(48,-48,chassis.getPose().theta);
    // match loader
    toggleLoader(true);
    toggleIntake(true,0);
    pros::delay(200);
    chassis.moveToPoint(65,-48,3000,{.maxSpeed=60,.minSpeed=40},false);
    pros::delay(1000);

    chassis.moveToPoint(30, -48, 2000,{.forwards = false,.minSpeed=60,.earlyExitRange=2},false);
    toggleIntake(true, 1);
    pros::delay(2000);
    toggleIntake(false,0);
    //chassis.moveToPoint(60,-48,3000,{.maxSpeed=60,.minSpeed=40},false);
}

void AWPright(){
    chassis.setPose(0,0,0);

    toggleIntake(true,0);
    chassis.moveToPoint(6,38.5,2000, {.minSpeed = 60, .earlyExitRange=10});
    chassis.moveToPoint(6,38.5,2000, {.maxSpeed = 40, .earlyExitRange=1});

    chassis.turnToHeading(125,1000,{.maxSpeed=80, .earlyExitRange=20});
    //toggleIntake(false,0);

    chassis.moveToPose(34,12,180,3000,{.maxSpeed=80,.minSpeed=40,.earlyExitRange=2});

    chassis.moveToPoint(34,8,2000,{.maxSpeed=80}); // match loader
    
    chassis.moveToPoint(34,29,2000,{.forwards=false,.minSpeed=50,.earlyExitRange=5});
    chassis.moveToPoint(34,29,2000,{.forwards=false,.maxSpeed=30},true);
    toggleIntake(true,1);
}

void AWPleft(){ // based on altLeft // not within time
    chassis.setPose(62,-16.247,270);

    // intake directly into blocks
    chassis.moveToPoint(36,-24,5000,{.minSpeed=30,.earlyExitRange = 3},false);
    toggleIntake(true, 0);

    chassis.moveToPoint(22,-24,5000,{.maxSpeed=60,.minSpeed=40,.earlyExitRange = 1},false);
    toggleLoader(true);
    chassis.moveToPoint(15,-24,3000,{.maxSpeed=60,.minSpeed=40,.earlyExitRange = 0},false);

    chassis.moveToPoint(22,-24,5000,{.forwards = false, .maxSpeed=80,.minSpeed=50},false); // change y from 22 to 24

    chassis.turnToHeading(135,1000,{.maxSpeed=60,.earlyExitRange=10},true);

    // 24,24 or something
    // back into 10,-10 OR ADJUST to score

    chassis.moveToPoint(12+3,-12,3000,{.forwards=false, .maxSpeed=60, .minSpeed=40, .earlyExitRange = 4},false);
    toggleIntake(true,1);


    pros::delay(600);
    toggleIntake(true, -1);
    pros::delay(200);
    toggleIntake(false, 0);
    pros::delay(500);


    //toggleLoader(false);
    chassis.moveToPoint(48,-48,5000,{.forwards = true, .maxSpeed=80,.minSpeed=40,.earlyExitRange=3},false);
    toggleIntake(false,0);
    chassis.turnToPoint(72,-48,1000,{.maxSpeed=60,.minSpeed=30,.earlyExitRange=10},false);
    chassis.moveToPoint(30, -48, 2000,{.forwards = false,.minSpeed=60},false); // long goal
    toggleIntake(true, 1);
    pros::delay(1500);
    toggleIntake(false,0);

    chassis.moveToPoint(40,-48,2000,{.maxSpeed=60,.minSpeed=40,.earlyExitRange=3},false);
    chassis.turnToHeading(0,1000,{.maxSpeed=60,.earlyExitRange=10},false);
    // potentially pick up middle 3 but depending on time

    // cross to other side
    chassis.moveToPoint(40,52,3000,{.minSpeed=40},false);
    chassis.turnToHeading(90,700,{.maxSpeed=60,.earlyExitRange=10},false);

    chassis.moveToPoint(50,52,3000,{.minSpeed=40,.earlyExitRange=3},false);
    toggleIntake(true,0);
    pros::delay(1500);

    chassis.moveToPoint(30,52,3000,{.forwards=false,.minSpeed=50,.earlyExitRange=2},false);
    toggleIntake(true,1);
}


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

void skills(){ // not done
    chassis.setPose(62,-16.427,270);

    chassis.moveToPoint(22,-24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    toggleIntake(true,0);
    chassis.moveToPoint(22,-22,1500,{.maxSpeed=30},true);
    toggleLoader(true);

    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    toggleLoader(false);

    chassis.moveToPose(10,-10,135,1000,{.forwards=false,.maxSpeed=60,.minSpeed=20,.earlyExitRange=1},true);
    
    chassis.waitUntilDone();
    toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(800); // increased time 750 --> 1500 ms
   

    //chassis.moveToPoint(20,-20,1000,{.earlyExitRange=2},true);
    chassis.moveToPoint(22,22,2000,{},true);
    setScoreHeight(true);
    toggleIntake(true,0);
    chassis.waitUntilDone();
    toggleLoader(true);

    chassis.turnToHeading(225,1000,{.maxSpeed=80,.earlyExitRange=5},false);
    chassis.moveToPoint(10,10,1000,{},true);
    toggleLoader(false);
    chassis.waitUntil(10);
    toggleIntake(true,-1);

    pros::delay(1000);

    // 2nd segment - match loader

    chassis.moveToPoint(48,47,2000,{.forwards=false,.earlyExitRange=0},true); 
    chassis.waitUntil(16);
    toggleIntake(false,0); // reset intake 
    
    chassis.turnToHeading(90,1500,{.earlyExitRange=5},false);

    toggleLoader(true);
    pros::delay(200);

    // into loader - QI
    chassis.moveToPoint(55+5,47,1000,{.minSpeed=50},true); 
    
    toggleIntake(true, 0);

    pros::delay(2000); // 3 blocks = 1200 ms

    // go to other side
    chassis.moveToPoint(29.5,60,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    toggleLoader(false);

    chassis.moveToPoint(-48,47,2000,{.forwards=false,.minSpeed=30},true);

    // score in quadrant II
    chassis.turnToHeading(270,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    chassis.moveToPoint(-29.5,47,1600,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2000);
    //chassis.setPose(-29.5,48,chassis.getPose().theta); // fake reset
    toggleIntake(false,0);

    // into loader, QII
    toggleLoader(true);
    pros::delay(200);

    chassis.moveToPoint(-55+5,47,1000,{.minSpeed=50},true); 
    
    toggleIntake(true, 0);

    pros::delay(2000); // 3 blocks = 1200 ms
    
    // score, QII
    chassis.moveToPoint(-29.5,47,1600,{.forwards=false,.minSpeed=30},true);
    toggleLoader(false);
    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2000);

    // section 3
    chassis.moveToPoint(-36,47,2000,{.minSpeed=30},true);
    toggleIntake(true,-1); 
    chassis.turnToHeading(180,1000,{.maxSpeed=80,.earlyExitRange=5},true);
    toggleIntake(true,0);
    chassis.moveToPoint(-36,-47,5000,{.minSpeed=30},false);

    // into loader, QIII
    chassis.turnToHeading(270,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},false);
    toggleLoader(true);
    pros::delay(200);

    chassis.moveToPoint(-55+5,-47,1000,{.minSpeed=50},true); 

    pros::delay(2000); // 3 blocks = 1200 ms

    // mirror, into QIV 


    ///
    chassis.moveToPoint(-29.5,-60,2000,{.forwards=false,.minSpeed=30,.earlyExitRange=3},true);
    toggleLoader(false);

    chassis.moveToPoint(-48,47,2000,{.forwards=false,.minSpeed=30},true);

    // go to QIV 
    chassis.turnToHeading(270,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    chassis.moveToPoint(-29.5,47,1600,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(20);
    toggleIntake(true,1);

    pros::delay(2000);
    //chassis.setPose(-29.5,48,chassis.getPose().theta); // fake reset
    toggleIntake(false,0);

}

void leftAuto_CenterGoal(){ // !
    chassis.setPose(62,-16.427,270);

    chassis.moveToPoint(22,-24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    //toggleIntake(true,0);
    chassis.moveToPoint(22,-22,1500,{.maxSpeed=30},true);
    toggleIntake(true,0);
    toggleLoader(true);



    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    toggleLoader(false);
    //chassis.setPose(22,-22,chassis.getPose().theta); // fake reset

    chassis.moveToPose(10.5,-10.5,135,1000,{.forwards=false,.maxSpeed=60,.minSpeed=20,.earlyExitRange=0},true);
    toggleIntake(false,0);
    chassis.waitUntil(16); //
    toggleIntake(true,1);
    setScoreHeight(false);
    pros::delay(1000); // increased time 800 --> 1000 ms

    /*
    
    chassis.moveToPoint(11.5,-10,1000,{.forwards=false,.maxSpeed=60,.minSpeed=10},true);
    
    chassis.waitUntil(12);
    toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(1000); // increased time 750 --> 1500 ms

    */
   


    // 2nd segment

    chassis.moveToPoint(48,-47,2000,{.earlyExitRange=0},true); // accurate?
    pros::delay(500);
    toggleIntake(true,-1);
    //pros::delay(500);
    //setScoreHeight(true);
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
    chassis.moveToPoint(55+5,-47,1000,{.minSpeed=50},true); 
    
    toggleIntake(true, 0);

    //pros::delay(1200); // ~optimal intake delay (3 blocks)
    pros::delay(900); // adj for 3 blocks??

    chassis.moveToPoint(29.5,-47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam 
    toggleLoader(false);  

    
    pros::delay(1100);
    chassis.setPose(29.5,-47,chassis.getPose().theta); // fake reset
    pros::delay(200);

    // use wing
    //chassis.moveToPoint(48,-47,1000,{.earlyExitRange=16},false);
    chassis.moveToPoint(39,-37,1000,{.minSpeed=30},false);
    chassis.turnToHeading(90,750,{.minSpeed=15},false);
    chassis.moveToPoint(12,-37,2000,{.forwards=false,.maxSpeed=60,.minSpeed=0}); // can change to poses
}

void leftAuto_AWP(){ // AWP modified 12/1
    //chassis.setPose(48,-16.427,180);
    chassis.setPose(47.5,-16.427,180);

    // section 1 (initial goal)

    chassis.moveToPoint(47.5 /*48*/,-47,1000,{.maxSpeed=70,.minSpeed=10,.earlyExitRange=1});

    toggleIntake(true,0);

    chassis.turnToHeading(90,1000,{.minSpeed=20,.earlyExitRange=10});


    toggleLoader(true);
    pros::delay(100);

    // into loader
    chassis.moveToPoint(55+5,-47,1000,{.minSpeed=40},true); 
   
    pros::delay(1000); // ~optimal intake delay (3 blocks)

    // long goal score
    chassis.moveToPoint(29.5,-47,1600,{.forwards=false,.minSpeed=30},true);

    chassis.waitUntil(22);
    toggleIntake(true,1);
    toggleLoader(false);

    pros::delay(1500);
    chassis.setPose(29.5,-48,chassis.getPose().theta); // fake reset
    toggleIntake(false,0);


    // section 2 (center)

    chassis.moveToPoint(36,-47,1000,{.minSpeed=30,.earlyExitRange=2},true);
    toggleIntake(true,-1); // outtake in case of jam

    chassis.turnToHeading(90,1000,{.minSpeed=30,.earlyExitRange=10});
    //chassis.swingToHeading(0,lemlib::DriveSide::LEFT,1000,{.minSpeed=30,.earlyExitRange=10});

    chassis.moveToPoint(22,-22,1500,{.maxSpeed=60,.minSpeed=30,.earlyExitRange=1},true);
    toggleIntake(true,0);
    chassis.waitUntil(22);
    toggleLoader(true);
    
    chassis.turnToHeading(135,1000,{.earlyExitRange=10});
    toggleLoader(false);

    //chassis.moveToPose(10,-10,135,1000,{.forwards=false,.maxSpeed=60,.minSpeed=20,.earlyExitRange=1},true);
    chassis.moveToPoint(11.5,-10,1000,{.forwards=false,.maxSpeed=60,.minSpeed=10},true);
    
    chassis.waitUntil(12);
    toggleIntake(true,1); // score on center
    setScoreHeight(false);
    pros::delay(1000); // increased time 750 --> 1500 ms


    // section 3 (other goal)

    chassis.moveToPoint(22,-22,1000,{.minSpeed=30,.earlyExitRange=2},true); 
    toggleIntake(false,0);

    chassis.turnToHeading(0,1000,{.minSpeed=30,.earlyExitRange=10},true);
    setScoreHeight(true);

    //
    chassis.moveToPoint(22,22,1000,{.minSpeed=60,.earlyExitRange=12},true); // other 3 balls
    toggleIntake(true,0);

    chassis.moveToPoint(22,22,1000,{.maxSpeed=60,.minSpeed=50,.earlyExitRange=3},true);
    //chassis.waitUntil(6);
    //toggleLoader(true);

    //chassis.moveToPoint(48,48,1500,{.minSpeed=20,.earlyExitRange=2});
    //chassis.turnToHeading(90,1000,{.minSpeed=30,.earlyExitRange=10});

    chassis.moveToPose(48,48,90,1200,{.lead=0.2,.minSpeed=40,.earlyExitRange=3});

    // long goal score
    chassis.moveToPoint(29.5,47,1600,{.forwards=false,.minSpeed=40},true);

    chassis.waitUntil(18);
    toggleIntake(true,1);
    toggleLoader(false);
}

void leftAuto_UseWing(){ // untested; should be similar to right side
    chassis.setPose(62,-16.427,270);

    chassis.moveToPoint(22,-22,1500,{.minSpeed=40,.earlyExitRange=10},true);
    toggleIntake(true,0);
    
    chassis.waitUntil(36);
    chassis.moveToPoint(22,-22,1500,{.maxSpeed=60},false);
    toggleLoader(true);
    pros::delay(200);

    chassis.turnToHeading(135,1000,{.maxSpeed=80,.earlyExitRange=10},false);

    chassis.setPose(22,-22,chassis.getPose().theta);

    // 2nd segment

    chassis.moveToPoint(48,-47,2000,{.earlyExitRange=0},true); // accurate?

    toggleIntake(false,0);
    chassis.waitUntilDone();
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.earlyExitRange=5},true);
    chassis.setPose(48,-47,chassis.getPose().theta); // correct drift

    toggleLoader(true);
    pros::delay(200);

    // into loader
    chassis.moveToPoint(55+5,-47,1000,{.minSpeed=70},true); 
    
    toggleIntake(true, 0);
    pros::delay(1100); // ~optimal intake delay (3 blocks)

    chassis.moveToPoint(29.5,-47,1500,{.forwards=false,.earlyExitRange=1},false);
    toggleIntake(true,1); // score; might jam
    pros::delay(1000);

    chassis.moveToPoint(36,chassis.getPose().y, 1000, {.earlyExitRange=1.5});
    chassis.turnToHeading(180,750,{.earlyExitRange=10});

    // should deploy wing
    chassis.moveToPose(-10,-12,270,2000,{}); // wing

}

void rightAuto_UseWing(){ // untested
    chassis.setPose(62,16.427,270);

    chassis.moveToPoint(22,24,1500,{.minSpeed=20,.earlyExitRange=15},true);
    toggleIntake(true,0);
    chassis.moveToPoint(22,22,1500,{.maxSpeed=30},true);
    toggleLoader(true); // 3 blocks

    chassis.turnToHeading(45,1000,{.maxSpeed=80,.earlyExitRange=10},false);
    toggleLoader(false);
    

    // 2nd segment

    chassis.moveToPoint(48,47,2000,{.earlyExitRange=0},true); // accurate?
    toggleIntake(true,-1);
    setScoreHeight(true);
    chassis.waitUntil(16);
    toggleIntake(false,0); // reset intake 
    
    chassis.turnToHeading(90,1000,{.maxSpeed=80,.earlyExitRange=5},false);

    toggleLoader(true);
    pros::delay(400);

    // into loader
    chassis.moveToPoint(55+5,47,1000,{.minSpeed=50},true); 
    
    toggleIntake(true, 0);
    
    pros::delay(1200); // ~optimal intake delay (3 blocks)

    chassis.moveToPoint(29.5,47,1500,{.forwards=false,.earlyExitRange=1},true);
    chassis.waitUntil(24);
    toggleIntake(true,1); // score; might jam    
    pros::delay(1300);

    // use wing
    //chassis.moveToPoint(39,53.5,1000,{.minSpeed=30},true);
    /*chassis.moveToPoint(39,47,1000,{.minSpeed=30,.earlyExitRange=5},false);
    chassis.moveToPose(39,59,90,1500,{},true);*/
    chassis.moveToPoint(39,58,1000,{.minSpeed=30},false);
    chassis.turnToHeading(90,750,{},false);
    chassis.moveToPoint(20,58,1000,{.forwards=false,.minSpeed=50}); // can change to poses
}

void test(){
    //toggleIntake(true,1);
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0, 24, 3000);
}

void autonomous() {
    //pidtuning();
    colorIsBlue(false);

    pros::Task antiJam(antiJamTask);
    
    //leftAuto_AWP(); // 1
    //leftAuto_CenterGoal(); // 2
    
    //rightAuto_UseWing(); //

    //skills();

    test(); // 7

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