#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cmath>
#include <iostream>


//ASSET(testblueright_txt);




int delayTask = 190;
int delayIntake = 200;


double startTime;
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
/*double getDistanceTo(double readingPosition, double goalPosition){ // coordinates in inches
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
}*/




double rightDistanceSensorOffset = 5.9; // in, from the center
// returns distance relative to reading position
// uses the RIGHT side distance sensor.
// use absolute value of coordinates
double readRight_coordinate(double readingPosition){
   double distanceRead = right_distanceSensor.get_distance()/25.4; // mm converted to inches


   return readingPosition - distanceRead - rightDistanceSensorOffset;
}


double leftDistanceSensorOffset = 5.9;
double readLeft_coordinate(double readingPosition){
   double distanceRead = left_distanceSensor.get_distance()/25.4; // mm converted to inches


   return readingPosition - distanceRead - leftDistanceSensorOffset;
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


void forward(double distance, int direction, int timeout, float minSpeed = 0, float maxSpeed = 127, float earlyExit = 0, bool async = true){
   double x = chassis.getPose().x + distance * sin(chassis.getPose().theta * M_PI / 180.0) * direction;
   double y = chassis.getPose().y + distance * cos(chassis.getPose().theta * M_PI / 180.0) * direction;


   chassis.moveToPoint(x,y,timeout,{.forwards = (direction == 1), .maxSpeed=maxSpeed,.minSpeed=minSpeed,.earlyExitRange=earlyExit},async);
}


void freezeAt(double x, double y, double heading){
   while(true){
       chassis.moveToPose(x,y,heading, 1500,{.minSpeed=50});
       pros::delay(15);
       if(pros::millis() - startTime > 14500){
           chassis.cancelAllMotions();
           break;
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


void right_AWP(){
   chassis.setPose(47.5,16.427,0);


   // section 1 (initial goal)


   chassis.moveToPoint(47.5,47.5,1000,{.earlyExitRange=0});
   toggleIntake(true,0);


   chassis.turnToHeading(90,1000,{.earlyExitRange=0});
   toggleLoader(true);


   // into loader
   chassis.moveToPoint(59,47,900,{.maxSpeed=50,.minSpeed=30},true);
   wait(900);


   // long goal score
   chassis.moveToPoint(29.5-2,48,1700,{.forwards=false,.maxSpeed=100,.minSpeed=30},true);
   chassis.waitUntil(24);
   toggleIntake(true,1);
   toggleLoader(false);


   wait(400); // cut extra time
   chassis.setPose(28.5,48,chassis.getPose().theta); // fake reset
   wait(300);


   // to center


   chassis.moveToPoint(38,47,1000,{.minSpeed=40,.earlyExitRange=2},true);


   chassis.turnToHeading(225,1000,{.minSpeed=40,.earlyExitRange=10});


   chassis.moveToPoint(22,22,1500,{.maxSpeed=100,.minSpeed=30,.earlyExitRange=1},true);
   toggleIntake(true,0);


   chassis.waitUntil(27);
   toggleLoader(true);
    chassis.turnToHeading(180-5,1000,{.minSpeed=30,.earlyExitRange=5},false);


   chassis.moveToPoint(22,-22,2000,{.minSpeed=30,.earlyExitRange=12},true);
   toggleLoader(false);


   //chassis.moveToPoint(22,-24,2000,{.maxSpeed=40,.minSpeed=10,.earlyExitRange=1},true);
   chassis.moveToPoint(22,-24,2000,{.minSpeed=40,.earlyExitRange=1},true);


   chassis.waitUntil(8);
   toggleLoader(true);


   // long goal 2 (pasted from left victor)


   chassis.moveToPoint(38,-48,2000,{.minSpeed=30},true);
  
   chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=5},false);


   chassis.moveToPoint(29.5-2,-48,1300,{.forwards=false,.maxSpeed=80,.minSpeed=40,.earlyExitRange=1},true);
   chassis.waitUntil(4);
   toggleIntake(true,1); // score


   wait(500);
   chassis.setPose(29.5,-48,chassis.getPose().theta); //
   wait(300);


   // into loader
   chassis.moveToPoint(47,-47.5,1000,{.minSpeed=40,.earlyExitRange=0},true);
   chassis.moveToPoint(60,-47,850,{.maxSpeed=50,.minSpeed=40},true);
   toggleIntake(true,0);


   // go center
   chassis.moveToPoint(10,-10,3000,{.forwards=false,.maxSpeed=90,.minSpeed=10},true);
   chassis.waitUntil(43.5);
   chassis.cancelMotion();


   chassis.moveToPoint(10,-10,1000,{.forwards=false,.maxSpeed=50,.minSpeed=10},true);
   chassis.waitUntil(9); // 10 --> 9


   toggleIntake(true,2);
   wait(200);


   upperIntake.move(-30);
   //wait(900);


   /*
   chassis.moveToPoint(30,-37,2000,{.minSpeed=30},true);
   chassis.turnToHeading(90,1500,{.minSpeed=30,.earlyExitRange=5},true);
   toggleIntake(false,0);
  
   chassis.moveToPoint(12,-37,2000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); // max speed editable
   chassis.turnToHeading(110,1000,{.minSpeed=30,.earlyExitRange=2},true);
   toggleIntake(false,0);*/


   /*chassis.moveToPoint(47,-48,2000,{.maxSpeed=127,.minSpeed=20,.earlyExitRange=0},true);  // early exit?


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


   chassis.moveToPoint(11,-10,3000,{.forwards=false,.maxSpeed=120,.minSpeed=10},true);
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
   chassis.moveToPoint(10,-10,500,{.forwards=false,.minSpeed=40});*/
}


// standard
void right_LowerLong(){
   chassis.setPose((48-2.25),(15.75),270);


   chassis.moveToPoint(22,22,1500,{.minSpeed=20,.earlyExitRange=15},true);
   chassis.moveToPoint(22,22,1300,{.maxSpeed=25,.minSpeed=10},true);
   toggleIntake(true,0);
   toggleLoader(true);




   chassis.turnToHeading(225,1000,{.earlyExitRange=1},false);


   // lower goal
   chassis.moveToPoint(10,10,1500,{.maxSpeed=60},true);
   toggleLoader(false);
   chassis.waitUntil(8);
   toggleIntake(true,-1);


   wait(750);


   /*chassis.moveToPoint(14,14,500,{.forwards=false,.minSpeed=20},true);
   chassis.moveToPoint(10,10,500,{.minSpeed=30});*/
   wait(250);


   // 2nd segment


   chassis.moveToPoint(48,46,2000,{.forwards=false,.earlyExitRange=1},true); // accurate?
   chassis.waitUntil(10);
   toggleIntake(true,0);
  
   chassis.turnToHeading(90,1000,{.earlyExitRange=0},true);
   toggleLoader(true);
 
   // then, match load


   chassis.moveToPoint(59,46.5,800,{.maxSpeed=50,.minSpeed=30},true);
   wait(800);


   // score again
   chassis.moveToPoint(29.5-2,47.5,1500,{.forwards=false,.earlyExitRange=1},true);
   chassis.waitUntil(20);
   toggleIntake(true,1); // score
   toggleLoader(false);


   wait(700); // wait
   chassis.setPose(29.5,48,chassis.getPose().theta); // fake reset
   wait(300);


   // use wing
   // 8.5 displaced


   /*chassis.moveToPoint(39,38.5,1000,{.minSpeed=30},true);
   toggleIntake(false,0);
   chassis.turnToHeading(270,3000,{.minSpeed=15},false);
   pros::delay(50);
   chassis.moveToPoint(10,38.5,2000,{.maxSpeed=80}); // can change to poses
   chassis.turnToHeading(235,2000,{.minSpeed=30},false);*/


   forward(10,1,1500,30);
   toggleIntake(false,0);
   chassis.waitUntilDone();
   chassis.turnToHeading(210,1000);
   chassis.waitUntilDone();
   pose_chassis.moveToPose(16,38+1,270+5,3000,{.lead=0.8,.minSpeed=60},true);
   pose_chassis.waitUntilDone();
   wait(500);
   chassis.turnToHeading(235+10,2000,{.minSpeed=50,.earlyExitRange=5},false);
}


// victor, AWP start
void right_LongLower(){
   chassis.setPose(47.5,16.427,0);


   // section 1 (initial goal)


   chassis.moveToPoint(47.5,47.5+0.3,1000,{.earlyExitRange=0.3});
   toggleIntake(true,0);


   chassis.turnToHeading(90,700,{.earlyExitRange=1});
   toggleLoader(true);


   // into loader
   chassis.moveToPoint(59,47,1000,{.maxSpeed=50,.minSpeed=30},true);
   wait(1000);


   // long goal score
   chassis.moveToPoint(29.5-2,48,1700,{.forwards=false,.maxSpeed=60,.minSpeed=30},true);
   chassis.waitUntil(24);
   toggleIntake(true,1);
   toggleLoader(false);


   wait(700); // cut extra time
   chassis.setPose(29.5-1,48,chassis.getPose().theta); // fake reset
   wait(300);


   // section 2 (center)


   chassis.moveToPoint(47,47,1000,{.minSpeed=40,.earlyExitRange=2},true);


   chassis.turnToHeading(235,1000,{.minSpeed=30,.earlyExitRange=10});
   chassis.moveToPoint(22,22,1500,{.minSpeed=20,.earlyExitRange=15},true);
   chassis.moveToPoint(22,22,1300,{.maxSpeed=30,.minSpeed=10,.earlyExitRange=4},true);
   toggleIntake(true,0);
   toggleLoader(true);


   // lower goal
   chassis.moveToPoint(10,10-2,1500,{.maxSpeed=60},true);
   toggleLoader(false);
   chassis.waitUntil(8);
   toggleIntake(true,-1);
   wait(1000);




   // section 3 (descore)


   chassis.moveToPoint(30,38.5-4,1000,{.forwards=false,.maxSpeed=80,.minSpeed=10},true);
   //chassis.moveToPoint(39,38.5-4,1000,{.forwards=false,.maxSpeed=80,.minSpeed=10},true);
   chassis.waitUntil(10);
   toggleIntake(false,0);
   chassis.turnToHeading(270,3000,{.minSpeed=15,.earlyExitRange=2},false);


   chassis.moveToPoint(12,38.5-3,1200,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); //
   chassis.turnToHeading(235,1000,{.minSpeed=50,.earlyExitRange=5},false);
}


void right_Long7(){
   //chassis.setPose(62,16.427,270);
   chassis.setPose((48-2.25),(15.75),270);


   chassis.moveToPoint(22,22,1500,{.minSpeed=20,.earlyExitRange=15},true);
   chassis.moveToPoint(22,22,1300,{.maxSpeed=30,.minSpeed=30},true);
   toggleIntake(true,0);
   toggleLoader(true);


   chassis.turnToHeading(45,1000,{.maxSpeed=80,.earlyExitRange=10},false);


   // load


   chassis.moveToPoint(48,47.5,2000,{.minSpeed=10,.earlyExitRange=0},true); //
  
   chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=5},true);


   chassis.moveToPoint(60,47.5,900,{.maxSpeed=50,.minSpeed=40},true);
  
   toggleIntake(true, 0);


   // score
   chassis.moveToPoint(29.5-2,47.5,1500,{.forwards=false,.earlyExitRange=1},true);
   chassis.waitUntil(20);
   toggleIntake(true,1); // score
   toggleLoader(false); 
  


   wait(1000); // wait
   chassis.setPose(29.5,48,chassis.getPose().theta); // fake reset
   wait(500);


   //  wing
  
   forward(10,1,1500,30);
   toggleIntake(false,0);
   chassis.waitUntilDone();
   chassis.turnToHeading(210,1000);
   chassis.waitUntilDone();
   pose_chassis.moveToPose(16,38+1,270+5,3000,{.lead=0.8,.minSpeed=60},true);
   /*chassis.moveToPoint(39,38.5+0.5,800,{.minSpeed=30},true);
   toggleIntake(false,0);
   chassis.turnToHeading(270,2500,{.minSpeed=15},false);
   chassis.moveToPoint(10,38.5+0.5,2000,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); */ // can change to poses
   pose_chassis.waitUntilDone();
   wait(500);
   chassis.turnToHeading(235+15,2000,{.minSpeed=50,.earlyExitRange=5},false);
}


