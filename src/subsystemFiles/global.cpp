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

// motor ports + radio port 11, 5
pros::MotorGroup driveLeft({-2, -3, -12}, pros::MotorGearset::blue,pros::MotorEncoderUnits::degrees); // left motor group (reversed)
pros::MotorGroup driveRight({8, 9, 19}, pros::MotorGearset::blue,pros::MotorEncoderUnits::degrees); // right motor group (normal)

// for debugging
pros::Motor left1(-2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left2(-3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor left3(-12, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right1(8, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor right2(9, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);     
pros::Motor right3(19, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::MotorGroup lowerIntake({-1,10}, pros::MotorGearset::blue,pros::MotorEncoderUnits::degrees);
pros::Motor upperIntake(7, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);

pros::adi::DigitalOut loaderPiston('A');
pros::adi::DigitalOut centerDescore('C'); 
pros::adi::DigitalOut wingPiston('B');
pros::adi::DigitalOut hoodPiston('D');

pros::Imu inertial(18);
// pros radio port: 11, 5

pros::Distance right_distanceSensor(20); 
pros::Distance back_distanceSensor(4); 
pros::Distance front_distanceSensor(13);
pros::Optical intake_optical(15);

pros::Rotation vertPod(-14); // vert tracking

//LED 
//pros::adi::Led led('H',17);
//pros::adi::Led led2('B',17); // WIRE

lemlib::TrackingWheel verticalEncoder(&vertPod, 2.0,+(13.75/2 - 5.5 -11.0/12)); // to the right

lemlib::OdomSensors sensors(&verticalEncoder,nullptr,nullptr,nullptr, &inertial);

// NORMAL point by point, turning settings
lemlib::ControllerSettings lateral_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              52, // derivative gain (kD)
                                              0, // anti windup
                                              0.3, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              0.7, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              8 // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(3.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              26, // derivative gain (kD)
                                              0, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              300, // large error range timeout, in milliseconds
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

// MOVE TO POSE settings
lemlib::ControllerSettings pose_lateral_controller(11.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              55, // derivative gain (kD)
                                              0, // anti windup
                                              0.3, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              0.7, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              8 // maximum acceleration (slew)
);

lemlib::ControllerSettings pose_angular_controller(3.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              26, // derivative gain (kD)
                                              0, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              300, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);


lemlib::Chassis pose_chassis(drivetrain, // drivetrain settings
    pose_lateral_controller, // lateral PID settings
    pose_angular_controller, // angular PID settings
    sensors // odometry sensors 
);

// controller is declared in header class
pros::Controller controller(pros::E_CONTROLLER_MASTER); 