#ifndef CONFIG_H
#define CONFIG_H

// ==================== 默认 Wi-Fi 配置宏 ====================
// 可通过串口在启动时覆盖这些默认值
#define WIFI_DEFAULT_SSID     "YOUR_WIFI_SSID"
#define WIFI_DEFAULT_PASSWORD "YOUR_WIFI_PASSWORD"

// ==================== WOL 配置 ====================
#define WOL_PORT              9       // WOL 标准端口
#define LOCAL_PORT            9       // ESP8266 监听端口

// ==================== 串口配置 ====================
#define SERIAL_BAUDRATE       115200  // 串口波特率
#define SERIAL_CONFIG_TIMEOUT 5000    // 配置模式超时时间 (ms)

// ==================== WiFi 连接配置 ====================
#define WIFI_CONNECT_TIMEOUT  30000   // WiFi 连接超时 (ms)

#endif