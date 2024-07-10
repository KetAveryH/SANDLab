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

///////////////////////////////////////////////////////////////////////////
// Each of these represents an edge node performing inference. 
// Uncomment the edge nodes you will be monitoring with an INA219

#define NUM_NODES 6
#define NODE_1
#define NODE_2
// #define NODE_3
// #define NODE_4
// #define NODE_5
// #define NODE_6

///////////////////////////////////////////////////////////////////////////



// INA219's have a max of 4 distinguishable addresses per SDA and SCL pair (I2C line)
// When wiring up your scribe to the following nodes, set connect pins 47 and 21 to SDA and SCL lines respectively
#ifdef NODE_1
    #define SIGNAL_PIN_1 4
    #define INA_ADDRESS_1_1 0x40
    Adafruit_INA219 ina219_1_1(INA_ADDRESS_1_1);
    volatile int rise_1 = 0;
    volatile int fall_1 = 0;
    uint16_t riseMillis_1 = 0;
    uint16_t fallMillis_1 = 0;

    void IRAM_ATTR signal1ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
      // Check the state of pin 4
      if (digitalRead(4) == HIGH) {
          rise_1 = 1;
      } 
      if (digitalRead(4) == LOW) {
          fall_1 = 1;
      }
    }
#else
    // const char* signal_received_1 = "N/A";
#endif

#ifdef NODE_2
    #define SIGNAL_PIN_2 5
    #define INA_ADDRESS_1_2 0x41
    Adafruit_INA219 ina219_1_2(INA_ADDRESS_1_2);
    volatile int rise_2 = 0;
    volatile int fall_2 = 0;
    uint16_t riseMillis_2 = 0;
    uint16_t fallMillis_2 = 0;

    void IRAM_ATTR signal2ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
      // Check the state of pin 4
      if (digitalRead(4) == HIGH) {
          rise_2 = 1;
      } 
      if (digitalRead(4) == LOW) {
          fall_2 = 1;
      }
    }
#else
    // const char* signal_received_2 = "N/A"; TODO
#endif

#ifdef NODE_3
    #define SIGNAL_PIN_3 6
    #define INA_ADDRESS_1_3 0x44
    Adafruit_INA219 ina219_1_3(INA_ADDRESS_1_3);
    volatile int rise_3 = 0;
    volatile int fall_3 = 0;
    uint16_t riseMillis_3 = 0;
    uint16_t fallMillis_3 = 0;

    void IRAM_ATTR signal3ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
      // Check the state of pin 4
      if (digitalRead(4) == HIGH) {
          rise_3 = 1;
      } 
      if (digitalRead(4) == LOW) {
          fall_3 = 1;
      }
    }
#else
    // const char* signal_received_3 = "N/A";
#endif

#ifdef NODE_4
    #define SIGNAL_PIN_4 7
    #define INA_ADDRESS_1_4 0x45
    Adafruit_INA219 ina219_1_4(INA_ADDRESS_1_4);
    volatile int rise_4 = 0;
    volatile int fall_4 = 0;
    uint16_t riseMillis_4 = 0;
    uint16_t fallMillis_4 = 0;

    void IRAM_ATTR signal4ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
      // Check the state of pin 4
      if (digitalRead(4) == HIGH) {
          rise_4 = 1;
      } 
      if (digitalRead(4) == LOW) {
          fall_4 = 1;
      }
    }
#else
    // const char* signal_received_4 = "N/A";
#endif

// Use pins 42 and 41 for SDA and SCL lines respectively for devices below this point
#ifdef NODE_5
    #define SIGNAL_PIN_5 15
    #define INA_ADDRESS_2_5 0x40
    Adafruit_INA219 ina219_2_1(INA_ADDRESS_2_5);
    volatile int rise_5 = 0;
    volatile int fall_5 = 0;
    uint16_t riseMillis_5 = 0;
    uint16_t fallMillis_5 = 0;

    void IRAM_ATTR signal5ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
      // Check the state of pin 4
      if (digitalRead(4) == HIGH) {
          rise_5 = 1;
      } 
      if (digitalRead(4) == LOW) {
          fall_5 = 1;
      }
    }
#else
    // const char* signal_received_5 = "N/A";
#endif

#ifdef NODE_6
    #define SIGNAL_PIN_6 16
    #define INA_ADDRESS_2_6 0x41
    Adafruit_INA219 ina219_2_2(INA_ADDRESS_2_6);
    volatile int rise_6 = 0;
    volatile int fall_6 = 0;
    uint16_t riseMillis_6 = 0;
    uint16_t fallMillis_6 = 0;

    void IRAM_ATTR signal6ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
      // Check the state of pin 4
      if (digitalRead(4) == HIGH) {
          rise_6 = 1;
      } 
      if (digitalRead(4) == LOW) {
          fall_6 = 1;
      }
    }
#else
    const char* signal_received_6 = "N/A";
#endif

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

  shuntVoltage = ina219_1.getShuntVoltage_mV();
  busVoltage = ina219_1.getBusVoltage_V();
  loadVoltage = busVoltage + (shuntVoltage / 1000);
  current = ina219_1.getCurrent_mA();
  power = ina219_1.getPower_mW();

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