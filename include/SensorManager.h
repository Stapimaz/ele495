/**
 * @file SensorManager.h
 * @brief I2C sensörlerinin (VL53L1X, TCS34725, MPU6050) fonksiyon tanımlamalarını içerir.
 */
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Config.h"
#include <Wire.h>
#include <VL53L1X.h>
#include <Adafruit_TCS34725.h>

extern VL53L1X tofSensor;
extern Adafruit_TCS34725 colorSensor;

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
