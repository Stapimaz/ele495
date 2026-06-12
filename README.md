# ELE 495 Bitirme Tasarım Projesi - Otonom Basketbol Robotu (ESP32)

Bu depo, ELE 495 Bitirme Tasarım Projesi kapsamında geliştirilen otonom basketbol atış robotunun gömülü yazılım (C++ / FreeRTOS tabanlı) kodlarını barındırmaktadır. Sistem, tamamen otonom çalışmak üzere tasarlanmış olup sensör entegrasyonlarını, motor sürüş mantığını ve bir C# (veya başka bir UI) masaüstü uygulaması ile haberleşmek için UDP altyapısını içerir.

## 📌 Proje Özeti
Robot, belirli renk bölgelerine giderek şut çeken, atış aralarında 5 saniye bekleyen ve skora göre (2 kere aynı yerden basket atılırsa 10 cm kayarak) otonom davranan bir mantığa sahiptir. Bu mantık, topun elle konulması zorunluluğunu sensör kullanmadan çözmek üzere **"Zamanlayıcı Tabanlı Otonom Döngü" (Blind Timer Loop)** yaklaşımıyla geliştirilmiştir.

---

## 📡 Haberleşme ve Arayüz (UI) Spesifikasyonları
Arayüz (C# Desktop App) geliştirecek takım arkadaşı için sistemin ağ (Network) ve komut (UDP) altyapısı aşağıdaki gibidir:

### 1. Ağ Yapılandırması (WiFi AP)
ESP32 kendi WiFi ağını (Access Point) oluşturur. Arayüz programının çalıştığı bilgisayar önce bu WiFi ağına bağlanmalıdır.
- **SSID (Ağ Adı):** `ELE495_ROBOT`
- **Şifre:** `12345678`
- **ESP32 IP Adresi:** `192.168.4.1` (Genellikle)
- **UDP Portu:** `4242`

### 2. Arayüzün Göndereceği Komutlar (UDP TX)
Arayüz (Client), ESP32'nin `4242` portuna (IP: `192.168.4.1`) aşağıdaki String metinleri göndermelidir (Komutlar tam olarak bu Stringler olmalıdır, null terminator `\0` veya sadece metin):

| Komut (String) | Açıklama |
| --- | --- |
| `CMD_START_INNER` | Robotu IDLE (Bekleme) durumundan çıkarıp **Turkuaz (İç)** bölge için tam otonom şut döngüsünü başlatır. |
| `CMD_START_OUTER` | Robotu IDLE durumundan çıkarıp **Sarı (Dış)** bölge için tam otonom şut döngüsünü başlatır. |
| `CMD_SCORE_YES` | Robot atış yaptıktan sonraki bekleme döngüsündeyken, top basket olmuşsa UI tarafından gönderilir. Bu komut, robotun içindeki skor sayacını 1 artırır. 2 skorda robot otonom olarak 10 cm yana kayar. |

*Önemli Not: Robotun otonom kalması kuralı gereği, UI robota "Ateş Et" diyemez. Sadece döngüyü başlatır ve skor bilgisini verir.*

### 3. ESP32'nin Göndereceği Telemetri Verisi (UDP RX - Broadcast)
ESP32, bağlanan tüm cihazlara (`192.168.4.255` broadcast adresi üzerinden) **saniyede 10 kere** (100ms aralıklarla) anlık durumunu JSON formatında gönderir. Arayüz, UDP 4242 portunu dinleyerek robotun anlık durumunu ekrana yazdırabilir.

**Gönderilen JSON Formatı:**
```json
{"state":3, "status":"alive", "score":1}
```

**State Karşılıkları (Enum RobotState):**
- `0`: IDLE (Sistem bekliyor, komut almaya hazır)
- `1`: GLOBAL_SCAN (Pota aranıyor, merkez açı bulunuyor)
- `2`: DRIVE_TO_ZONE (Bulunan çizgiye sürülüyor)
- `3`: PREPARE_SHOOT (Bölgede çakılı, atış için 5 saniye bekliyor - topu bu arada koyun)
- `4`: SHOOT (Atış yapılıyor - tetik çekiliyor)
- `5`: REPOSITION_10CM (2. başarılı şut atıldığı için 10 cm kayılıyor)

---

## 🛠️ Donanım Altyapısı
1. **ESP32-WROOM-32 (30 Pin):** Çift çekirdek (Core 0: Network/UDP, Core 1: Otonomi/Sensörler)
2. **VL53L1X Lazer (ToF) Sensör:** Pota tarama ve atış gücü mesafe hesaplaması için.
3. **TCS34725 Renk Sensörü:** Turkuaz ve Sarı çizgileri tanımak için. *(ToF sensörü ile I2C 0x29 adres çakışmasını önlemek için sistem açılışında XSHUT pini ile donanımsal reset uygulanır).*
4. **MPU6050 Jiroskop:** Robotun çizgiye giderken ip gibi düz gitmesi (Açı koruması) için PID kontrollü.
5. **L298N Motor Sürücü:** İki tekerlekli (2WD) şase kontrolü (PWM ile Diferansiyel Sürüş).
6. **JF-0530B Solenoid:** Top fırlatma mekanizması (Mesafe bazlı değişken PWM ile).

---

## ⚙️ Kurulum ve Kalibrasyon
Kodu ilk kez yükledikten sonra, robotun ortama uyum sağlaması için `include/Config.h` dosyasında şu ayarlamaların yapılması gereklidir:
1. **Renk Sensörü Eşikleri:** Gerçek piste konulup Turkuaz/Sarı RGB değerleri ölçülmeli ve `StateMachine.cpp` içindeki if-else mantığına girilmelidir.
2. **ToF Mesafeleri:** Minimum ve maksimum atış mesafesine göre `PWM_MIN_STRENGTH` ve `PWM_MAX_STRENGTH` sınırları deneysel olarak ayarlanmalıdır.

---

## 🔌 Fiziksel Kurulum ve Pin Bağlantıları (Wiring Diagram)

Aşağıdaki bağlantılar, görselini ilettiğiniz 30 Pinli **ESP32-WROOM-32** (NodeMCU-32S) geliştirme kartına tam uyumlu olacak şekilde projenin kodlarındaki (`Config.h`) pin numaralarına göre hazırlanmıştır. Kart üzerindeki `D` harfli pinler direkt numarayı ifade eder (Örn: `D21` = GPIO 21).

### 1. Güç Yönetimi (Çok Önemli!)
Tüm sistemi tek bir 12V batarya (Örn: 3S Li-Po) ile besleyebilirsiniz. ESP32'yi bataryaya doğrudan **BAĞLAMAYIN**, gücü L298N üzerinden alacağız.

| Batarya / Kaynak | Hedef (L298N Sürücü) | ESP32 | Açıklama |
| :--- | :--- | :--- | :--- |
| **12V Batarya (+)** | L298N `12V` (VCC) Pini | - | Motorlara güç verir. |
| **12V Batarya (-)** | L298N `GND` Pini | ESP32 `GND` | **CRITICAL:** ESP32, L298N ve bataryanın GND pinleri MUTLAKA birleşmelidir (Ortak Toprak). |
| - | L298N `5V` Pini (Çıkış) | ESP32 `VIN` | L298N içindeki regülatör 12V'u 5V'a düşürüp ESP32'yi besler. |

### 2. I2C Sensör Veriyolu (ToF, Renk, MPU6050)
Üç sensör (VL53L1X, TCS34725, MPU6050) I2C protokolünü kullandığı için **hepsi paralel (aynı pinlere)** bağlanacaktır. Sensörler 3.3V ile çalışmalıdır.

| ESP32 Pini | Hedef Sensör Pini | Açıklama |
| :--- | :--- | :--- |
| `3V3` | Tüm Sensörlerin `VCC` / `VIN` Pini | Sensörlere güç beslemesi. |
| `GND` | Tüm Sensörlerin `GND` Pini | Ortak toprak. |
| `D21` | Tüm Sensörlerin `SDA` Pini | I2C Veri hattı. |
| `D22` | Tüm Sensörlerin `SCL` Pini | I2C Saat hattı. |
| `D4` | **Sadece VL53L1X** `XSHUT` Pini | Adres çakışmasını çözmek için zorunlu donanımsal reset pini. |

### 3. L298N Motor Sürücü Sinyal Bağlantıları
Tekerlekleri süren 2 adet DC motor için.

| ESP32 Pini | L298N Pini | Açıklama |
| :--- | :--- | :--- |
| `D13` | `ENA` | Sol Motor PWM (Hız kontrolü). Üzerindeki jumper çıkarılmalı. |
| `D14` | `IN1` | Sol Motor İleri Yön. |
| `D27` | `IN2` | Sol Motor Geri Yön. |
| `D25` | `ENB` | Sağ Motor PWM (Hız kontrolü). Üzerindeki jumper çıkarılmalı. |
| `D26` | `IN3` | Sağ Motor İleri Yön. |
| `D33` | `IN4` | Sağ Motor Geri Yön. |
| - | `OUT1` & `OUT2` | Sol DC Motor kutuplarına gider. |
| - | `OUT3` & `OUT4` | Sağ DC Motor kutuplarına gider. |

### 4. Solenoid Atış Mekanizması Sürücü Devresi
**UYARI:** Solenoid, ESP32'den veya L298N sinyalinden direkt beslenemez! Arada Solenoid'i sürecek bir MOSFET Modülü (Örn: IRF520 veya Logic-Level IRLZ44N) kullanılmalıdır. Atış şiddeti PWM ile ayarlandığı için Röle KULLANILAMAZ.

| ESP32 Pini | MOSFET Modülü | Solenoid / Güç |
| :--- | :--- | :--- |
| `D32` | MOSFET `SIG` / `Gate` | Atış şiddetini belirleyen PWM sinyali. |
| `GND` | MOSFET `GND` / `Source` | Ortak toprak bağlantısı. |
| - | MOSFET `V+` veya Vin | 12V Batarya (+)'sına gider. |
| - | MOSFET `V-` veya Drain | Solenoidin (-) ucuna gider. Solenoidin (+) ucu direkt 12V'a bağlanır. |

*(Önemli: Eğer çıplak MOSFET kullanıyorsanız, Solenoid uçlarına ters paralel bir "Flyback Diyot - Örn: 1N4007" bağlamanız devreyi yanmaktan korur. Hazır MOSFET modüllerinin çoğunda bu koruma vardır.)*
