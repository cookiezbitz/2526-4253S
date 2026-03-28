//#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "pros/adi.hpp"
#include "pros/colors.hpp"
#include "pros/distance.hpp"
#include "pros/imu.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"
#include <algorithm>
#include <cstddef>

extern bool isBlue;

// motors declaration
extern pros::MotorGroup driveLeft;
extern pros::MotorGroup driveRight;

extern pros::Motor left1;
extern pros::Motor left2;
extern pros::Motor left3;
extern pros::Motor right1;
extern pros::Motor right2;
extern pros::Motor right3;

extern pros::MotorGroup lowerIntake;
extern pros::Motor upperIntake;

extern pros::adi::DigitalOut loaderPiston;
extern pros::adi::DigitalOut centerDescore;
extern pros::adi::DigitalOut wingPiston;
extern pros::adi::DigitalOut hoodPiston;

extern pros::Imu inertial;

extern pros::Distance right_distanceSensor;
extern pros::Distance back_distanceSensor;
extern pros::Distance front_distanceSensor;
extern pros::Optical intake_optical;

extern pros::adi::Led led;
extern pros::adi::Led led2;

extern pros::Rotation vertPod;

extern lemlib::Chassis chassis;
extern lemlib::Chassis pose_chassis;

extern pros::Task intakeTask;
extern pros::Task colorSortTask;

// controller declaration
extern pros::Controller controller;