void right_Long9(){
   chassis.setPose((48-2.25),(15.75),270);


   chassis.moveToPoint(22,22,1500,{.minSpeed=20,.earlyExitRange=15},true);
   chassis.moveToPoint(22,22,1300,{.maxSpeed=30,.minSpeed=30},true);
   toggleIntake(true,0);
   chassis.waitUntil(12);
   toggleLoader(true);


   chassis.moveToPoint(10,32,1000,{.maxSpeed=80});
   chassis.waitUntil(10);
   toggleLoader(false);
   chassis.turnToHeading(165,500,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=2},true);
  
   chassis.moveToPoint(8,40,800,{.maxSpeed=80,.minSpeed=25},false);
  
   toggleLoader(true);
   wait(200);


   // (7.8, -39.5, 195)


   chassis.moveToPoint(48,24,2000,{.forwards=false,.minSpeed=40,.earlyExitRange=36},true);
   chassis.moveToPoint(48,48,2000,{.forwards=false,.minSpeed=20});


   // 2nd segment
  
   chassis.turnToHeading(90,1500,{},true);


   // then, match load
   toggleLoader(true);


   chassis.waitUntilDone();
   chassis.setPose(48,readLeft_coordinate(70.75),chassis.getPose().theta);


   chassis.moveToPoint(60,46.5,900,{.maxSpeed=50,.minSpeed=40},true);


   toggleIntake(true, 0);


   // score again
   chassis.moveToPoint(29.5-2,-47.5,1500,{.forwards=false,.earlyExitRange=1},true);
   chassis.waitUntil(20);
   toggleIntake(true,1); // score
   toggleLoader(false); 


   wait(1100); // wait
   chassis.setPose(29.5,-48,chassis.getPose().theta);
   wait(900);


   // use wing, OR hood
   // toggleHood(true);
   // 10.5 displaced
   forward(10,1,1500,30);
   toggleIntake(false,0);
   chassis.waitUntilDone();
   chassis.turnToHeading(210,1000);
   chassis.waitUntilDone();
   pose_chassis.moveToPose(16,38+1,270+5,3000,{.lead=0.8,.minSpeed=60},true);
   pose_chassis.waitUntilDone();
   wait(500);
   chassis.turnToHeading(235+15,2000,{.minSpeed=50,.earlyExitRange=5},false);
}


