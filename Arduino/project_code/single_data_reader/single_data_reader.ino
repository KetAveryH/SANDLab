#include "Wire.h"
#include "Adafruit_INA219.h"
#include "sd_read_write.h"
#include "SD_MMC.h"

#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.

// Define GPIO pins
#define DEVICE1_SIGNAL_PIN 42
#define DEVICE1_ACK_PIN 41
#define DEVICE2_RECEIVE_PIN 1
#define DEVICE2_ACK_PIN 2

volatile bool device1Sending = false;
volatile bool device2Receiving = false;

int signalState = 0;
int initialMillis = 0;

HardwareSerial SerialPort(1); // Creates an instance for UART1

// Sets acknowledge pin to high on rising edge of Signal Pin
// This signifies the other ESP to set its signal pin to low
void IRAM_ATTR ackPinHighISR() {
    signalState = 1;
    digitalWrite(DEVICE1_ACK_PIN, HIGH);
    Serial.println("Acknowledgement line set High");
}

// Sets acknowledge pin back to low on falling edge of signal pin/
// Resets the state of both lines
void IRAM_ATTR ackPinLOWISR() {
    digitalWrite(DEVICE1_ACK_PIN, LOW);
    Serial.println("Acknowledgement line set Low");
}


void setup() {
    SerialPort.begin(115200, SERIAL_8N1, 47, 21); // Initializes UART1 with custom TX (GPIO10) and RX (GPIO9) pins
    // SD_MMC INITIALIZATION

    pinMode(DEVICE1_SIGNAL_PIN, INPUT);
    pinMode(DEVICE1_ACK_PIN, OUTPUT);

    SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
    if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
      Serial.println("Card Mount Failed");
      return;
    }
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        Serial.println("No SD_MMC card attached");
        return;
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

        

    listDir(SD_MMC, "/", 0);

    // INA 219 INITIALIZATION

    // Open serial communications and wait for port to open:
    Wire.begin(16, 17);
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    if (! ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
    }

    deleteFile(SD_MMC, "/data.csv"); // TEMPORARY
    writeFile(SD_MMC, "/data.csv", "Time, Bus Voltage, Shunt Voltage, Load Voltage, Current_mA, Power_mA, Events, dataReceived\n");
    // on rising edge of signal pin, interrupt and set acknowledge pin to high
    attachInterrupt(digitalPinToInterrupt(DEVICE1_SIGNAL_PIN), ackPinHighISR, RISING);
    // attachInterrupt(digitalPinToInterrupt(DEVICE2_SIGNAL_PIN), ackPinHighISR, RISING);
    // on falling edge of signal pin, interrupt and set acknowledge pin to low, re-establishing initial conditions
    attatchInterrupt(digitalPinToInterrupt(DEVICE1_SIGNAL_PIN), ackPinLowISR, FALLING);
    // attatchInterrupt(digitalPinToInterrupt(DEVICE2_SIGNAL_PIN), ackPinLowISR, FALLING);
    initialMillis = millis();
}

void loop() {
    signalState = digitalRead(signalPin);
    Serial.print(signalState);

    

    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    int curMillis = millis() - initialMillis;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    String row = currentTime + ", " +
            String(busvoltage) + ", " + 
            String(shuntvoltage) + ", " + 
            String(loadvoltage) + ", " + 
            String(current_mA) + ", " + 
            String(power_mW) + ", " +
            String(signalState) + "\n";

    // Write to SD
    const char* row_c_str = row.c_str();
    
    appendFile(SD_MMC, "/data.csv", row_c_str);

    delay(1000);
}