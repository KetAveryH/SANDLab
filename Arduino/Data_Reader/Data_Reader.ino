#include "Wire.h"
#include "Adafruit_INA219.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Config.h" // Your configuration file with SSID and Password
#include "sd_read_write.h"
#include "SD_MMC.h"

#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);
Adafruit_INA219 ina219(0x40);
HardwareSerial SerialPort(1); // Creates an instance for UART1
const int signalPin = 14;



void setup() {
  SerialPort.begin(115200, SERIAL_8N1, 47, 21); // Initializes UART1 with custom TX (GPIO10) and RX (GPIO9) pins
  // SD_MMC INITIALIZATION

  pinMode(signalPin, INPUT);

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

  // WIFI INITIALIZATION

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();
  deleteFile(SD_MMC, "/data.csv"); // TEMPORARY
  writeFile(SD_MMC, "/data.csv", "Time, Bus Voltage, Shunt Voltage, Load Voltage, Current_mA, Power_mA, Events, dataReceived\n");

}

void loop() {
  String eventData = "";

  if (SerialPort.available() > 0) {
    String receivedData = SerialPort.readStringUntil('\n');
    eventData += receivedData;
    Serial.print("Received on UART1: ");
    Serial.println(receivedData);
  }

  int signalState = digitalRead(signalPin);
  Serial.print(signalState);

  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  // Serial.println(currentTime);

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
             String(eventData) + ", " +
             String(signalState) + "\n";

  Serial.println(row);

  // Write to SD
  const char* row_c_str = row.c_str();
  
  appendFile(SD_MMC, "/data.csv", row_c_str);

  delay(125);
}