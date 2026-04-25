#include "main.h"
#include "pros/misc.h"
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

void intake(){
	while(true){
		setIntake();
		
		pros::Task::delay(15);
	}
}

void swap_endian(uint8_t* data, size_t size) {
    for (size_t i = 0; i < size - 1; i += 2) {
        uint8_t tmp = data[i];
        data[i] = data[i + 1];
        data[i + 1] = tmp;
    }
}

void swap_rgb(uint8_t* data, size_t size) {
    for (size_t i = 0; i < size - 1; i += 2) {
        uint16_t pixel = data[i] | (data[i+1] << 8);
        uint16_t r = (pixel >> 11) & 0x1F;
        uint16_t g = (pixel >> 5) & 0x3F;
        uint16_t b = pixel & 0x1F;
        uint16_t swapped = (b << 11) | (g << 5) | r;
        data[i] = swapped & 0xFF;
        data[i+1] = (swapped >> 8) & 0xFF;
    }
}

extern "C" { extern lv_img_dsc_t smallstella; }


void opcontrol() {
	//pros::Task driverColorSortTask(driverColorSort, "Driver Color Sort");
	//chassis.setPose(38.5+8,(4+13.625/2.0),270);
	//pros::Task intakePistonRumble(intakePistonIndicator, "Intake Piston Rumble");
	//intakePistonRumble.resume();
	//chassis.setPose(38.5+8,-(4+13.625/2.0),270);
	pros::Task intakeTask(intake, "Intake Task");

	/*pros::lcd::clear();

	lv_obj_t* rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, 600, 400);
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(rect, lv_color_hex(0x04090a), 0);*/

	
	// Declare the image
	//LV_IMG_DECLARE(smallstella);
	//swap_endian((uint8_t*)smallstella.data, smallstella.data_size);
	//swap_rgb((uint8_t*)smallstella.data, smallstella.data_size);
	
	/*
	lv_obj_t* image = lv_img_create(lv_scr_act());
	lv_img_set_src(image, &smallstella);
	//lv_obj_set_size(image, 480, 272);
	lv_obj_align(image, LV_ALIGN_CENTER, 0, 0);*/
	
    
	while (true) {
		//lv_timer_handler();

		//controller.print(0, 0, "R: %.2f", right_distanceSensor.get_distance()/25.4); // mm to inches
		//controller.print(0, 0, "B: %.2f", back_distanceSensor.get_distance()/25.4); // mm to inches
		
		pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading 



		//pros::lcd::print(3, "lower: %f", lowerIntake.get_actual_velocity()); // act of lower intake
		//pros::lcd::print(4, "upper: %f", upperIntake.get_actual_velocity()); // act of upper intake


		/*
		pros::lcd::print(0, "left 1: %f", left1.get_temperature()); // 
        pros::lcd::print(1, "left 2: %f", left2.get_temperature()); // 
        pros::lcd::print(2, "left 3: %f", left3.get_temperature()); */

		pros::lcd::print(3, "left 1: %f  right 1: %f", left1.get_temperature(), right1.get_temperature()); // 
        pros::lcd::print(4, "left 2: %f  right 2: %f", left2.get_temperature(), right2.get_temperature()); // 
        pros::lcd::print(5, "left 3: %f  right 3: %f", left3.get_temperature(), right3.get_temperature()); //  

		pros::lcd::print(6, "lower intake: %f", lowerIntake.get_temperature()); // 
        pros::lcd::print(7, "upper intake: %f", upperIntake.get_temperature()); 

		// */
		
		/*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
			chassis.setPose(0,0,0);
			chassis.moveToPoint(0,12,2000);
			pros::delay(1000);
			pose_chassis.moveToPose(-24,24,270,5000);
		}*/
		setDrive();
		//setIntake();
		//setLoader();

		//count+=15;
		pros::delay(15);
	}
}