#include "SensorManager.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

void initMPU() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    
    byte status = mpu.begin();
    Serial.print("MPU6050 Durumu: ");
    Serial.println(status);
    
    // MPU6050'e ulaşılamazsa kod burada takılır. Hata alırsanız pinleri kontrol edin.
    while(status != 0){ } 
    
    Serial.println("MPU6050 Kalibre ediliyor, robotu KIPIRDATMAYIN...");
    delay(1000);
    mpu.calcOffsets(); 
    Serial.println("Kalibrasyon tamamlandı! Robot hareket etmeye hazir.");
}

void updateMPU() {
    mpu.update(); // Jiroskop verilerini her dongude tazelememiz sart
}

float getAngleZ() {
    return mpu.getAngleZ(); // Sadece donus acisini aliyoruz
}

void resolveI2CConflict() {
    // ToF sensörünü pasif duruma çekip I2C çakışmasını önle
    pinMode(TOF_XSHUT_PIN, OUTPUT);
    digitalWrite(TOF_XSHUT_PIN, LOW);
    // TODO: Kısa bir bekleme ve ToF'u yeniden başlatıp adresini TOF_NEW_ADDR olarak değiştir.
}

void initSensors() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    resolveI2CConflict();
    // TODO: Init VL53L1X, TCS34725, MPU6050
}

int readToFDistance() { return 0; /* TODO */ }
void readColorData(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) { /* TODO */ }
void readIMUData(float &yaw, float &pitch, float &roll) { /* TODO */ }
