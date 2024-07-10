#ifndef DEVICE_READER_H
#define DEVICE_READER_H

#include "Arduino.h"
#include "Adafruit_INA219.h"

class DeviceReader {
public:
    DeviceReader(int signalPin, uint8_t i2cAddress);
    void begin(uint16_t initialTime);
    void saveData();
    void readData();
    const char* getData();
    static void handlePinChangeWrapper();
    volatile int rise;
    volatile int fall;


private:
    String data;
    int signalPin;
    uint8_t i2cAddress;
    volatile uint16_t riseMillis;
    volatile uint16_t fallMillis;
    uint16_t initialTime;
    int rise_first;
    Adafruit_INA219 ina219;
    static DeviceReader* instance;
    void handlePinChange();

    float shuntVoltage = 0;
    float busVoltage = 0;
    float loadVoltage = 0;
    float current = 0;
    float power = 0;
    uint16_t currTime = 0;
};

#endif // DEVICE_READER_H
