# Otonom Basketbol Robotu (ESP32)

Bu proje, ESP32 (DevKit V1) temel alınarak geliştirilen ve PlatformIO üzerinde çalışan otonom bir basketbol robotu yazılım mimarisidir. FreeRTOS kullanılarak çoklu çekirdek (Core 0 ve Core 1) üzerinde sensör, motor kontrolü ve haberleşme görevleri dağıtılmıştır.

## 📌 Donanım ve Pin Bağlantıları

### 1. Güç Bağlantıları (ÖNEMLİ!)
*   **TB6612FNG (VM):** Sadece motorlara giden ana güç (Örn: 7.4V Li-Po Pil). GÜVENLİK İÇİN ARAYA ŞALTER KOYUN!
*   **ESP32 3.3V -> TB6612FNG (VCC):** Sürücünün mantık devresini besler.
*   **ESP32 3.3V -> TB6612FNG (STBY):** Motor sürücüyü aktif hale getirir (Bağlanmazsa motorlar dönmez).
*   **GND:** Pilin GND'si, ESP32'nin GND'si ve TB6612FNG'nin GND'si **KESİNLİKLE BİRBİRİNE BAĞLI (ORTAK) OLMALIDIR**.

### 2. Motor Sürücü (TB6612FNG)
| Pin Adı | ESP32 Pini | Açıklama |
| :--- | :--- | :--- |
| **PWMA** | D13 | Sol Motor Hız Kontrolü (PWM) |
| **AIN1** | D14 | Sol Motor Yön 1 |
| **AIN2** | D27 | Sol Motor Yön 2 |
| **PWMB** | D25 | Sağ Motor Hız Kontrolü (PWM) |
| **BIN1** | D26 | Sağ Motor Yön 1 |
| **BIN2** | D33 | Sağ Motor Yön 2 |
*Sol Motor Çıkışları: AO1 ve AO2. Sağ Motor Çıkışları: BO1 ve BO2.*

### 3. I2C Sensörler (MPU6050, VL53L1X, TCS34725)
*   **SDA:** D21
*   **SCL:** D22
*   **XSHUT (VL53L1X):** D4 (I2C Adres çakışmasını engellemek için)

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
