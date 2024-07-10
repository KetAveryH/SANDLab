#include "device_reader.h"
#include "Wire.h"
#include "sd_read_write.h"
#include "SD_MMC.h"
#include <Adafruit_NeoPixel.h> 

#define LED_PIN 48
#define SD_MMC_CMD 38
#define SD_MMC_CLK 39
#define SD_MMC_D0  40
#define TOUCH_THRESH 50000

Adafruit_NeoPixel pixels(1, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define the signal pins and I2C addresses for the five devices
#define SIGNAL_PIN_1 4
#define INA_ADDRESS_1_1 0x40

#define SIGNAL_PIN_2 5
#define INA_ADDRESS_1_2 0x41

#define SIGNAL_PIN_3 6
#define INA_ADDRESS_1_3 0x44

#define SIGNAL_PIN_4 7
#define INA_ADDRESS_1_4 0x45

#define SIGNAL_PIN_5 15
#define INA_ADDRESS_2_5 0x40

// Initialize the device readers  
DeviceReader reader1(SIGNAL_PIN_1, INA_ADDRESS_1_1);
void IRAM_ATTR handlePinChange_1() {
    // Check the state of pin 4
    if (digitalRead(SIGNAL_PIN_1) == HIGH) {
        reader1.rise = 1;
    } 
    if (digitalRead(SIGNAL_PIN_1) == LOW) {
        reader1.fall = 1;
    }
}

DeviceReader reader2(SIGNAL_PIN_2, INA_ADDRESS_1_2);
void IRAM_ATTR handlePinChange_2() {
    // Check the state of pin 4
    if (digitalRead(SIGNAL_PIN_2) == HIGH) {
        reader2.rise = 1;
    } 
    if (digitalRead(SIGNAL_PIN_2) == LOW) {
        reader2.fall = 1;
    }
}
DeviceReader reader3(SIGNAL_PIN_3, INA_ADDRESS_1_3);
void IRAM_ATTR handlePinChange_3() {
    // Check the state of pin 4
    if (digitalRead(SIGNAL_PIN_3) == HIGH) {
        reader3.rise = 1;
    } 
    if (digitalRead(SIGNAL_PIN_3) == LOW) {
        reader3.fall = 1;
    }
}
DeviceReader reader4(SIGNAL_PIN_4, INA_ADDRESS_1_4);
void IRAM_ATTR handlePinChange_4() {
    // Check the state of pin 4
    if (digitalRead(SIGNAL_PIN_4) == HIGH) {
        reader4.rise = 1;
    } 
    if (digitalRead(SIGNAL_PIN_4) == LOW) {
        reader4.fall = 1;
    }
}

DeviceReader reader5(SIGNAL_PIN_5, INA_ADDRESS_2_5);
void IRAM_ATTR handlePinChange_5() {
    // Check the state of pin 4
    if (digitalRead(SIGNAL_PIN_5) == HIGH) {
        reader5.rise = 1;
    } 
    if (digitalRead(SIGNAL_PIN_5) == LOW) {
        reader5.fall = 1;
    }
}



void setup() {
    Serial.begin(115200);

    // Initialize SD CARD
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
    listDir(SD_MMC, "/", 0);
    uint16_t initialTime = millis();
    // Initialize the device readers
    reader1.begin(initialTime);
    reader2.begin(initialTime);
    reader3.begin(initialTime);
    reader4.begin(initialTime);
    reader5.begin(initialTime);

    attachInterrupt(digitalPinToInterrupt(4), handlePinChange_1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(5), handlePinChange_2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(6), handlePinChange_3, CHANGE);
    attachInterrupt(digitalPinToInterrupt(7), handlePinChange_4, CHANGE);
    attachInterrupt(digitalPinToInterrupt(15), handlePinChange_5, CHANGE);
}

void loop() {
    reader1.readData();
    reader2.readData();
    reader3.readData();
    reader4.readData();
    reader5.readData();

    reader1.saveData();
    reader2.saveData();
    reader3.saveData();
    reader4.saveData();
    reader5.saveData();

    if (touchRead(T14) > TOUCH_THRESH) {

        writeFile(SD_MMC, "/data1.csv", reader1.getData());
        delay(250);
        writeFile(SD_MMC, "/data2.csv", reader2.getData());
        delay(250);
        writeFile(SD_MMC, "/data3.csv", reader3.getData());
        delay(250);
        writeFile(SD_MMC, "/data4.csv", reader4.getData());
        delay(250);
        writeFile(SD_MMC, "/data5.csv", reader5.getData());

        pixels.begin();
        pixels.setPixelColor(0, pixels.Color(0, 0, 30));
        pixels.show();

        delay(10000);
        pixels.clear();
    }

    delay(10);
}
