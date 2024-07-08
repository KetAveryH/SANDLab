/**
 * @file edge_detection.ino
 * @brief Example file to detect rising and falling edges on pin 4 using an ISR.
 *
 * This file sets up an interrupt on pin 4 to detect rising and falling edges.
 * When a rising edge is detected, it sets the variable `rose` to 1.
 * When a falling edge is detected, it sets the variable `fall` to 1.
 * The loop function prints the values of `rose` and `fall` when they change.
 * 
 * Author: Ket Hollingsworth
 * Date: 7/2/2024
 */

volatile int rose = 0;
volatile int fall = 0;

// ISR for handling pin 4 changes
void IRAM_ATTR handlePinChange() {
    // Check the state of pin 4
    if (digitalRead(4) == HIGH) {
        rose = 1;
    } else {
        fall = 1;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(4, INPUT); // Set pin 4 as input

    // Attach interrupt to pin 4 for both rising and falling edges
    attachInterrupt(digitalPinToInterrupt(4), handlePinChange, CHANGE);
}

void loop() {
    // Check if the rose variable has been set
    if (rose == 1) {
        noInterrupts(); // Disable interrupts while processing
        Serial.println("Rising edge detected on pin 4");
        rose = 0; // Reset the variable
        interrupts(); // Re-enable interrupts
    }

    // Check if the fall variable has been set
    if (fall == 1) {
        noInterrupts(); // Disable interrupts while processing
        Serial.println("Falling edge detected on pin 4");
        fall = 0; // Reset the variable
        interrupts(); // Re-enable interrupts
    }
    
    delay(10); // Small delay to reduce the number of prints
}