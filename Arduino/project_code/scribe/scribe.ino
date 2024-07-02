/**
 * @file scribe.ino
 * @brief This file allows you to read energy and event data from multiple ESP32s3's with millisecond precision.
 * 
 * For wiring reference take a look at the drive under "TinyML Summer '24/Test Setup". 
 * This program assumes a linear path in communication between edge devices.
 * 
 * Author: Ket Hollingsworth
 * Date: 7/1/2024
 */

#include "Wire.h"
#include "Adafruit_INA219.h"
#include "sd_read_write.h"
#include "SD_MMC.h"

#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.



// Each of these represents an edge node performing inference. 
// Uncomment the edge nodes you will be using
#define NUM_NODES 6
#define NODE_1
#define NODE_2
// #define NODE_3
// #define NODE_4
// #define NODE_5
// #define NODE_6

int initialMillis = 0;
uint16_t refMillis = 0;
int millisDif = 0;


// INA219's have a max of 4 distinguishable addresses per SDA and SCL pair (I2C line)
// When wiring up your scribe to the following nodes, set connect pins 47 and 21 to SDA and SCL lines respectively
#ifdef NODE_1
    #define SIGNAL_PIN_1 4
    #define INA_ADDRESS_1_1 0x40
    Adafruit_INA219 ina219_1_1(INA_ADDRESS_1_1);
    int signal_received_1 = 0;
    void IRAM_ATTR signal1ISR() {  // ISR function ensures that on falling edge of the signal pin, the data is recorded
        signal_received_1 = 1;

        // Return the difference in milliseconds
        if (refMillis > 0) {
            millisDif = millis() - refMillis;
            refMillis = 0;
        } else { 
            refMillis = millis(); // If signal is for sending record start time
        }
    }
#else
    const char* signal_received_1 = "N/A";
#endif

#ifdef NODE_2
    #define SIGNAL_PIN_2 5
    #define INA_ADDRESS_1_2 0x41
    Adafruit_INA219 ina219_1_2(INA_ADDRESS_1_2);
    int signal_received_2 = 0;

    void IRAM_ATTR signal2ISR() {
        signal_received_2 = 1;

        // Return the difference in milliseconds
        if (refMillis > 0) {
            millisDif = millis() - refMillis;
            refMillis = 0;
        } else { 
            refMillis = millis(); // If signal is for sending record start time
        }
    }
#else
    const char* signal_received_2 = "N/A";
#endif

#ifdef NODE_3
    #define SIGNAL_PIN_3 6
    #define INA_ADDRESS_1_3 0x44
    Adafruit_INA219 ina219_1_3(INA_ADDRESS_1_3);
    int signal_received_3 = 0;
    void IRAM_ATTR signal3ISR() {
        signal_received_3 = 1;

        // Return the difference in milliseconds
        if (refMillis > 0) {
            millisDif = millis() - refMillis;
            refMillis = 0;
        } else { 
            refMillis = millis(); // If signal is for sending record start time
        }
    }
#else
    const char* signal_received_3 = "N/A";
#endif

#ifdef NODE_4
    #define SIGNAL_PIN_4 7
    #define INA_ADDRESS_1_4 0x45
    Adafruit_INA219 ina219_1_4(INA_ADDRESS_1_4);
    int signal_received_4 = 0;
    void IRAM_ATTR signal4ISR() {
        signal_received_4 = 1;

        // Return the difference in milliseconds
        if (refMillis > 0) {
            millisDif = millis() - refMillis;
            refMillis = 0;
        } else { 
            refMillis = millis(); // If signal is for sending record start time
        }
    }
#else
    const char* signal_received_4 = "N/A";
#endif

// Use pins 42 and 41 for SDA and SCL lines respectively for devices below this point
#ifdef NODE_5
    #define SIGNAL_PIN_5 15
    #define INA_ADDRESS_2_5 0x40
    Adafruit_INA219 ina219_2_5(INA_ADDRESS_2_5);
    int signal_received_5 = 0;
    void IRAM_ATTR signal5ISR() {
        signal_received_5 = 1;

        // Return the difference in milliseconds
        if (refMillis > 0) {
            millisDif = millis() - refMillis;
            refMillis = 0;
        } else { 
            refMillis = millis(); // If signal is for sending record start time
        }
    }
#else
    const char* signal_received_5 = "N/A";
#endif

#ifdef NODE_6
    #define SIGNAL_PIN_6 16
    #define INA_ADDRESS_2_6 0x41
    Adafruit_INA219 ina219_2_6(INA_ADDRESS_2_6);
    int signal_received_6 = 0;
    void IRAM_ATTR signal6ISR() {
        signal_received_6 = 1;

        // Return the difference in milliseconds
        if (refMillis > 0) {
            millisDif = millis() - refMillis;
            refMillis = 0;
        } else { 
            refMillis = millis(); // If signal is for sending record start time
        }
    }
