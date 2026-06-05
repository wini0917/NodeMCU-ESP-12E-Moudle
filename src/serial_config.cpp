#include "serial_config.h"
#include "config.h"

String SerialWiFiConfig::_ssid     = WIFI_DEFAULT_SSID;
String SerialWiFiConfig::_password = WIFI_DEFAULT_PASSWORD;

void SerialWiFiConfig::begin(uint32_t baudrate) {
    Serial.begin(baudrate);
    // 等待串口就绪
    delay(100);
}

bool SerialWiFiConfig::waitForConfig(uint32_t timeoutMs) {
    Serial.println();
    Serial.println(F("============================================"));
    Serial.println(F("       ESP8266 WOL 中继代理 - 配置模式"));
    Serial.println(F("============================================"));
    Serial.print(F("当前默认 SSID:     "));
    Serial.println(WIFI_DEFAULT_SSID);
    Serial.print(F("当前默认 Password: "));
    Serial.println(WIFI_DEFAULT_PASSWORD);
    Serial.println(F("--------------------------------------------"));
    Serial.print(F("按任意键进入配置模式 ("));
    Serial.print(timeoutMs / 1000);
    Serial.println(F(" 秒超时)..."));
    Serial.println();

    // 等待用户按键或超时
    uint32_t startTime = millis();
    while (millis() - startTime < timeoutMs) {
        if (Serial.available() > 0) {
            // 清空残留输入
            while (Serial.available()) Serial.read();

            // 显示配置菜单
            printMenu();

            // 读取 SSID
            _ssid = readLine(30000);
            if (_ssid.length() > 0) {
                Serial.print(F("SSID 已设置为: "));
                Serial.println(_ssid);
            } else {
                _ssid = WIFI_DEFAULT_SSID;
                Serial.println(F("未输入，使用默认 SSID"));
            }

            // 读取密码
            _password = readLine(30000);
            if (_password.length() > 0) {
                Serial.print(F("Password 已设置为: "));
                Serial.println(_password);
            } else {
                _password = WIFI_DEFAULT_PASSWORD;
                Serial.println(F("未输入，使用默认 Password"));
            }

            Serial.println(F("============================================"));
            return true;
        }
        delay(50);
    }

    // 超时，使用默认凭据
    _ssid     = WIFI_DEFAULT_SSID;
    _password = WIFI_DEFAULT_PASSWORD;
    Serial.println(F("超时，使用默认凭据"));
    Serial.println(F("============================================"));
    return false;
}

const char* SerialWiFiConfig::getSSID() {
    return _ssid.c_str();
}

const char* SerialWiFiConfig::getPassword() {
    return _password.c_str();
}

String SerialWiFiConfig::readLine(uint32_t timeoutMs) {
    uint32_t startTime = millis();
    String line = "";

    while (millis() - startTime < timeoutMs) {
        while (Serial.available() > 0) {
            char c = Serial.read();
            if (c == '\n' || c == '\r') {
                // 去除末尾的 \r 或 \n
                if (line.length() > 0 && (line[line.length() - 1] == '\r')) {
                    line.remove(line.length() - 1);
                }
                // 去除首尾空白
                line.trim();
                return line;
            }
            line += c;
        }
        delay(10);
    }

    // 超时，返回已读取的内容
    line.trim();
    return line;
}

void SerialWiFiConfig::printMenu() {
    Serial.println();
    Serial.println(F("--- 配置 Wi-Fi 凭据 ---"));
    Serial.println(F("（直接回车使用默认值）"));
    Serial.println();
    Serial.print(F("请输入 SSID: "));
}