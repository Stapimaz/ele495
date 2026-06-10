#include "Shooter.h"

const int solenoidPwmChannel = 2; // Left:0, Right:1, Solenoid:2

void initShooter() {
    ledcSetup(solenoidPwmChannel, 5000, 8);
    ledcAttachPin(SOLENOID_PWM_PIN, solenoidPwmChannel);
    ledcWrite(solenoidPwmChannel, 0); // Başlangıçta kapalı
}

void triggerSolenoid(int pwmIntensity) {
    ledcWrite(solenoidPwmChannel, pwmIntensity);
    vTaskDelay(pdMS_TO_TICKS(150)); // 150ms enerji ver
    ledcWrite(solenoidPwmChannel, 0); // Kapat
}

int calculateShootingPower(int distanceMm) {
    // Mesafeyi sınırlar içine hapset (clamp)
    if (distanceMm < DISTANCE_MIN_MM) distanceMm = DISTANCE_MIN_MM;
    if (distanceMm > DISTANCE_MAX_MM) distanceMm = DISTANCE_MAX_MM;
    
    // map() ile mesafeyi PWM gücüne orantıla (Doğrusal interpolasyon)
    long pwm = map(distanceMm, DISTANCE_MIN_MM, DISTANCE_MAX_MM, PWM_MIN_STRENGTH, PWM_MAX_STRENGTH);
    return (int)pwm;
}
