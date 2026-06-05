#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

#include <Arduino.h>

/**
 * 串口 WiFi 配置模块
 * 负责通过串口与用户交互，获取 WiFi SSID 和密码
 */
class SerialWiFiConfig {
public:
    /**
     * 初始化串口
     * @param baudrate 波特率
     */
    static void begin(uint32_t baudrate);

    /**
     * 进入配置模式：在指定超时时间内等待用户通过串口输入 WiFi 凭据。
     * 如果超时未输入，使用默认宏定义的凭据。
     * @param timeoutMs 等待用户输入的超时时间 (ms)
     * @return true=用户输入了凭据, false=使用了默认凭据
     */
    static bool waitForConfig(uint32_t timeoutMs);

    /**
     * 获取配置后的 SSID
     */
    static const char* getSSID();

    /**
     * 获取配置后的密码
     */
    static const char* getPassword();

private:
    static String _ssid;
    static String _password;

    /**
     * 从串口读取一行（以 \n 结尾），去除首尾空白。
     * @param timeoutMs 单次读取超时，超时返回空字符串
     */
    static String readLine(uint32_t timeoutMs);

    /**
     * 打印配置菜单
     */
    static void printMenu();
};

#endif
