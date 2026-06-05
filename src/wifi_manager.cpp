#include "wifi_manager.h"

bool WiFiManager::connect(const char* ssid, const char* password, uint32_t timeoutMs) {
    Serial.print(F("连接 Wi-Fi: "));
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    uint32_t startTime = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeoutMs) {
        delay(500);
        Serial.print('.');
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println(F("Wi-Fi 已连接!"));
        Serial.print(F("IP 地址: "));
        Serial.println(WiFi.localIP());
        Serial.print(F("MAC 地址: "));
        Serial.println(WiFi.macAddress());
        return true;
    } else {
        Serial.println();
        Serial.println(F("Wi-Fi 连接失败!"));
        return false;
    }
}

bool WiFiManager::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

String WiFiManager::getLocalIP() {
    return WiFi.localIP().toString();
}

String WiFiManager::getMacAddress() {
    return WiFi.macAddress();
}