#include "wol_manager.h"
#include "config.h"

WiFiUDP WolManager::_udp;

bool WolManager::begin(uint16_t localPort) {
    if (_udp.begin(localPort)) {
        Serial.print(F("UDP 监听端口: "));
        Serial.println(localPort);
        return true;
    } else {
        Serial.println(F("UDP 启动失败!"));
        return false;
    }
}

void WolManager::handlePacket() {
    int packetSize = _udp.parsePacket();
    if (packetSize <= 0) return;

    Serial.print(F("收到 UDP 包, 大小: "));
    Serial.print(packetSize);
    Serial.print(F(" 字节, 来自: "));
    Serial.print(_udp.remoteIP());
    Serial.print(F(":"));
    Serial.println(_udp.remotePort());

    // 读取数据包内容
    uint8_t packetBuffer[packetSize];
    _udp.read(packetBuffer, packetSize);

    // 打印收到的数据（HEX 格式）
    Serial.print(F("数据 (HEX): "));
    printHex(packetBuffer, packetSize);

    // 验证魔术包
    uint8_t targetMAC[6];
    if (validateMagicPacket(packetBuffer, packetSize, targetMAC)) {
        Serial.print(F("有效魔术包! 目标 MAC: "));
        printMac(targetMAC);

        // 通过广播地址发送魔术包
        IPAddress broadcastIP(255, 255, 255, 255);
        sendMagicPacket(targetMAC, broadcastIP, WOL_PORT);
        Serial.println(F("魔术包已通过广播发送!"));
    }
}

void WolManager::sendMagicPacket(const uint8_t* mac, IPAddress targetIP, uint16_t port) {
    uint8_t packet[102];

    // 填充 6 字节 0xFF
    for (int i = 0; i < 6; i++) {
        packet[i] = 0xFF;
    }

    // 填充 16 次目标 MAC 地址
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 6; j++) {
            packet[6 + i * 6 + j] = mac[j];
        }
    }

    // 发送魔术包
    _udp.beginPacket(targetIP, port);
    _udp.write(packet, 102);
    if (_udp.endPacket()) {
        Serial.print(F("魔术包发送成功 -> "));
        Serial.print(targetIP);
        Serial.print(F(":"));
        Serial.println(port);
    } else {
        Serial.println(F("魔术包发送失败!"));
    }
}

bool WolManager::validateMagicPacket(const uint8_t* buffer, int size, uint8_t* outMac) {
    // 魔术包标准格式: 6 字节 0xFF + 16 次目标 MAC = 至少 102 字节
    if (size < 102) {
        Serial.println(F("数据包太小，不是有效的魔术包"));
        return false;
    }

    // 检查前 6 字节是否全为 0xFF
    for (int i = 0; i < 6; i++) {
        if (buffer[i] != 0xFF) {
            Serial.println(F("魔术包格式无效 (缺少 0xFF 前缀)"));
            return false;
        }
    }

    // 提取目标 MAC 地址（第 7-12 字节）
    for (int i = 0; i < 6; i++) {
        outMac[i] = buffer[6 + i];
    }

    // 验证 16 次重复
    for (int rep = 1; rep < 16; rep++) {
        for (int i = 0; i < 6; i++) {
            if (buffer[6 + rep * 6 + i] != outMac[i]) {
                Serial.println(F("魔术包格式无效 (MAC 重复校验失败)"));
                return false;
            }
        }
    }

    return true;
}

void WolManager::printHex(const uint8_t* buffer, int size) {
    for (int i = 0; i < size; i++) {
        if (buffer[i] < 0x10) Serial.print('0');
        Serial.print(buffer[i], HEX);
        Serial.print(' ');
    }
    Serial.println();
}

void WolManager::printMac(const uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 0x10) Serial.print('0');
        Serial.print(mac[i], HEX);
        if (i < 5) Serial.print(':');
    }
    Serial.println();
}