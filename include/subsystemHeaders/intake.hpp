//#include "main.h"

extern bool intakeOn;
extern bool intakeReversed;

void setIntake(); // button control

void toggleIntake(bool status, double state); //auton

void toggleWeak(double direction);

void score(int breakoutTime = 1000);

void moveAndScore(double distance, int timeout);