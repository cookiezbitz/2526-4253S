#include "main.h"
#include "pros/rtos.hpp"
#include "subsystemHeaders/global.hpp"
#include <bit>


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

void intakePistonIndicator(){
	controller.clear();
	while(true){
		if(intakePistonStatus==false){
			controller.rumble("- .");
			controller.print(1,1,"rumbling");
		}
		pros::Task::delay(1000);
	}
}

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

int count = 0;
void opcontrol() {
	//pros::Task driverColorSortTask(driverColorSort, "Driver Color Sort");
	
	//pros::Task intakePistonRumble(intakePistonIndicator, "Intake Piston Rumble");
	//intakePistonRumble.resume();
	
	while (true) {

		//controller.print(0, 0, "R: %.2f", right_distanceSensor.get_distance()/25.4); // mm to inches
		//controller.print(0, 0, "B: %.2f", back_distanceSensor.get_distance()/25.4); // mm to inches
		
		
		pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading 
		

		//pros::lcd::print(3, "lower: %f", lowerIntake.get_actual_velocity()); // act of lower intake
		//pros::lcd::print(4, "upper: %f", upperIntake.get_actual_velocity()); // act of upper intake


		// /*
		//pros::lcd::print(0, "left 1: %f", left1.get_temperature()); // 
        //pros::lcd::print(1, "left 2: %f", left2.get_temperature()); // 
        //pros::lcd::print(2, "left 3: %f", left3.get_temperature()); //

		pros::lcd::print(3, "left 1: %f  right 1: %f", left1.get_temperature(), right1.get_temperature()); // 
        pros::lcd::print(4, "left 2: %f  right 2: %f", left2.get_temperature(), right2.get_temperature()); // 
        pros::lcd::print(5, "left 3: %f  right 3: %f", left3.get_temperature(), right3.get_temperature()); //  

		pros::lcd::print(6, "lower intake: %f", lowerIntake.get_temperature()); // 
        pros::lcd::print(7, "upper intake: %f", upperIntake.get_temperature()); // 

		// */
		
		if (count % 300 == 0 && intakePistonStatus == false) {
	    	// Only send every 300ms
	    	controller.rumble("- .");
			//controller.print(0, 0, "B: %.f", count); // mm to inches
	   	}
		
		setDrive();
		setIntake();
		setLoader();

		count+=15;
		pros::delay(15);
	}
}