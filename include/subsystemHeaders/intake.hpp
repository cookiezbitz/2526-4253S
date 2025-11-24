//#include "main.h"

extern bool intakeOn;
extern bool intakeReversed;

void setIntake(); // button control

void toggleIntake(bool status, double state); // auton

// true = high goal, false = mid goal
void setScoreHeight(bool intakePosition); 