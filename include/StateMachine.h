/**
 * @file StateMachine.h
 * @brief Robotun otonomi döngüsünün durum değişkenlerini ve fonksiyon prototiplerini içerir.
 */
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Config.h"

void updateStateMachine();
extern RobotState currentState;
extern TargetColor currentTargetColor;
extern int scoreCount; // Aynı yerden alınan skor sayısı

#endif // STATE_MACHINE_H
