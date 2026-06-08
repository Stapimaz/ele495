# Otonom Basketbol Robotu (ESP32)

Bu proje, ESP32 (DevKit V1) temel alınarak geliştirilen ve PlatformIO üzerinde çalışan otonom bir basketbol robotu yazılım mimarisidir. FreeRTOS kullanılarak çoklu çekirdek (Core 0 ve Core 1) üzerinde sensör, motor kontrolü ve haberleşme görevleri dağıtılmıştır.

## 📌 Donanım ve Pin Bağlantıları

### 1. Güç Bağlantıları (ÖNEMLİ!)
*   **L298N (12V veya Yüksek Voltaj Girişi):** Sadece motorlara giden ana güç (Örn: 7.4V Li-Po Pil). GÜVENLİK İÇİN ARAYA ŞALTER KOYUN!
*   **ESP32 5V (veya VIN) -> L298N (5V):** L298N'nin mantık devresini beslemek için kullanılabilir (Eğer L298N'nin kendi 5V regülatör jumper'ı kapalıysa). Lütfen L298N'nin özelliklerine göre 5V pinini kontrol edin.
*   **GND:** Pilin GND'si, ESP32'nin GND'si ve L298N'nin GND'si **KESİNLİKLE BİRBİRİNE BAĞLI (ORTAK) OLMALIDIR**.

### 2. Motor Sürücü (L298N)
| Pin Adı | ESP32 Pini | Açıklama |
| :--- | :--- | :--- |
| **ENA** | D13 | Sol Motor Hız Kontrolü (PWM) |
| **IN1** | D14 | Sol Motor Yön 1 |
| **IN2** | D27 | Sol Motor Yön 2 |
| **ENB** | D25 | Sağ Motor Hız Kontrolü (PWM) |
| **IN3** | D26 | Sağ Motor Yön 1 |
| **IN4** | D33 | Sağ Motor Yön 2 |
*Sol Motor Çıkışları: OUT1 ve OUT2. Sağ Motor Çıkışları: OUT3 ve OUT4.*

### 3. I2C Sensörler (MPU6050, VL53L1X, TCS34725)
*   **MPU6050 (İvmeölçer & Jiroskop):**
    *   **VCC:** ESP32 3.3V
    *   **GND:** Ortak GND
    *   **SDA:** D21
    *   **SCL:** D22
*   **Diğer Sensörler:** VL53L1X XSHUT (D4) pini ile çakışmaları engellenir.

### 4. Ek Eyleyiciler
*   **Solenoid (Şut):** D32 (PWM veya Dijital Tetikleme)

---

## 🚀 Yazılım Yükleme Talimatları (Upload)

Proje klasik Arduino IDE yerine **PlatformIO** formatında hazırlanmıştır. Kodları yüklemek için:

1. **VS Code** üzerinde sol kısımdan uzaylı/karınca ikonuna (PlatformIO) tıklayın.
2. Açılan panelde **Project Tasks -> esp32doit-devkit-v1 -> General** yolunu izleyin.
3. Önce **Build** butonuna tıklayarak projenin sorunsuz derlendiğinden emin olun.
4. ESP32'yi bilgisayarınıza USB ile bağlayın.
5. **Upload** butonuna tıklayarak kodu karta yükleyin. (Alternatif olarak VS Code'un en altındaki mavi çubukta bulunan sağ ok `→` butonunu da kullanabilirsiniz).
6. *Not:* Terminalde `Connecting........___....` yazısını gördüğünüzde işlem duraklarsa, ESP32 üzerindeki **BOOT (veya IO0)** butonuna yükleme başlayana kadar birkaç saniye basılı tutmanız gerekebilir.
7. Kod yüklendikten sonra alttaki "Fiş" veya "Seri Port" (Serial Monitor) ikonuna tıklayıp **115200 baud** hızında ESP32'nin verdiği kalibrasyon ve test loglarını takip edebilirsiniz. MPU6050 kalibre edilirken robotu kıpırdatmayın!
