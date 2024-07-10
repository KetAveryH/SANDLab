#include "device_reader.h"

DeviceReader* DeviceReader::instance = nullptr;

DeviceReader::DeviceReader(int signalPin, uint8_t i2cAddress)
    : signalPin(signalPin), i2cAddress(i2cAddress), ina219(i2cAddress), rise(0), fall(0), riseMillis(0), fallMillis(0), initialTime(0), rise_first(2) {}

void DeviceReader::begin(uint16_t initialTime) {
    instance = this;  // Set the instance pointer to this object
    pinMode(signalPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(signalPin), handlePinChangeWrapper, CHANGE);
    ina219.begin();
    data = "Time, rise, fall, Power_mW, Load Voltage, Current_mA, message_latency\n";
    initialTime = initialTime;
}

void DeviceReader::handlePinChangeWrapper() {
    if (instance) {
        instance->handlePinChange();
    }
}

void DeviceReader::handlePinChange() {
    Serial.println(signalPin);
    if (digitalRead(signalPin) == HIGH) {
        rise = 1;
        riseMillis = millis();
    } else if (digitalRead(signalPin) == LOW) {
        fall = 1;
        fallMillis = millis();
    }
}

void DeviceReader::readData() {
    currTime = millis() - initialTime;
    shuntVoltage = ina219.getShuntVoltage_mV();
    busVoltage = ina219.getBusVoltage_V();
    loadVoltage = busVoltage + (shuntVoltage / 1000);
    current = ina219.getCurrent_mA();
    power = ina219.getPower_mW();
}   

void DeviceReader::saveData() {
    data += String(currTime) + ", ";
    
    if (rise == 1) {
        data += String(rise) + ", ";
        if (fall == 0) {
            rise_first = 1;
        }
        Serial.println("Rising edge detected on pin " + String(signalPin) + " " + String(currTime));
        rise = 0;
    } else {
        data += "0, ";
    }

    if (fall == 1) {
        data += String(fall) + ", ";
        if (rise == 0) {
            rise_first = 0;
        }
        Serial.println("Falling edge detected on pin " + String(signalPin) + " " + String(currTime));
        fall = 0;
    } else {
        data += "0, ";
    }

    uint16_t message_latency = (rise_first == 0) ? (fallMillis - riseMillis) : (rise_first == 1) ? (riseMillis - fallMillis) : 0;
    rise_first = 2;

    data += String(loadVoltage) + ", " +
            String(current) + ", " +
            String(power) + ", " +
            String(message_latency) + "\n";

    // Serial.println(data);
}

  const char* DeviceReader::getData() {
    // Serial.println(data.c_str());
    return data.c_str();
}
