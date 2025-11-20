#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include <cmath>

//ASSET(testblueright_txt);


int delayTask = 190;
int delayIntake = 200;

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

void AWPleft(){ // based on altLeft
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
    // go to x, -48 to set up for nelson
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

}

void skills(){
    chassis.setPose(-62,-15,90);
    chassis.moveToPose(-24,-23,110,5000);
    toggleIntake(true,0);
    chassis.moveToPose(-63,-48.779,270,5000);
    chassis.moveToPose(-65,-48.877,270,5000);
}

void autonomous() {
    //redRight();
    //redLeft();
    //blueRight();
    //blueLeft();

    //basic();
    
    //leftAuton();
    altLeftAuton();
    //AWPright();
    //AWPleft();
    //skills();
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
        double delta_heading = current_heading - prev_heading;
        prev_heading = current_heading; // save current heading as prev for next loop

        double current_rotation = vertPod.get_position()/100.0;
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

    double path_radius = (total_error * total_error) / (2 * error_x); // radius of the circular path to follow

    double direct_power;
    double left_power;
    double right_power;

    while(total_error >= early_exit_range || time_elapsed < timeout){
        error_x = goal_x - x_global;
        error_y = goal_y - y_global; 
        total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2))); // update errors

        direct_power = drivePID(total_error); // [0, 100]

        left_power = direct_power + (track_width * direct_power)/(2 * path_radius);
        right_power = direct_power - (track_width * direct_power)/(2 * path_radius);
        
        driveLeft.move(left_power);
        driveRight.move(right_power);

        pros::Task::delay(15);
        time_elapsed += 15;
    }
}

void move_to_pose(double goal_x, double goal_y, double goal_theta, double lead_distance, double timeout, double early_exit_range){
    time_elapsed = 0; // reset time elapsed at start of movement

    double total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2))); // also equal to lookahead distance

    double error_x = goal_x - x_global;
    double error_y = goal_y - y_global;

    double carrot_x = goal_x - (total_error * lead_distance * sin(goal_theta)); // NEW
    double carrot_y = goal_y - (total_error * lead_distance * cos(goal_theta)); // NEW

    double carrot_error = sqrt((pow(carrot_x - x_global, 2) + pow(carrot_y - y_global, 2)));; // NEW

    double path_radius = (carrot_error * carrot_error) / (2 * error_x); // radius of the circular path to follow

    double direct_power;
    double left_power;
    double right_power;

    while(total_error >= early_exit_range || time_elapsed < timeout){
        error_x = goal_x - x_global;
        error_y = goal_y - y_global; 
        total_error = sqrt((pow(goal_x - x_global, 2) + pow(goal_y - y_global, 2))); // TO GOAL POINT
        
        // update carrot point + errors
        carrot_x = goal_x - (total_error * lead_distance * sin(goal_theta)); //
        carrot_y = goal_y - (total_error * lead_distance * cos(goal_theta)); //

        carrot_error = sqrt((pow(carrot_x - x_global, 2) + pow(carrot_y - y_global, 2))); // TO CARROT POINT


        direct_power = drivePID(carrot_error); // [0, 100]  edited

        left_power = direct_power + (track_width * direct_power)/(2 * path_radius);
        right_power = direct_power - (track_width * direct_power)/(2 * path_radius);
        
        driveLeft.move(left_power);
        driveRight.move(right_power);

        pros::Task::delay(15);
        time_elapsed += 15;
    }
}