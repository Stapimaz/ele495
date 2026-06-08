#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "Config.h"

void initMotors();
void driveMotorLeft(int speed);
void driveMotorRight(int speed);
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();
void testMotors();
bool turnToAngle(float targetAngle, float currentAngle);

#endif // MOTOR_CONTROL_H
