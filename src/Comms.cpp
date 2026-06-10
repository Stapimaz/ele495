#include "Comms.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "StateMachine.h" // currentState'e erişim için

WiFiUDP udp;
char packetBuffer[255]; // Gelen UDP paketini tutacak tampon bellek

// Son telemetri gönderim zamanını tutan değişken (Flood yapmamak için)
unsigned long lastTelemetryTime = 0;

void initComms() {
    Serial.println("WiFi Access Point baslatiliyor...");
    // Kendi ağını oluştur
    WiFi.softAP(AP_SSID, AP_PASS);
    
    IPAddress IP = WiFi.softAPIP(); // Genelde 192.168.4.1 olur
    Serial.print("AP Acildi! IP Adresi: ");
    Serial.println(IP);
    
    // UDP portunu dinlemeye başla
    udp.begin(UDP_PORT);
    Serial.printf("UDP Sunucusu %d portunda dinleniyor.\n", UDP_PORT);
}

void checkForIncomingCommands() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        int len = udp.read(packetBuffer, 255);
        if (len > 0) {
            packetBuffer[len] = 0; // Metnin sonunu belirle (Null terminator)
        }
        
        String command = String(packetBuffer);
        command.trim(); // Boşluk ve satır sonu karakterlerini temizle
        
        Serial.printf("UDP'den gelen komut: %s\n", command.c_str());
        
        // Sadece IDLE durumunda çalışmayı başlatabilir
        if (currentState == STATE_IDLE) {
            if (command == CMD_START_INNER) {
                Serial.println("DONGU BASLIYOR: Turkuaz bolge hedefleniyor.");
                currentTargetColor = COLOR_TURQUOISE;
                scoreCount = 0; // Skor sıfırla
                currentState = STATE_GLOBAL_SCAN;
            } 
            else if (command == CMD_START_OUTER) {
                Serial.println("DONGU BASLIYOR: Sari bolge hedefleniyor.");
                currentTargetColor = COLOR_YELLOW;
                scoreCount = 0; // Skor sıfırla
                currentState = STATE_GLOBAL_SCAN;
            }
        }
        
        // Atış beklenirken veya döngü içindeyken Arayüzden BASKET bilgisi gelebilir
        if (command == CMD_SCORE_YES) {
            Serial.println("ARAYUZDEN BASKET (SKOR) ONAYI ALINDI!");
            scoreCount++;
        }
    }
}

void sendTelemetryData() {
    // Ağı (Network) boğmamak için 100 milisaniyede bir (saniyede 10 kere) telemetri gönderelim
    if (millis() - lastTelemetryTime > 100) {
        lastTelemetryTime = millis();
        
        // .NET uygulamasının parse edebileceği basit bir JSON formatı hazırlayalım
        char jsonMsg[128];
        snprintf(jsonMsg, sizeof(jsonMsg), "{\"state\":%d, \"status\":\"alive\", \"score\":%d}", (int)currentState, scoreCount);
        
        // Broadcast adresi ESP32 SoftAP için standart 192.168.4.255'tir
        IPAddress broadcastIp(192, 168, 4, 255); 
        
        udp.beginPacket(broadcastIp, UDP_PORT);
        udp.print(jsonMsg);
        udp.endPacket();
    }
}