// standard
void left_CenterLong(){
   //chassis.setPose(38.5+8,-(4+13.625/2.0),270);
   chassis.setPose((48-2.25),-(15.75),270);


   chassis.moveToPoint(22,-22,1500,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=15},true);


   chassis.moveToPoint(20,-24,1000,{.maxSpeed=30,.minSpeed=10,.earlyExitRange=0},true);
   toggleIntake(true,0);
   wait(200);
   toggleLoader(true);


   pose_chassis.moveToPose(10,-10,135,2000,{.forwards=false,.minSpeed=20},true);


   pose_chassis.waitUntil(16);
   //chassis.waitUntil(9); // 10 --> 9
   toggleIntake(true,2);


   //chassis.waitUntil(10);
   wait(1500);


   /*chassis.moveToPoint(12,-12,500,{.minSpeed=30});
   chassis.waitUntil(3);
   setScoreHeight(true);
   chassis.moveToPoint(8,-8,500,{.forwards=false,.minSpeed=40});*/


   pose_chassis.waitUntilDone();
  
   // 2nd segment


   chassis.moveToPoint(47,-47.5-0.5,2000,{.maxSpeed=100,.minSpeed=10,.earlyExitRange=0},true); //
   toggleIntake(true,0);


   chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=15,.earlyExitRange=2},true);


   // into loader
   chassis.moveToPoint(60,-48,900+100,{.maxSpeed=50,.minSpeed=40},true);


   chassis.moveToPoint(29.5-2,-47.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=40,.earlyExitRange=1},true);
   chassis.waitUntil(24);
   toggleIntake(true,1);
   toggleLoader(false); 


   wait(800);
   chassis.setPose(29.5,-48,chassis.getPose().theta); // fake reset


   // use wing
   /*chassis.moveToPoint(48,-47,1000,{.minSpeed=15},true);
   chassis.waitUntil(1.5);
   chassis.cancelMotion();


   chassis.moveToPoint(40,-35.5,2000,{.minSpeed=30,.earlyExitRange=0.5},false);
   chassis.turnToHeading(90,1500,{.minSpeed=10,.earlyExitRange=1},false);
   chassis.waitUntilDone();
  
   chassis.moveToPoint(12,-35.5,2000,{.forwards=false,.maxSpeed=80,.minSpeed=20,.earlyExitRange=1}); // max speed editable
   chassis.turnToHeading(135,1000,{.minSpeed=30,.earlyExitRange=5},true);
   toggleIntake(false,0);*/
   forward(10,1,1500,30);
   chassis.waitUntilDone();
   pose_chassis.moveToPose(14,-39,90,2500,{.forwards=false,.lead=0.8,.minSpeed=70},true);
   //pose_chassis.waitUntilDone();
   //chassis.turnToHeading(110,1000,{.minSpeed=50},false);
}


