//#include "main.h"

extern int intakeState;

void setIntake(); // button control

void toggleIntake(bool status, double state, int volt); // auton overload
void toggleIntake(bool status, double state); // auton

// true = high goal, false = mid goal
void setScoreHeight(bool intakePosition); 

void toggleLowerGoalPiston(bool status);

void setIntakeState(int state);