#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include <cstddef>
#include "subsystemHeaders/global.hpp"


bool isBlue;

int intakeTimer = 0;
bool intakeSlowed = false;

// motor ports + radio port ___
pros::MotorGroup driveLeft({-2, -4, -8}, pros::MotorGearset::blue); // left motor group (reversed)
pros::MotorGroup driveRight({3, 18, 6}, pros::MotorGearset::blue); // right motor group (normal)

// for debugging
pros::Motor left1(-2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left2(-4, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left3(-8, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right1(3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right2(18, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);     
pros::Motor right3(6, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::Motor lowerIntake(10, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor upperIntake(7, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut loaderPiston('B');
pros::adi::DigitalOut scoringModifier('F'); 
//pros::adi::DigitalOut doublePark('D'); 
//pros::adi::DigitalOut doubleParkClamp('C');
pros::adi::DigitalOut wingPiston('E');

pros::adi::DigitalOut lowerIntakeModifier('H');

pros::Imu inertial(17);

pros::Distance right_distanceSensor(20); 
pros::Distance back_distanceSensor(9); 

pros::Rotation vertPod(16); // vert tracking

//LED 
pros::adi::Led led('H',17);
pros::adi::Led led2('B',17); // WIRE

lemlib::TrackingWheel verticalEncoder(&vertPod, 2.0*(48.0/47.6)*(24.0/23.5)/**(24.25/24.0)*(24.5/24.0)*(24.5/24.0)*/,-0.75); // need to edit

lemlib::OdomSensors sensors(&verticalEncoder,nullptr,nullptr,nullptr, &inertial);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              40, // derivative gain (kD)
                                              0, // anti windup
                                              0.3, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              0.7, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              8 // maximum acceleration (slew)
);

/*

9,0,45

lemlib::ControllerSettings lateral_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              40, // derivative gain (kD)
                                              3, // anti windup
                                              0.5, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              8 // maximum acceleration (slew)
);
*/

// angular PID controller
lemlib::ControllerSettings angular_controller(3+0.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              18.2, // derivative gain (kD)
                                              0, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              4, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

/*
lemlib::ControllerSettings angular_controller(2.7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              20, // derivative gain (kD)
                                              0, // anti windup
                                              2, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              5, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
*/

lemlib::Drivetrain drivetrain(&driveLeft, // left motor group
    &driveRight, // right motor group
    11.5, // track width  ; 
    lemlib::Omniwheel::NEW_325,
    450, // drivetrain rpm
    2 // horizontal drift is 2 (for now)
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
    lateral_controller, // lateral PID settings
    angular_controller, // angular PID settings
    sensors // odometry sensors
);

// controller is declared in header class
pros::Controller controller(pros::E_CONTROLLER_MASTER); 