// victor
void left_LongCenter(){
   chassis.setPose((48-2.25),-(15.75),270);


   chassis.moveToPoint(22,-22,1500,{.maxSpeed=80,.minSpeed=20,.earlyExitRange=15},true);


   chassis.moveToPoint(20,-24,1000,{.maxSpeed=30,.minSpeed=10,.earlyExitRange=1},true);
   toggleIntake(true,0);
   //wait(300);
   toggleLoader(true);


   chassis.turnToHeading(135,750,{.maxSpeed=80,.minSpeed=10,.earlyExitRange=10},true);


   // 2nd segment
   chassis.moveToPoint(38,-48,2000,{.minSpeed=30},true);
  
   chassis.turnToHeading(90,1000,{.maxSpeed=80,.minSpeed=30},false);


   // long goal first
   chassis.moveToPoint(29.5-2,-48,1300,{.forwards=false,.maxSpeed=80,.minSpeed=40,.earlyExitRange=1},true);
   chassis.waitUntil(7);
   toggleIntake(true,1); // score


   wait(700);
   chassis.setPose(29.5,-48,chassis.getPose().theta); // fake reset
   wait(300);


   // into loader
   chassis.moveToPoint(47,-47.5,1000,{.maxSpeed=80,.minSpeed=40,.earlyExitRange=0},true);
   chassis.moveToPoint(60,-47.5,850,{.maxSpeed=50,.minSpeed=40},true);
   toggleIntake(true,0);


   // go center
   chassis.moveToPoint(10,-10,3000,{.forwards=false,.maxSpeed=90,.minSpeed=10},true);
   chassis.waitUntil(43.5);
   chassis.cancelMotion();


   chassis.moveToPoint(10,-10,1000,{.forwards=false,.maxSpeed=50,.minSpeed=10},true);
   /*toggleIntake(false,0);
   chassis.waitUntil(4);
   toggleIntake(true,-1);*/
   chassis.waitUntil(9); // 10 --> 9


   toggleIntake(true,2);


   //chassis.waitUntil(10);


   wait(900);
  
   /*chassis.moveToPoint(13,-12,500,{.minSpeed=30});
   toggleLoader(false);
   toggleIntake(true,-1);
   chassis.waitUntil(1);
   chassis.cancelMotion();
   chassis.moveToPoint(9+1,-8,500,{.forwards=false,.minSpeed=40});
   toggleIntake(false,0);*/


   // poke
   chassis.moveToPoint(16,-16,1000,{.minSpeed=30});
   toggleLoader(false);
   toggleStick(true);
   toggleIntake(false,0);
   chassis.moveToPoint(10,-10,800,{.forwards=false,.minSpeed=40,.earlyExitRange=2});


   // use wing
   chassis.moveToPoint(30,-37,2000,{.minSpeed=30},true);
   toggleStick(false);
   chassis.turnToHeading(90,1500,{.minSpeed=30,.earlyExitRange=5},true);
   toggleIntake(false,0);
  
   chassis.moveToPoint(12,-37,2000,{.forwards=false,.maxSpeed=80,.minSpeed=30,.earlyExitRange=1}); // max speed editable
   chassis.turnToHeading(110,1000,{.minSpeed=30,.earlyExitRange=2},true);
   toggleIntake(false,0);
}


