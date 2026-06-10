/**
 * @file main.cpp
 * @brief ESP32 ana giriş dosyası. FreeRTOS task'larını (Core 0: Network, Core 1: Otonomi) 
 *        başlatır ve pin kurulumlarını çağırarak sistemi ayağa kaldırır.
 */
#include <Arduino.h>
#include "Config.h"
#include "MotorControl.h"
// Tüm modülleri aktif ediyoruz
#include "SensorManager.h"
#include "Shooter.h"
#include "Comms.h"
#include "StateMachine.h"

TaskHandle_t NetworkTaskHandle;
TaskHandle_t AutonomyTaskHandle;

// Core 0: Sadece UDP dinleme ve ağ işlemleri
void networkTask(void *pvParameters) {
    initComms(); // WiFi AP ve UDP Sunucusunu Başlat
    for (;;) {
        checkForIncomingCommands(); // "START" komutu bekle
        sendTelemetryData();        // JSON verisi gönder
        vTaskDelay(pdMS_TO_TICKS(10)); // FreeRTOS watchdog için bekleme
    }
}

// Core 1: Sensörler, Karar mekanizması ve Motor Kontrolü
void autonomyTask(void *pvParameters) {
    initSensors(); // Tüm I2C sensörlerini (ToF, Renk, MPU) başlat
    initMotors();
    initShooter();

    for (;;) {
        // Her dongude MPU acisini tazele (Otonomi için hayati önem taşır)
        updateMPU();

        // Ana Durum Makinesini (State Machine) İşlet
        updateStateMachine();

        vTaskDelay(pdMS_TO_TICKS(10)); // FreeRTOS çökmemesi için bekleme (Zorunlu)
    }
}

void setup() {
    Serial.begin(115200);
    
    // Core 0'da Ağ (Network) Task'ini Başlatma
    xTaskCreatePinnedToCore(
        networkTask,        
        "NetworkTask",      
        4096,               
        NULL,               
        1,                  
        &NetworkTaskHandle, 
        0                   
    );

    // Core 1'de Otonomi (Autonomy) Task'ini Başlatma
    xTaskCreatePinnedToCore(
        autonomyTask,       
        "AutonomyTask",     
        4096,               
        NULL,               
        1,                  
        &AutonomyTaskHandle,
        1                   
    );
}

void loop() {
    // Standart loop boş bırakılır. FreeRTOS taskları işleri devralır.
    vTaskDelete(NULL); 
}
