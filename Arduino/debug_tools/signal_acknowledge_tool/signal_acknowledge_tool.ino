/**
 * @file signal_acknowledge_tool.ino
 * @brief This file allows you to control an output pins state (high/low) using touch pins as an interface
 *
 * The program defaults your desired SIGNAL_PIN to high or low. 
 * If pin 14 is touched SIGNAL_PIN will be set to Low.
 * If pin 3 is touched SIGNAL_PIN will be set to High.
 *
 * Author: Ket Hollingsworth
 * Date: 6/28/2024
 */

#include "Wire.h"
#define SIGNAL_PIN 21
#define TOUCH_THRESH 30000

void setup() {
    Serial.begin(115200);
    pinMode(SIGNAL_PIN, OUTPUT);
    digitalWrite(SIGNAL_PIN, HIGH);
}

void loop() {
    delay(1000);

    Serial.print(touchRead(T14));
    Serial.print(" ");
    Serial.println(touchRead(T3));
    if (touchRead(T14) > TOUCH_THRESH) {
      digitalWrite(SIGNAL_PIN, LOW);
      Serial.println("Signal Set Low");
    } else if (touchRead(T3) > TOUCH_THRESH) {
      digitalWrite(SIGNAL_PIN, HIGH);
      Serial.println("Signal Set High");
    }

}