void left_Long9(){ //
   //chassis.setPose(38.5+8,-(4+13.625/2.0),270);
   chassis.setPose((48-2.25),-(15.75),270);


   chassis.moveToPoint(22,-22,1500,{.minSpeed=20,.earlyExitRange=15},true);
   chassis.moveToPoint(22,-22,1300,{.maxSpeed=30,.minSpeed=30},true);
   toggleIntake(true,0);
   chassis.waitUntil(12);
   toggleLoader(true);


   chassis.moveToPoint(10,-32,1000,{.maxSpeed=80});
   chassis.waitUntil(10);
   toggleLoader(false);
   chassis.turnToHeading(195,500,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=2},true);
  
   chassis.moveToPoint(8,-40,800,{.maxSpeed=80,.minSpeed=25},false);
  
   toggleLoader(true);
   wait(200);


   // (7.8, -39.5, 195)


   chassis.moveToPoint(48,-24,2000,{.forwards=false,.minSpeed=40,.earlyExitRange=36},true);
   chassis.moveToPoint(48,-48,2000,{.forwards=false,.minSpeed=20});


   // 2nd segment
  
   chassis.turnToHeading(90,1500,{},true);


   // then, match load
   toggleLoader(true);


   chassis.waitUntilDone();
   chassis.setPose(48,-readRight_coordinate(70.75),chassis.getPose().theta);


   chassis.moveToPoint(60,-46.5,900,{.maxSpeed=50,.minSpeed=40},true);


   toggleIntake(true, 0);


   // score again
   chassis.moveToPoint(29.5-2,-47.5,1500,{.forwards=false,.earlyExitRange=1},true);
   chassis.waitUntil(20);
   toggleIntake(true,1); // score
   toggleLoader(false); 


   wait(1100); // wait
   chassis.setPose(29.5,-48,chassis.getPose().theta);
   wait(400+500);


   // use wing, OR hood
   // 10.5 displaced
   forward(10,1,1500,40);
   chassis.waitUntilDone();
   pose_chassis.moveToPose(14,-39,90,2500,{.forwards=false,.lead=0.8,.minSpeed=60},true);
}


