#include <Arduino.h>
#include "Config.h"
#include "MotorControl.h"
// Şimdilik diğer modülleri deaktif ediyoruz
#include "SensorManager.h" // MPU6050 testi için aktif
// #include "Shooter.h"
// #include "Comms.h"
// #include "StateMachine.h"

TaskHandle_t NetworkTaskHandle;
TaskHandle_t AutonomyTaskHandle;

// Core 0: Sadece UDP dinleme ve ağ işlemleri
void networkTask(void *pvParameters) {
    // initComms();
    for (;;) {
        // Haberleşmeyi şimdilik boş bırakıyoruz
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}

int imuTestState = 0; // MPU6050 Testi için senaryo durumu

// Core 1: Sensörler, Karar mekanizması ve Motor Kontrolü
void autonomyTask(void *pvParameters) {
    initMotors();
    initMPU(); // MPU6050'yi baslat ve I2C üzerinden kalibre et

    for (;;) {
        // Her dongude MPU acisini tazele
        updateMPU();
        float currentAngle = getAngleZ();

        // 4 Adimli basit kapali cevrim otonom test
        switch (imuTestState) {
            case 0:
                // Sola dogru 15 derece dönene kadar bekle
                if (turnToAngle(15.0, currentAngle)) {
                    imuTestState = 1;
                }
                break;
            case 1:
                stopMotors();
                vTaskDelay(pdMS_TO_TICKS(2000)); // 2 saniye heykeli oyna
                imuTestState = 2;
                break;
            case 2:
                // 0 dereceye (baslangic) geri dön
                if (turnToAngle(0.0, currentAngle)) {
                    imuTestState = 3;
                }
                break;
            case 3:
                stopMotors();
                vTaskDelay(pdMS_TO_TICKS(2000)); // 2 saniye bekle
                imuTestState = 0; // Tekrar bastır
                break;
        }

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
