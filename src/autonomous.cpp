#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

//ASSET(testblueright_txt);


int delayTask = 190;
int delayIntake = 200;

void colorSort(){
    while(true){
        if(optical.get_proximity() > 150){
            if(isBlue){ // blue alliance
                if(optical.get_hue() < 50){  
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                    
                }
            }else{ // red alliance
                if(optical.get_hue() > 150){
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                }
            }
            pros::Task::delay(100);
        }
    }
}
