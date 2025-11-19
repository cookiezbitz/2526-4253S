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
pros::MotorGroup driveLeft({-11, -14, -2}, pros::MotorGearset::blue); // left motor group (reversed)
pros::MotorGroup driveRight({3, 1, 12}, pros::MotorGearset::blue); // right motor group (normal)

// for debugging
pros::Motor left1(-11, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left2(-14, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left3(-2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right1(3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right2(12, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);     
pros::Motor right3(1, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);


pros::Motor lowerIntake(16, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor midIntake(10, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);
pros::Motor upperIntake(9, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut loaderPiston('A');

pros::Imu inertial(4);

pros::Rotation vertPod(16); // vert tracking

//LED 
pros::adi::Led led('A',17);
pros::adi::Led led2('B',17);

// unedited
pros::Optical optical(1);

pros::Rotation arm_rotation(-14); 


lemlib::TrackingWheel horizontalEncoder(&vertPod, lemlib::Omniwheel::NEW_2,-4);

lemlib::OdomSensors sensors(nullptr,nullptr,&horizontalEncoder,nullptr, &inertial);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              8, // derivative gain (kD)
                                              3, // anti windup
                                              0.5, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              25 // maximum acceleration (slew)
);

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