#include "StateMachine.h"
#include "SensorManager.h"
#include "MotorControl.h"
#include "Shooter.h"

RobotState currentState = STATE_IDLE;

void updateStateMachine() {
    switch (currentState) {
        case STATE_IDLE:
            // TODO
            break;
        case STATE_GLOBAL_SCAN:
            // TODO
            break;
        case STATE_ASSAULT_INNER:
            // TODO
            break;
        case STATE_SHOOT_INNER:
            // TODO
            break;
        case STATE_TACTICAL_RETREAT:
            // TODO
            break;
        case STATE_SHOOT_OUTER:
            // TODO
            break;
        case STATE_RELOAD_AND_REPOSITION:
            // TODO
            break;
    }
}
