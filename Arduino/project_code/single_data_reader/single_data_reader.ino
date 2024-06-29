#include "Wire.h"
#include "Adafruit_INA219.h"
#include "sd_read_write.h"
#include "SD_MMC.h"

#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.

// Define GPIO pins
#define DEVICE_SIGNAL_PIN1 42
#define DEVICE_SIGNAL_PIN2 41

Adafruit_INA219 ina219(0x40);


int signal1Received = 0;
int signal2Received = 0;

int initialMillis = 0;

// On falling edge of signal, signalReceived variables are set to 1
void IRAM_ATTR signal1ISR() {
  signal1Received = 1;
}
void IRAM_ATTR signal2ISR() {
  signal2Received = 1;
}


void setup() {
    Serial.begin(115200);
    
    pinMode(DEVICE_SIGNAL_PIN1, INPUT);
    pinMode(DEVICE_SIGNAL_PIN2, INPUT);

    SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
    if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
        Serial.println("Card Mount Failed");
        return;
    }
    
    uint8_t cardType = SD_MMC.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD_MMC card attached");
        return;
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
    Wire.begin(16, 17);
    listDir(SD_MMC, "/", 0);

    if (!ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
    }

    deleteFile(SD_MMC, "/data.csv"); // TEMPORARY
    writeFile(SD_MMC, "/data.csv", "Time, Bus Voltage, Shunt Voltage, Load Voltage, Current_mA, Power_mA, Signal1, Signal2 \n");

    attachInterrupt(digitalPinToInterrupt(DEVICE_SIGNAL_PIN1), signal1ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(DEVICE_SIGNAL_PIN2), signal2ISR, FALLING);
    initialMillis = millis();
}

void loop() {

  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  // Serial.print(power_mW);
  int curTime = millis() - initialMillis;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  String row = String(curTime) + ", " +
          String(busvoltage) + ", " + 
          String(shuntvoltage) + ", " + 
          String(loadvoltage) + ", " + 
          String(current_mA) + ", " + 
          String(power_mW) + ", " +
          String(signal1Received) + ", " +
          String(signal2Received) + "\n";

  const char* row_c_str = row.c_str();
  // Serial.println(row_c_str);
  appendFile(SD_MMC, "/data.csv", row_c_str);
  // Serial.println(digitalRead(DEVICE_SIGNAL_PIN));

  if (signal1Received == 1) {
    // Serial.println("Signal Received 1");
    if (digitalRead(DEVICE_SIGNAL_PIN1) == 1) {
      signal1Received = 0;
      Serial.println("Line 1 reset");
    }
  }

  if (signal2Received == 1) {
    // Serial.println("Signal Received 2");
    if (digitalRead(DEVICE_SIGNAL_PIN2) == 1) {
      signal2Received = 0;

      Serial.println("Line 2 reset");
    }
  }

  delay(5);
}
