#include <BLEScan.h>

#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "12345678-1234-5678-1234-56789abcdef1"

BLEClient *pClient;
BLERemoteCharacteristic *pRemoteCharacteristic;


typedef struct mmWave_Data{
    
}mmWave_Data;

void setup() {
    Serial.begin(115200);
    BLEDevice::init("ESP32_Central");
    pClient = BLEDevice::createClient();
}

void loop() {
    // 扫描并连接到外围设备
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->start(5);

    // 假设找到目标设备，连接
    pClient->connect("Peripheral_MAC_ADDRESS"); // 替换为外围设备的 MAC 地址

    // 查找服务和特征
    pRemoteCharacteristic = pClient->getService(SERVICE_UUID)->getCharacteristic(CHARACTERISTIC_UUID);
    
    // 读取消息
    while (pClient->isConnected()) {
        if (pRemoteCharacteristic->canNotify()) {
            String value = pRemoteCharacteristic->getValue().c_str();
            Serial.println("Received message: " + value);
        }
        delay(1000);
    }
}