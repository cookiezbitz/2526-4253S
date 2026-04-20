#include "liblvgl/llemu.hpp"
#include "main.h"
#include "pros/motors.h"
#include "subsystemHeaders/global.hpp"


void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

//extern "C" { extern lv_img_dsc_t stella; }
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::register_btn1_cb(on_center_button);
	//pros::lcd::set_text(0, "Initializing...");

	driveLeft.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
	driveRight.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
	
	lowerIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	upperIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); //

	lowerIntake.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	upperIntake.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	
	// pneumatics
	loaderPiston.set_value(false); 
	centerDescore.set_value(false); 
	wingPiston.set_value(false); 
	hoodPiston.set_value(true);

	//

	inertial.reset();
	inertial.tare_heading();

	intake_optical.set_led_pwm(50);

	// reset and encoders
	vertPod.reset_position();
	vertPod.set_data_rate(15);
	//vertPod.set_reversed(true);

	chassis.calibrate();

	/*xPod.reset_position();
	xPod.set_data_rate(15);
	xPod.set_reversed(true);*/

	//pros::lcd::set_text(0, "4253S bot is READY! :D");
	
	//lemlib::Pose pose = chassis.getPose();
    
	

	/*pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

			//pros::lcd::set_text(0, "Left: " + std::to_string(driveLeft.get_position()));
			//pros::lcd::set_text(1, "Right: " + std::to_string(driveRight.get_position()));

			// print the x, y, and theta values of the pose

			
			/*pros::lcd::set_text(0, "Left1: "+ std::to_string(left1.get_position()));
			pros::lcd::set_text(1, "Left2: "+ std::to_string(left2.get_position()));
			pros::lcd::set_text(2, "Left3: "+ std::to_string(left3.get_position()));
			pros::lcd::set_text(3, "Right1: "+ std::to_string(right1.get_position()));
			pros::lcd::set_text(4, "Right2: "+ std::to_string(right2.get_position()));
			pros::lcd::set_text(5, "Right3: "+ std::to_string(right3.get_position()));
			

            // delay to save resources
            pros::delay(20);
        }
    });*/

	
}