/**
 * ESP8266 WOL 中继代理
 * -----------------------
 * 接收 UDP 魔术包并广播到局域网，用于远程唤醒设备。
 * 支持通过串口动态配置 WiFi 凭据。
 *
 * 模块结构:
 * - config.h         全局配置宏
 * - serial_config    串口配置交互
 * - wifi_manager      WiFi 连接管理
 * - wol_manager       WOL 魔术包处理
 */

#include <Arduino.h>
#include "config.h"
#include "serial_config.h"
#include "wifi_manager.h"
#include "wol_manager.h"

void setup() {
    // 1. 初始化串口配置
    SerialWiFiConfig::begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println(F("=== ESP8266 WOL 中继代理 ==="));

    // 2. 等待用户通过串口配置 WiFi（超时则使用默认宏）
    SerialWiFiConfig::waitForConfig(SERIAL_CONFIG_TIMEOUT);

    // 3. 连接 WiFi
    const char* ssid     = SerialWiFiConfig::getSSID();
    const char* password = SerialWiFiConfig::getPassword();

    if (!WiFiManager::connect(ssid, password, WIFI_CONNECT_TIMEOUT)) {
        Serial.println(F("Wi-Fi 连接失败! 将在 5 秒后重启..."));
        delay(5000);
        ESP.restart();
    }

    // 4. 启动 WOL UDP 监听
    WolManager::begin(LOCAL_PORT);

    Serial.println(F("系统就绪，等待魔术包..."));
}

void loop() {
    // 处理 WOL 魔术包
    WolManager::handlePacket();
    delay(10);
}