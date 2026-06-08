#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Config.h"
#include <Wire.h>

void initSensors();
void resolveI2CConflict();
int readToFDistance();
void readColorData(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c);
void readIMUData(float &yaw, float &pitch, float &roll);

// MPU6050 Testi için
void initMPU();
void updateMPU();
float getAngleZ();

#endif // SENSOR_MANAGER_H
