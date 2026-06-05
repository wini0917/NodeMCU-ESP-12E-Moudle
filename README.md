# ESP8266 ESP-12E WOL 中继代理

基于 NodeMCU ESP-12E 的 Wake-on-LAN 中继代理，通过 UDP 接收魔术包并广播到局域网，实现远程唤醒设备。

## 功能特性

- **WOL 中继代理**：接收 UDP 魔术包并转发到局域网广播地址，唤醒目标设备
- **串口动态配置**：支持通过串口在启动时动态配置 WiFi 凭据，无需重新烧录固件
- **魔术包验证**：自动验证接收到的 UDP 数据包是否为标准 WOL 魔术包格式
- **超时回退机制**：串口配置超时时自动使用默认宏定义的 WiFi 凭据
- **WiFi 连接管理**：自动连接 WiFi，连接失败时自动重启

## 硬件要求

- NodeMCU ESP-12E / ESP8266 开发板
- USB 数据线（用于供电和串口通信）

## 软件环境

- [PlatformIO](https://platformio.org/) 开发环境
- Arduino Framework

## 项目结构

```
NodeMCU-ESP-12E-Moudle/
├── include/
│   └── config.h              # 全局配置宏（WiFi 默认凭据、端口、超时时间等）
├── src/
│   ├── main.cpp              # 主程序入口，初始化各模块并启动 WOL 监听
│   ├── serial_config.h/.cpp  # 串口 WiFi 配置交互模块
│   ├── wifi_manager.h/.cpp   # WiFi 连接管理模块
│   └── wol_manager.h/.cpp    # WOL 魔术包接收、验证与转发模块
├── platformio.ini            # PlatformIO 项目配置文件
└── README.md                 # 本文件
```

## 快速开始

### 1. 配置默认 WiFi 凭据

编辑 [include/config.h](include/config.h) 文件，修改默认的 WiFi 凭据：

```cpp
#define WIFI_DEFAULT_SSID     "YOUR_WIFI_SSID"
#define WIFI_DEFAULT_PASSWORD "YOUR_WIFI_PASSWORD"
```

### 2. 编译并烧录

使用 PlatformIO 编译并上传固件到 ESP8266：

```bash
pio run --target upload
```

### 3. 串口配置（可选）

上电后，在 5 秒超时时间内按任意键进入配置模式，可通过串口输入新的 WiFi SSID 和密码：

```
=== ESP8266 WOL 中继代理 ===
当前默认 SSID:     YOUR_WIFI_SSID
当前默认 Password: YOUR_WIFI_PASSWORD
--------------------------------------------
按任意键进入配置模式 (5 秒超时)...
```

### 4. 发送 WOL 魔术包

使用任意 WOL 工具或脚本，向 ESP8266 的 IP 地址发送标准 WOL 魔术包（UDP 端口 9），ESP8266 会自动验证并广播到局域网。

## 配置说明

| 宏定义 | 默认值 | 说明 |
|--------|--------|------|
| `WIFI_DEFAULT_SSID` | `"YOUR_WIFI_SSID"` | 默认 WiFi SSID |
| `WIFI_DEFAULT_PASSWORD` | `"YOUR_WIFI_PASSWORD"` | 默认 WiFi 密码 |
| `WOL_PORT` | `9` | WOL 标准端口 |
| `LOCAL_PORT` | `9` | ESP8266 本地监听端口 |
| `SERIAL_BAUDRATE` | `115200` | 串口波特率 |
| `SERIAL_CONFIG_TIMEOUT` | `5000` | 串口配置模式超时时间 (ms) |
| `WIFI_CONNECT_TIMEOUT` | `30000` | WiFi 连接超时时间 (ms) |

## 模块说明

### serial_config
通过串口与用户交互，在启动时提供可选的 WiFi 凭据配置。支持超时回退到默认宏定义。

### wifi_manager
封装 ESP8266WiFi 库，提供 WiFi 连接、状态检查、IP 和 MAC 地址获取等功能。

### wol_manager
- 启动 UDP 监听，接收 WOL 魔术包
- 验证魔术包格式（6 字节 0xFF 前缀 + 16 次目标 MAC 重复）
- 提取目标 MAC 地址并通过广播地址转发

## 技术细节

- **平台**：ESP8266 (NodeMCU v2)
- **框架**：Arduino
- **通信协议**：UDP
- **WOL 标准**：符合 Magic Packet 规范（102 字节：6×0xFF + 16×MAC）

## 使用场景

- 远程唤醒家中/办公室的电脑
- 作为 WOL 代理，将外网 WOL 请求转发到内网
- 辅助设备通过 WiFi 实现有线设备的远程唤醒

## 许可证

MIT License