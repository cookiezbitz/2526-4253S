#include "main.h"
#include "pros/rtos.hpp"
#include "subsystemHeaders/global.hpp"


int delaySort = 190;
int delayForward = 200;
/*
void driverColorSort(){
    while(true){
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			pros::lcd::set_text(6, "helo");
			if(optical.get_proximity() > 150){
				pros::lcd::set_text(5, "sort");
				if(isBlue){ // blue alliance
					if(optical.get_hue() < 30){  
						// delay a time
						pros::Task::delay(delaySort);

						// intake reverse
						toggleIntake(true,-1);

						// intake fwd again
						pros::Task::delay(delayForward);
						toggleIntake(true,1);
						
					}
				}else{ // red alliance
					if(optical.get_hue() > 50){
						// delay a time
						pros::Task::delay(delaySort);

						// intake reverse
						toggleIntake(true,-1);

						// intake fwd again
						pros::Task::delay(delayForward);
						toggleIntake(true,1);
					}
				}
				pros::Task::delay(200);
        	}else{
				toggleIntake(true,1);
				pros::lcd::set_text(5, "nooo sort");
			}
		}
		pros::delay(15);
    }
}
*/


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	//pros::Task driverColorSortTask(driverColorSort, "Driver Color Sort");
	//chassis.setPose(62,-16.427,270);
	while (true) {
		led.set_all(0X0000FF); // blue = 0X0000FF red = 0XFF0000
		led2.set_all(0X0000FF);
		led.update();
		led2.update();

		pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
		
		setDrive();
		setIntake();
		setLoader();
		//setArm();

		pros::delay(15);
	}
}