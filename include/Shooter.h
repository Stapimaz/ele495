/**
 * @file Shooter.h
 * @brief Solenoid atış mekanizmasının tetikleme ve güç hesaplama fonksiyon tanımlarını içerir.
 */
#ifndef SHOOTER_H
#define SHOOTER_H

#include "Config.h"

void initShooter();
void triggerSolenoid(int pwmIntensity);
int calculateShootingPower(int distanceMm);

#endif // SHOOTER_H
