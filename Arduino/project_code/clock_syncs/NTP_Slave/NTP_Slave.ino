#include <WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "Config.h" // Your configuration file with SSID and Password

WiFiClient client;
const char* masterIP = "172.28.97.89"; // Replace with the actual IP address of the master ESP32

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (client.connect(masterIP, 12345)) {
    const int dataLength = 8; // Length of the expected time string (HH:MM:SS)
    char timeBuffer[dataLength + 1]; // +1 for the null terminator

    int bytesRead = client.readBytes(timeBuffer, dataLength);
    timeBuffer[bytesRead] = '\0'; // Null-terminate the string

    String timeString = String(timeBuffer);
    // Serial.print("Received time string: ");
    // Serial.println(timeString);

    if (timeString.length() != dataLength) {
      Serial.println("Received an incorrectly sized time string.");
      Serial.println(timeString);
    } else {
      // Check if the string contains the expected format
      if (timeString[2] == ':' && timeString[5] == ':') {
        int hour = timeString.substring(0, 2).toInt();
        int minute = timeString.substring(3, 5).toInt();
        int second = timeString.substring(6, 8).toInt();
        
        // Serial.print("Hour: ");
        // Serial.println(hour);
        // Serial.print("Minute: ");
        // Serial.println(minute);
        // Serial.print("Second: ");
        // Serial.println(second);

        // Set the system time
        setTime(hour, minute, second, 17, 6, 2024);
      } else {
        Serial.println("Invalid time string format");
      }
    }
    
    client.stop();
    
  }
  delay(125); // Synchronize multiple times a second
  Serial.println(now());
}
