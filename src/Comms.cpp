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
        
        if (command == UDP_START_COMMAND) {
            if (currentState == STATE_IDLE) {
                Serial.println("START komutu alindi! Otonomi moduna (GLOBAL_SCAN) geciliyor.");
                currentState = STATE_GLOBAL_SCAN;
            }
        }
    }
}

void sendTelemetryData() {
    // Ağı (Network) boğmamak için 100 milisaniyede bir (saniyede 10 kere) telemetri gönderelim
    if (millis() - lastTelemetryTime > 100) {
        lastTelemetryTime = millis();
        
        // .NET uygulamasının parse edebileceği basit bir JSON formatı hazırlayalım
        // İleride mesafe, renk vb veriler eklenebilir.
        char jsonMsg[128];
        snprintf(jsonMsg, sizeof(jsonMsg), "{\"state\":%d, \"status\":\"alive\"}", (int)currentState);
        
        // Broadcast adresi ESP32 SoftAP için standart 192.168.4.255'tir
        IPAddress broadcastIp(192, 168, 4, 255); 
        
        udp.beginPacket(broadcastIp, UDP_PORT);
        udp.print(jsonMsg);
        udp.endPacket();
    }
}
