#include "StateMachine.h"
#include "SensorManager.h"
#include "MotorControl.h"
#include "Shooter.h"

RobotState currentState = STATE_IDLE;
TargetColor currentTargetColor = COLOR_NONE;
int scoreCount = 0; // Aynı yerden alınan skor sayısı

// Tarama ve hedefleme için lokal değişkenler
static int scanStep = 0;
static float angle1 = 0;
static float angle2 = 0;
static float targetCenterAngle = 0;

// Zamanlayıcı (Timer) değişkeni
static unsigned long prepareTimer = 0;

void updateStateMachine() {
    switch (currentState) {
        case STATE_IDLE:
            stopMotors();
            scanStep = 0; // Bir sonraki döngü için sıfırla
            break;
            
        case STATE_GLOBAL_SCAN:
            // Adım 0: Sol kenarı bul (Mesafe eşiğin altına düşünce potaya gelmişiz demektir)
            if (scanStep == 0) {
                turnLeft(120); // Yavaşça dön
                int dist = readToFDistance();
                if (dist > 0 && dist < EDGE_SCAN_THRESHOLD_MM) {
                    angle1 = getAngleZ();
                    scanStep = 1;
                    Serial.println("Sol kenar bulundu.");
                }
            }
            // Adım 1: Sağ kenarı bul (Pota paneli bitip tekrar eşiğin üstüne çıkana kadar)
            else if (scanStep == 1) {
                turnLeft(120);
                int dist = readToFDistance();
                if (dist >= EDGE_SCAN_THRESHOLD_MM) {
                    angle2 = getAngleZ();
                    targetCenterAngle = (angle1 + angle2) / 2.0;
                    scanStep = 2;
                    Serial.println("Sag kenar bulundu, merkeze donuluyor...");
                }
            }
            // Adım 2: Hesaplanmış merkeze kilitlen
            else if (scanStep == 2) {
                if (turnToAngle(targetCenterAngle, getAngleZ())) {
                    Serial.println("Merkeze kilitlenildi. Rota basliyor.");
                    currentState = STATE_DRIVE_TO_ZONE;
                }
            }
            break;
            
        case STATE_DRIVE_TO_ZONE:
            {
                // MPU ile düz hatta sürüş (Açı koruma - Doğrusal Seyrüsefer)
                float currentAngle = getAngleZ();
                float diff = targetCenterAngle - currentAngle;
                int baseSpeed = 150;
                int correction = diff * 4.0; // P Kazancı
                
                // Rengi Oku
                uint16_t r, g, b, c;
                readColorData(r, g, b, c);
                
                bool colorMatched = false;
                // NOT: Renk sensörü eşikleri donanıma ve ışığa göre test edilip güncellenmeli!
                if (currentTargetColor == COLOR_TURQUOISE) {
                    if (b > r && b > g && c > 100) colorMatched = true; // Basit Mavi/Turkuaz algılama
                } else if (currentTargetColor == COLOR_YELLOW) {
                    if (r > b && g > b && c > 100) colorMatched = true; // Basit Sarı algılama
                }
                
                if (colorMatched) {
                    Serial.println("Hedef bolgeye ulasildi. Bekleme modu aktif!");
                    // Zıt PWM ile frenleme yapılabilir (opsiyonel), şimdilik sadece dur:
                    stopMotors();
                    
                    prepareTimer = millis(); // Kronometreyi başlat
                    currentState = STATE_PREPARE_SHOOT;
                } else {
                    // Hedef sarıysa (dış) geri, turkuazsa (iç) ileri
                    if (currentTargetColor == COLOR_YELLOW) {
                        driveMotorLeft(-(baseSpeed - correction));
                        driveMotorRight(-(baseSpeed + correction));
                    } else {
                        driveMotorLeft(baseSpeed - correction);
                        driveMotorRight(baseSpeed + correction);
                    }
                }
            }
            break;

        case STATE_PREPARE_SHOOT:
            {
                // 5 Saniyelik Non-Blocking (Kör) Bekleme Döngüsü
                // Bu esnada top konabilir veya konmayabilir. Arayüzden CMD_SCORE gelebilir.
                if (millis() - prepareTimer > 5000) {
                    Serial.println("Süre doldu, acımasız şut geliyor!");
                    currentState = STATE_SHOOT;
                }
            }
            break;
            
        case STATE_SHOOT:
            {
                Serial.println("Atis gerceklesiyor...");
                int dist = readToFDistance();
                int pwm = calculateShootingPower(dist);
                triggerSolenoid(pwm);
                Serial.println("Atis tamamlandi.");
                
                // Skor sayısına bak: 2 Kere basket olduysa 10cm kay. Yoksa bekle ve tekrar at.
                if (scoreCount >= 2) {
                    currentState = STATE_REPOSITION_10CM;
                } else {
                    // Top konmadıysa veya kaçırdıysak (scoreCount artmadıysa) aynı yerden atmaya devam
                    prepareTimer = millis();
                    currentState = STATE_PREPARE_SHOOT;
                }
            }
            break;
            
        case STATE_REPOSITION_10CM:
            {
                Serial.println("10 cm Repozisyon manevrasi...");
                // Tekerlek çapına ve hıza bağlı olarak süre kalibre edilmelidir.
                turnLeft(200);
                vTaskDelay(pdMS_TO_TICKS(300)); // Örn: 0.3 saniye sola kayma
                stopMotors();
                
                scoreCount = 0; // Skoru sıfırla
                prepareTimer = millis(); // Yeni bir döngü başlat
                currentState = STATE_PREPARE_SHOOT; // Yer değiştirdikten sonra atış yap.
            }
            break;
    }
}
