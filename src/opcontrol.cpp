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
	//chassis.setPose(48,-16.427,180);
	while (true) {
		
		
		pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

		//pros::lcd::print(3, "lower: %f", lowerIntake.get_actual_velocity()); // act of lower intake
		//pros::lcd::print(4, "upper: %f", upperIntake.get_actual_velocity()); // act of upper intake

		/*
		pros::lcd::print(0, "left 1: %f", left1.get_temperature()); // 
        pros::lcd::print(1, "left 2: %f", left2.get_temperature()); // 
        pros::lcd::print(2, "left 3: %f", left3.get_temperature()); // 

		pros::lcd::print(3, "right 1: %f", right1.get_temperature()); // 
        pros::lcd::print(4, "right 2: %f", right2.get_temperature()); // 
        pros::lcd::print(5, "right 3: %f", right3.get_temperature()); //  

		pros::lcd::print(6, "lower intake: %f", lowerIntake.get_temperature()); // 
        pros::lcd::print(7, "upper intake: %f", upperIntake.get_temperature()); // 
		*/
		
		setDrive();
		setIntake();
		setLoader();

		pros::delay(15);
	}
}