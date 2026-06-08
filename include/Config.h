#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// I2C Pinleri
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// VL53L1X ToF Sensör
#define TOF_XSHUT_PIN 4
#define TOF_DEFAULT_ADDR 0x29
#define TOF_NEW_ADDR 0x30

// Diğer I2C Adresleri
#define TCS34725_ADDR 0x29 
#define MPU6050_ADDR 0x68

// TB6612FNG Motor Sürücü
#define MOTOR_LEFT_PWMA 13
#define MOTOR_LEFT_AIN1 14
#define MOTOR_LEFT_AIN2 27

#define MOTOR_RIGHT_PWMB 25
#define MOTOR_RIGHT_BIN1 26
#define MOTOR_RIGHT_BIN2 33

// JF-0530B Solenoid
#define SOLENOID_PWM_PIN 32

// State Machine Enum Tanımlaması
enum RobotState {
    STATE_IDLE,
    STATE_GLOBAL_SCAN,
    STATE_ASSAULT_INNER,
    STATE_SHOOT_INNER,
    STATE_TACTICAL_RETREAT,
    STATE_SHOOT_OUTER,
    STATE_RELOAD_AND_REPOSITION
};

#endif // CONFIG_H