#else
    const char* signal_received_6 = "N/A";
#endif

/**
 * @brief Resets the signal received variables if the corresponding signal line is high.
 *
 * This function checks the state of signal received variables and their corresponding
 * signal pins. If a signal has been received (indicated by the signal_received_* variable being 1)
 * and the corresponding signal pin is high, the function resets the signal_received_* variable
 * to 0. 
 *
 * This helps us more accurately pinpoint communication events by quickly returning to 0 after a communication event.
 */
void reset_signal_variables() {
    #ifdef NODE_1
    if (signal_received_1 == 1 && digitalRead(SIGNAL_PIN_1) == 1) {
        signal_received_1 = 0;
        initialMillis = 0;
        Serial.println("Line 1 reset");
    }
    #endif

    #ifdef NODE_2
    if (signal_received_2 == 1 && digitalRead(SIGNAL_PIN_2) == 1) {
        signal_received_2 = 0;
        Serial.println("Line 2 reset");
    }
    #endif

    #ifdef NODE_3
    if (signal_received_3 == 1 && digitalRead(SIGNAL_PIN_3) == 1) {
        signal_received_3 = 0;
        Serial.println("Line 3 reset");
    }
    #endif

    #ifdef NODE_4
    if (signal_received_4 == 1 && digitalRead(SIGNAL_PIN_4) == 1) {
        signal_received_4 = 0;
        Serial.println("Line 4 reset");
    }
    #endif

    #ifdef NODE_5
    if (signal_received_5 == 1 && digitalRead(SIGNAL_PIN_5) == 1) {
        signal_received_5 = 0;
        Serial.println("Line 5 reset");
    }
    #endif

    #ifdef NODE_6
    if (signal_received_6 == 1 && digitalRead(SIGNAL_PIN_6) == 1) {
        signal_received_6 = 0;
        Serial.println("Line 6 reset");
    }
    #endif
}

String get_ina_data() {
    float power_data[NUM_NODES * 5];  // Initialize the power_data array
    int curTime = millis() - initialMillis;

    int index = 0;  // Index to track position in the power_data array

    #ifdef NODE_1
    power_data[index++] = ina219_1_1.getShuntVoltage_mV();
    power_data[index++] = ina219_1_1.getBusVoltage_V();
    power_data[index++] = power_data[index - 1] + (power_data[index - 3] / 1000); // Load voltage = bus voltage + (shunt voltage / 1000)
    power_data[index++] = ina219_1_1.getCurrent_mA();
    power_data[index++] = ina219_1_1.getPower_mW();
    #endif

    #ifdef NODE_2
    power_data[index++] = ina219_1_2.getShuntVoltage_mV();
    power_data[index++] = ina219_1_2.getBusVoltage_V();
    power_data[index++] = power_data[index - 1] + (power_data[index - 3] / 1000); // Load voltage = bus voltage + (shunt voltage / 1000)
    power_data[index++] = ina219_1_2.getCurrent_mA();
    power_data[index++] = ina219_1_2.getPower_mW();
    #endif

    #ifdef NODE_3
    power_data[index++] = ina219_1_3.getShuntVoltage_mV();
    power_data[index++] = ina219_1_3.getBusVoltage_V();
    power_data[index++] = power_data[index - 1] + (power_data[index - 3] / 1000); // Load voltage = bus voltage + (shunt voltage / 1000)
    power_data[index++] = ina219_1_3.getCurrent_mA();
    power_data[index++] = ina219_1_3.getPower_mW();
    #endif

    #ifdef NODE_4
    power_data[index++] = ina219_1_4.getShuntVoltage_mV();
    power_data[index++] = ina219_1_4.getBusVoltage_V();
    power_data[index++] = power_data[index - 1] + (power_data[index - 3] / 1000); // Load voltage = bus voltage + (shunt voltage / 1000)
    power_data[index++] = ina219_1_4.getCurrent_mA();
    power_data[index++] = ina219_1_4.getPower_mW();
    #endif

    #ifdef NODE_5
    power_data[index++] = ina219_2_5.getShuntVoltage_mV();
    power_data[index++] = ina219_2_5.getBusVoltage_V();
    power_data[index++] = power_data[index - 1] + (power_data[index - 3] / 1000); // Load voltage = bus voltage + (shunt voltage / 1000)
    power_data[index++] = ina219_2_5.getCurrent_mA();
    power_data[index++] = ina219_2_5.getPower_mW();
    #endif

    #ifdef NODE_6
    power_data[index++] = ina219_2_6.getShuntVoltage_mV();
    power_data[index++] = ina219_2_6.getBusVoltage_V();
    power_data[index++] = power_data[index - 1] + (power_data[index - 3] / 1000); // Load voltage = bus voltage + (shunt voltage / 1000)
    power_data[index++] = ina219_2_6.getCurrent_mA();
    power_data[index++] = ina219_2_6.getPower_mW();
    #endif

    String row = String(curTime);
    row += ", " + String(signal_received_1) + ", " +
                String(signal_received_2) + ", " +
                String(signal_received_3) + ", " +
                String(signal_received_4) + ", " +
                String(signal_received_5) + ", " +
                String(signal_received_6) + ", " +
                String(millisDif);
                
    for (int i = 0; i < NUM_NODES * 5; i++) {
        row += ", " + String(power_data[i]);
    }
    row += "\n";
    
    // Serial.println(row);

    if (millisDif > 0) {
        millisDif = 0;
    } 

    return row;
}



