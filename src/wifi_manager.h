#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

/**
 * WiFi 管理模块
 * 负责 WiFi 连接管理
 */
class WiFiManager {
public:
    /**
     * 连接到 WiFi
     * @param ssid      WiFi SSID
     * @param password  WiFi 密码
     * @param timeoutMs 连接超时时间 (ms)
     * @return true=连接成功, false=连接失败
     */
    static bool connect(const char* ssid, const char* password, uint32_t timeoutMs);

    /**
     * 检查 WiFi 是否已连接
     */
    static bool isConnected();

    /**
     * 获取本地 IP 地址
     */
    static String getLocalIP();

    /**
     * 获取 MAC 地址
     */
    static String getMacAddress();
};

#endif