void left_Long7(){ //
   chassis.setPose((48-2.25),-(15.75),270);


   chassis.moveToPoint(22,-22,1500,{.minSpeed=20,.earlyExitRange=15},true);
   chassis.moveToPoint(22,-22,1300,{.maxSpeed=30,.minSpeed=30},true);
   toggleIntake(true,0);
   toggleLoader(true);


   chassis.turnToHeading(135,1000,{.maxSpeed=80,.minSpeed=15+15,.earlyExitRange=10},false);


   // 2nd segment


   chassis.moveToPoint(48,-47.5,2000,{.minSpeed=10,.earlyExitRange=0},true); // accurate?
  
   chassis.turnToHeading(90,1500,{.maxSpeed=80,.minSpeed=0,.earlyExitRange=5},false);
   //wait(200);


   // then, match load
   chassis.moveToPoint(55+5,-47.5,900,{.maxSpeed=50,.minSpeed=40},true);
  
   toggleIntake(true, 0);
   //pros::delay(500); // load


   // score
   chassis.moveToPoint(29.5-2,-47.5,1500,{.forwards=false,.earlyExitRange=1},true);
   chassis.waitUntil(20);
   toggleIntake(true,1); // score
   toggleLoader(false); 


   wait(1100); // wait
   chassis.setPose(29.5,-48,chassis.getPose().theta); // fake reset
   wait(400);


   // use wing
   // 10.5 displaced
  
   /*chassis.moveToPoint(39,-38-1,1000,{.minSpeed=40},true);
   toggleIntake(false,0);
   chassis.turnToHeading(90,750,{.minSpeed=15+10,.earlyExitRange=2+3},false);
   chassis.moveToPoint(15-5,-38-1,5000,{.forwards=false,.maxSpeed=90,.minSpeed=30}); // change max speed
   chassis.turnToHeading(90+20, 1000, {.minSpeed=20,.earlyExitRange=5}, false);*/


   forward(10,1,1500,30);
   chassis.waitUntilDone();
   pose_chassis.moveToPose(10,-39,90,2500,{.forwards=false,.lead=0.8,.minSpeed=50},true);
   pose_chassis.waitUntilDone();
   chassis.turnToHeading(135,1000,{.minSpeed=50},false);
}


