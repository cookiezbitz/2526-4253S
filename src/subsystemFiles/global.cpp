#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "pros/abstract_motor.hpp"
#include "pros/motors.hpp"
#include <cstddef>
#include "subsystemHeaders/global.hpp"


bool isBlue;
bool armGoalPosition;

int intakeTimer = 0;
bool intakeSlowed = false;

// motor ports + radio port ___
pros::MotorGroup driveLeft({-2, -4, -8}, pros::MotorGearset::blue); // left motor group (reversed)
pros::MotorGroup driveRight({3, 9, 10}, pros::MotorGearset::blue); // right motor group (normal)

// for debugging
pros::Motor left1(-2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left2(-4, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left3(-8, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right1(3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right2(9, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);     
pros::Motor right3(10, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);


pros::Motor lowerIntake(6, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
//pros::Motor midIntake(10, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);
pros::Motor upperIntake(7, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut loaderPiston('B');
pros::adi::DigitalOut scoringModifier('B'); // WIRE
pros::adi::DigitalOut doublePark('C'); // WIRE
pros::adi::DigitalOut doubleParkClamp('C');


pros::Imu inertial(4);

pros::Rotation vertPod(19); // vert tracking

//LED 
pros::adi::Led led('H',17);
pros::adi::Led led2('B',17); // WIRE

lemlib::TrackingWheel verticalEncoder(&vertPod, lemlib::Omniwheel::NEW_2,-4); // need to edit

lemlib::OdomSensors sensors(nullptr,nullptr,nullptr,nullptr, &inertial);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              300, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              800, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

/*
lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              8, // derivative gain (kD)
                                              3, // anti windup
                                              0.5, // small error range, in inches
                                              300, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              800, // large error range timeout, in milliseconds
                                              25 // maximum acceleration (slew)
);
*/

// angular PID controller
lemlib::ControllerSettings angular_controller(1.4, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              0, // derivative gain (kD)
                                              10, // anti windup
                                              2, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              5, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

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