//#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "pros/adi.hpp"
#include "pros/colors.hpp"
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

extern pros::Motor lowerIntake;
extern pros::Motor midIntake;
extern pros::Motor upperIntake;

extern pros::adi::DigitalOut loaderPiston;

extern pros::Imu inertial;

//
extern pros::Optical optical;

extern pros::Rotation arm_rotation;
extern pros::Rotation xPod;

extern lemlib::Chassis chassis;

extern pros::Task colorSortTask;

// controller declaration
extern pros::Controller controller;