void right_DoubleMid(){
   // to write
   chassis.setPose(47.5,16.427,0);


   // section 1 (initial goal)


   chassis.moveToPoint(47.5,47.5+0.3,1000,{.earlyExitRange=0.3});
   toggleIntake(true,0);


   chassis.turnToHeading(90,700,{.earlyExitRange=1});
   toggleLoader(true);


   // into loader
   chassis.moveToPoint(59,47,1000,{.maxSpeed=50,.minSpeed=40},true);
   wait(1000);


   // grab blocks


   chassis.moveToPoint(47,47,1000,{.forwards=false,.minSpeed=40,.earlyExitRange=2},true);
  
   chassis.waitUntilDone();
   chassis.setPose(readLeft_coordinate(70.5),chassis.getPose().y,chassis.getPose().theta);
   wait(100);
   chassis.turnToHeading(180,1000,{.minSpeed=10,.earlyExitRange=2});
   toggleLoader(false);
   chassis.waitUntilDone();
   forward(5,1,600,50);
   chassis.waitUntilDone();
   chassis.setPose(readLeft_coordinate(70.5),chassis.getPose().y,chassis.getPose().theta);
   wait(100);


   chassis.moveToPoint(22,22,1500,{.minSpeed=50,.earlyExitRange=15},true);
   chassis.moveToPoint(22,22,1300,{.minSpeed=30,.earlyExitRange=4},true);
   chassis.waitUntil(9);
   toggleLoader(true);
   //toggleIntake(true,0);
  


   // 2 under goal


   //chassis.turnToHeading(270+10,1000,{.earlyExitRange=10});
   chassis.moveToPoint(10,32,1000,{.maxSpeed=80,.minSpeed=30});
   chassis.turnToHeading(345,500,{.maxSpeed=80,.minSpeed=30,.earlyExitRange=2},true);
   toggleLoader(false);


   chassis.moveToPoint(8,40,800,{.maxSpeed=80,.minSpeed=25},false);
  
   toggleLoader(true);
   wait(100);


   // lower goal


   chassis.moveToPoint(22+2,22,1500,{.forwards=false},true);
   chassis.turnToHeading(225,1000,{.minSpeed=20,.earlyExitRange=5},false);


   chassis.moveToPoint(10,10-2,1500,{.maxSpeed=60},true);
   //toggleLoader(false);
   chassis.waitUntil(6);
   toggleIntake(true,-1);
   wait(1000);
  
   // upper goal
   chassis.moveToPoint(22,22,2000,{.forwards=false,.minSpeed=20},true);
   chassis.waitUntil(3);
   toggleIntake(true,0);


   chassis.turnToHeading(180,1000,{.minSpeed=30});
   chassis.waitUntilDone();
   chassis.setPose(readLeft_coordinate(70.75),chassis.getPose().y,chassis.getPose().theta);


   chassis.moveToPoint(22,-20,1500,{.minSpeed=50,.earlyExitRange=4},false);
   toggleLoader(true);


   pose_chassis.moveToPose(10,-10+3,135,2000,{.forwards=false,.minSpeed=30},true);


   pose_chassis.waitUntil(16-2);
   toggleIntake(true,2);
   //wait(1500);


   // poke
   /*
   chassis.moveToPoint(16,-16,1000,{.minSpeed=30});
   toggleLoader(false);
   toggleStick(true);
   toggleIntake(false,0);
   chassis.moveToPoint(10,-10,800,{.forwards=false,.minSpeed=20,.earlyExitRange=2});*/
}


void test(){
   chassis.setPose(29.5,48,90);


   chassis.moveToPoint(47,47,3000);
   chassis.turnToHeading(235,2000);
   chassis.moveToPoint(10,10,4000);


   // TEST TRAVERSING AROUND THE FIELD
}


void autonomous() {
   //pros::Task antiJam(antiJamTask);
   //antiJam.remove();
   startTime = pros::millis();


   //right_AWP(); // done
   right_DoubleMid(); // <- to write


   //right_Long7(); // done
   //right_Long9(); // <- TEST
   //right_LowerLong(); // done, ish. not the best routine to run
   //right_LongLower(); // done, can be optimized? lower scoring is a bit too fast
  
   //left_CenterLong(); // done (no poke btw)
   //left_LongCenter(); // done
   //left_Long9(); // done; <- make TWO vers; 1 wing and 1 no wing (close hood)
   //left_Long7(); // done
   //test(); 


   chassis.waitUntilDone();
   pose_chassis.waitUntilDone();
   freezeAt(chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
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

