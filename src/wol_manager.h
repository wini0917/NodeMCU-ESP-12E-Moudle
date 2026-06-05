#ifndef WOL_MANAGER_H
#define WOL_MANAGER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

/**
 * Wake-on-LAN 管理模块
 * 负责解析魔术包并转发
 */
class WolManager {
public:
    /**
     * 初始化 WOL 功能，启动 UDP 监听
     * @param localPort 本地监听端口
     * @return true=启动成功, false=启动失败
     */
    static bool begin(uint16_t localPort);

    /**
     * 处理传入的 UDP 数据包
     * 在 loop() 中定期调用
     */
    static void handlePacket();

    /**
     * 发送魔术包到指定 MAC 和 IP
     * @param mac      目标设备 MAC 地址 (6 字节数组)
     * @param targetIP 目标 IP 地址 (通常为广播地址)
     * @param port     目标端口 (默认 WOL 端口 9)
     */
    static void sendMagicPacket(const uint8_t* mac, IPAddress targetIP, uint16_t port);

private:
    static WiFiUDP _udp;

    /**
     * 验证数据包是否为有效的魔术包
     * @param buffer 数据缓冲区
     * @param size   数据大小
     * @param outMac 输出: 提取到的目标 MAC 地址 (6 字节)
     * @return true=有效魔术包, false=无效
     */
    static bool validateMagicPacket(const uint8_t* buffer, int size, uint8_t* outMac);

    /**
     * 以 HEX 格式打印数据缓冲区
     */
    static void printHex(const uint8_t* buffer, int size);

    /**
     * 以 MAC 格式打印 6 字节地址
     */
    static void printMac(const uint8_t* mac);
};

#endif