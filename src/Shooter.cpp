#include "Shooter.h"

void initShooter() {
    pinMode(SOLENOID_PWM_PIN, OUTPUT);
    // TODO: Setup PWM using ledcSetup
}

void triggerSolenoid(int pwmIntensity) {
    // TODO: ledcWrite ile PWM sinyali gönder
}
