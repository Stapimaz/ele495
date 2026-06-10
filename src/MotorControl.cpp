/**
 * @file MotorControl.cpp
 * @brief Robotun L298N sürücü ve tekerlek motorlarının sürüşünü yönetir. 
 *        MPU6050 ile açı koruma ve P-Kontrol algoritmalarını barındırır.
 */
#include "MotorControl.h"

// ESP32 PWM Ayarları
const int pwmFreq = 5000;
const int pwmResolution = 8;     // 0-255 arası
const int leftPwmChannel = 0;
const int rightPwmChannel = 1;

void initMotors() {
    // Yön pinlerini OUTPUT olarak ayarla
    pinMode(MOTOR_LEFT_IN1, OUTPUT);
    pinMode(MOTOR_LEFT_IN2, OUTPUT);
    
    pinMode(MOTOR_RIGHT_IN3, OUTPUT);
    pinMode(MOTOR_RIGHT_IN4, OUTPUT);

    // PWM Kanallarını ayarla ve pinlere bağla
    ledcSetup(leftPwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(MOTOR_LEFT_ENA, leftPwmChannel);

    ledcSetup(rightPwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(MOTOR_RIGHT_ENB, rightPwmChannel);
    
    // Motorları başlangıçta durdur
    stopMotors();
}

void driveMotorLeft(int speed) {
    if (speed > 0) {
        digitalWrite(MOTOR_LEFT_IN1, HIGH);
        digitalWrite(MOTOR_LEFT_IN2, LOW);
        ledcWrite(leftPwmChannel, speed);
    } else if (speed < 0) {
        digitalWrite(MOTOR_LEFT_IN1, LOW);
        digitalWrite(MOTOR_LEFT_IN2, HIGH);
        ledcWrite(leftPwmChannel, -speed);
    } else {
        digitalWrite(MOTOR_LEFT_IN1, LOW);
        digitalWrite(MOTOR_LEFT_IN2, LOW);
        ledcWrite(leftPwmChannel, 0);
    }
}

void driveMotorRight(int speed) {
    if (speed > 0) {
        digitalWrite(MOTOR_RIGHT_IN3, HIGH);
        digitalWrite(MOTOR_RIGHT_IN4, LOW);
        ledcWrite(rightPwmChannel, speed);
    } else if (speed < 0) {
        digitalWrite(MOTOR_RIGHT_IN3, LOW);
        digitalWrite(MOTOR_RIGHT_IN4, HIGH);
        ledcWrite(rightPwmChannel, -speed);
    } else {
        digitalWrite(MOTOR_RIGHT_IN3, LOW);
        digitalWrite(MOTOR_RIGHT_IN4, LOW);
        ledcWrite(rightPwmChannel, 0);
    }
}

void moveForward(int speed) { 
    driveMotorLeft(speed); 
    driveMotorRight(speed); 
}

void moveBackward(int speed) { 
    driveMotorLeft(-speed); 
    driveMotorRight(-speed); 
}

void stopMotors() { 
    driveMotorLeft(0); 
    driveMotorRight(0); 
}

void turnLeft(int speed) { 
    driveMotorLeft(-speed); 
    driveMotorRight(speed); 
}

void turnRight(int speed) { 
    driveMotorLeft(speed); 
    driveMotorRight(-speed); 
}

bool turnToAngle(float targetAngle, float currentAngle) {
    float difference = targetAngle - currentAngle;
    
    // Eger fark +- 2 derece icerisindeyse hedefe vardık
    if (abs(difference) <= 2.0) {
        stopMotors();
        return true; 
    }
    
    // Oransal (P) Kontrol
    float kP = 2.0; // P kazancı (ileride optimize edilebilir)
    int turnSpeed = abs(difference) * kP;
    
    // Min ve Max hız limitleri (ölü bandı aşmak için min hız)
    if (turnSpeed < 80) turnSpeed = 80;   
    if (turnSpeed > 255) turnSpeed = 255;
    
    // Fark pozitifse sola, negatifse sağa don
    if (difference > 0) {
        turnLeft(turnSpeed);
    } else {
        turnRight(turnSpeed);
    }
    
    return false;
}

void testMotors() {
    // 2 saniye tam gaz ileri
    moveForward(255);
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 1 saniye dur
    stopMotors();
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // 2 saniye tam gaz geri
    moveBackward(255);
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 1 saniye dur
    stopMotors();
    vTaskDelay(pdMS_TO_TICKS(1000));
}
