#include "SensorManager.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
VL53L1X tofSensor;
Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


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
    delay(10); // Sensörün kapanması için bekle
    digitalWrite(TOF_XSHUT_PIN, HIGH);
    delay(10); // Sensörün açılması için bekle
    
    // ToF varsayılan adresinde (0x29) başlar. 
    // Renk sensörü de 0x29'da olduğu için önce ToF'un adresini değiştiriyoruz.
    tofSensor.setTimeout(500);
    if (!tofSensor.init()) {
        Serial.println("ToF sensor baslatilamadi!");
    } else {
        tofSensor.setAddress(TOF_NEW_ADDR);
        tofSensor.setDistanceMode(VL53L1X::Long);
        tofSensor.setMeasurementTimingBudget(50000); // 50ms okuma hızı
        tofSensor.startContinuous(50);
        Serial.println("ToF sensor baslatildi ve adresi degistirildi (0x30).");
    }
}

void initSensors() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    
    // 1. I2C Çakışmasını Çöz ve ToF'u Başlat
    resolveI2CConflict();
    
    // 2. Renk Sensörünü Başlat
    if (colorSensor.begin(TCS34725_ADDR, &Wire)) {
        Serial.println("Renk sensoru baslatildi (0x29).");
    } else {
        Serial.println("Renk sensoru bulunamadi!");
    }
    
    // 3. MPU6050 Başlat
    // (Zaten initMPU fonksiyonunda Wire.begin yapılıp başlatılıyor,
    // ancak yapı bütünlüğü için initMPU çağrılmadan önce I2C hazır.)
}

int readToFDistance() { 
    tofSensor.read();
    return tofSensor.ranging_data.range_mm; // Mesafe mm cinsinden
}

void readColorData(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) { 
    colorSensor.getRawData(&r, &g, &b, &c);
}

void readIMUData(float &yaw, float &pitch, float &roll) { /* TODO */ }
