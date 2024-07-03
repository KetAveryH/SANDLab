/**
 * @file simplified_scribe.ino
 * @brief This file allows you to read energy and event data from multiple ESP32s3's with millisecond precision.
 * 
 * For wiring reference take a look at the drive under "TinyML Summer '24/Test Setup". 
 * This program assumes a linear path in communication between edge devices.
 * 
 * Author: Ket Hollingsworth
 * Date: 7/2/2024
 */

#include "Wire.h"
#include "Adafruit_INA219.h"
#include "sd_read_write.h"
#include "SD_MMC.h"
#include <Adafruit_NeoPixel.h> 

#define LED_PIN 48

#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.

#define SIGNAL_PIN_1 4
#define INA_ADDRESS_1_1 0x40
Adafruit_INA219 ina219(INA_ADDRESS_1_1);

#define TOUCH_THRESH 50000

Adafruit_NeoPixel pixels(1, LED_PIN, NEO_GRB + NEO_KHZ800);




volatile int rise = 0;
volatile int fall = 0;
uint16_t riseMillis = 0;
uint16_t fallMillis = 0;

// ISR for handling pin 4 changes
void IRAM_ATTR handlePinChange() {
    // Check the state of pin 4
    if (digitalRead(4) == HIGH) {
        rise = 1;
    } 
    if (digitalRead(4) == LOW) {
        fall = 1;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(4, INPUT); // Set pin 4 as input
    

        
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


    // Attach interrupt to pin 4 for both rising and falling edges
    attachInterrupt(digitalPinToInterrupt(4), handlePinChange, CHANGE);
}

float shuntVoltage = 0;
float busVoltage = 0;
float loadVoltage = 0;
float current = 0;
float power = 0;

String data = "Time, rise, fall, Power_mW, Load Voltage, Current_mA, message_latency\n";
uint16_t initialTime = millis();
uint16_t message_latency = 0;
int rise_first = 2;
int begin = 0;
void loop() {
  // Serial.println(data);
  while (touchRead(T3) < TOUCH_THRESH && !begin) {
    delay(250);
    begin = 1;
  }
  uint16_t currTime = millis() - initialTime;
  data += String(currTime) + ", ";

  // Check if the rose variable has been set
  if (rise == 1) {
    
    riseMillis = millis();
    
    // noInterrupts(); // Disable interrupts while processing
    // Serial.println(String(rise));
    data += String(rise) + ", ";
    if (fall == 0) {
        rise_first = 1;
    } 
    Serial.println("Rising edge detected on pin 4");
    rise = 0;
    // interrupts(); // Re-enable interrupts
  } else {
    // noInterrupts();
    // Serial.println(String(rise));
    data += String(rise) + ", ";
    // interrupts();
  }

  // Check if the fall variable has been set
  if (fall == 1) {
    
    fallMillis = millis();
    // noInterrupts(); // Disable interrupts while processing
    // Serial.println(String(fall));
    data += String(fall) + ", ";
    if (rise == 0) {
        rise_first = 0;
    }
    Serial.println("Falling edge detected on pin 4");
    fall = 0; // Reset the variable

    // interrupts(); // Re-enable interrupts
  } else {
    // noInterrupts();
    // Serial.println(String(fall));
    data += String(fall) + ", ";
    // interrupts();
  }

  shuntVoltage = ina219.getShuntVoltage_mV();
  busVoltage = ina219.getBusVoltage_V();
  loadVoltage = busVoltage + (shuntVoltage / 1000);
  current = ina219.getCurrent_mA();
  power = ina219.getPower_mW();

  if (rise_first == 0) {
      message_latency = fallMillis - riseMillis;
      rise_first = 2;
  } else if (rise_first == 1) {
      message_latency = riseMillis - fallMillis;
      rise_first = 2;
  } else {
      message_latency = 0;
  }
  
  data += String(loadVoltage) + ", " +
          String(current) + ", " +
          String(power) + ", " +
          String(message_latency) + "\n";


  if (touchRead(T14) > TOUCH_THRESH) {
    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(0, 0, 30));
    pixels.show();

    const char* data_c_str = data.c_str();
    writeFile(SD_MMC, "/data.csv", data_c_str); // limitation is the number of appends to file, I instead will just append to a string that will then be appended every 10 ms
    delay(10000);
    pixels.clear();
  }

  
  delay(10); // Small delay to reduce the number of prints
}