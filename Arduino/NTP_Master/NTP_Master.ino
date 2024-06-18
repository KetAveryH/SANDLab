#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Config.h" // Your configuration file with SSID and Password

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

WiFiServer server(12345); // Use port 12345 for communication

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();
  server.begin();

  // Print the IP address
  Serial.print("Master ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  timeClient.update();
  WiFiClient client = server.available();
  String currentTime = timeClient.getFormattedTime();
  Serial.println(currentTime);
  if (client) {
    currentTime = timeClient.getFormattedTime();
    Serial.println(currentTime);
    // Send start marker, data, and end marker
    client.print(currentTime);
    client.stop();
  }
  delay(1000); // Synchronize every second
}