void setup() {
    Serial.begin(115200);
    Wire.begin(47, 21); // SDA 47, SCL 21

    // Define Signal Pins and attach interrupts on their falling edges, interpret as a signal received.

    #ifdef NODE_1
    pinMode(SIGNAL_PIN_1, INPUT);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN_1), signal1ISR, FALLING);
    #endif

    #ifdef NODE_2
    pinMode(SIGNAL_PIN_2, INPUT);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN_2), signal2ISR, FALLING);
    #endif

    #ifdef NODE_3
    pinMode(SIGNAL_PIN_3, INPUT);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN_3), signal3ISR, FALLING);
    #endif

    #ifdef NODE_4
    pinMode(SIGNAL_PIN_4, INPUT);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN_4), signal4ISR, FALLING);
    #endif

    #ifdef NODE_5
    pinMode(SIGNAL_PIN_5, INPUT);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN_5), signal5ISR, FALLING);
    #endif

    #ifdef NODE_6
    pinMode(SIGNAL_PIN_6, INPUT);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN_6), signal6ISR, FALLING);
    #endif
    
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


    // Initialize each INA219 sensor
    #ifdef NODE_1
    if (!ina219_1_1.begin()) {
        Serial.println("Failed to find INA219 chip at address 0x40 I2C1");
    }
    #endif

    #ifdef NODE_2
    if (!ina219_1_2.begin()) {
        Serial.println("Failed to find INA219 chip at address 0x41 I2C1");
    }
    #endif

    #ifdef NODE_3
    if (!ina219_1_3.begin()) {
        Serial.println("Failed to find INA219 chip at address 0x44 I2C1");
    }
    #endif

    #ifdef NODE_4
    if (!ina219_1_4.begin()) {
        Serial.println("Failed to find INA219 chip at address 0x45 I2C1");
    }
    #endif

    #ifdef NODE_5
    if (!ina219_2_5.begin()) {
        Serial.println("Failed to find INA219 chip at address 0x40 I2C2");
    }
    #endif

    #ifdef NODE_6
    if (!ina219_2_6.begin()) {
        Serial.println("Failed to find INA219 chip at address 0x41 I2C2z");
    }
    #endif


    // Wipe SD card for new data

    deleteFile(SD_MMC, "/data.csv"); // TEMPORARY
    writeFile(SD_MMC, "/data.csv", "Time, Signal1, Signal2, Signal3, Signal4, Signal5, Signal6, Latency, Bus Voltage 1, Shunt Voltage 1, Load Voltage 1, Current_mA 1, Power_mW 1, Bus Voltage 2, Shunt Voltage 2, Load Voltage 2, Current_mA 2, Power_mW 2, Bus Voltage 3, Shunt Voltage 3, Load Voltage 3, Current_mA 3, Power_mW 3, Bus Voltage 4, Shunt Voltage 4, Load Voltage 4, Current_mA 4, Power_mW 4, Bus Voltage 5, Shunt Voltage 5, Load Voltage 5, Current_mA 5, Power_mW 5, Bus Voltage 6, Shunt Voltage 6, Load Voltage 6, Current_mA 6, Power_mW 6\n");


    initialMillis = millis();
}

int loop_count = 0;
String data = "";
void loop() {
    String row = get_ina_data();
    data = data + row;
    
    if (loop_count == 10) {
        const char* data_c_str = data.c_str();
        appendFile(SD_MMC, "/data.csv", data_c_str); // limitation is the number of appends to file, I instead will just append to a string that will then be appended every 10 ms
    }
    
    reset_signal_variables();
    loop_count++;
    delay(2);
}