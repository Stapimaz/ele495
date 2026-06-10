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

// L298N Motor Sürücü
#define MOTOR_LEFT_ENA 13
#define MOTOR_LEFT_IN1 14
#define MOTOR_LEFT_IN2 27

#define MOTOR_RIGHT_ENB 25
#define MOTOR_RIGHT_IN3 26
#define MOTOR_RIGHT_IN4 33

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

// Otonomi ve Algoritma Parametreleri
// Kenar Tarama Eşiği (cm cinsinden ToF okuması) - Pota algılama mesafesi
#define EDGE_SCAN_THRESHOLD_MM 150 // 15 cm (VL53L1X mm okur)

// UDP Başlatma Komutu
#define UDP_START_COMMAND "START"

// Atış Şiddeti (PWM) Hesaplama Parametreleri
// Ölçülen minimum ve maksimum mesafeler (mm)
#define DISTANCE_MIN_MM 300   // Örn: 30 cm
#define DISTANCE_MAX_MM 1500  // Örn: 150 cm

// Bu mesafelere karşılık gelen PWM değerleri (0-255)
#define PWM_MIN_STRENGTH 100
#define PWM_MAX_STRENGTH 255

// Ağ (WiFi) ve UDP Parametreleri
#define AP_SSID "ELE495_ROBOT"
#define AP_PASS "12345678" // Minimum 8 karakter olmalı
#define UDP_PORT 4242

#endif // CONFIG_H
