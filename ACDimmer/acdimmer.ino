/*********************************************************************
This is the code for AC fan speed control using and AC dimmer module.
It uses a zero-crossing detection method to control the TRIAC
for dimming the AC voltage applied to the fan.
**********************************************************************/

#include <Arduino.h>

// Pin Definitions
const int zeroCrossPin = D2;  // Zero-crossing detection pin
const int dimmerPin = D3;     // Dimmer control pin (TRIAC control)

// Dimming level (0-100), where 100 is full speed
int dimmingLevel = 50;  // Start with 50% speed for testing

// Variables to hold timing values
volatile bool zeroCrossDetected = false;
unsigned long dimmingTime;

// Interrupt function for zero crossing detection
void IRAM_ATTR zeroCrossInterrupt() {
    zeroCrossDetected = true;
}

void setup() {
    pinMode(zeroCrossPin, INPUT);
    pinMode(dimmerPin, OUTPUT);

    // Serial for debugging
    Serial.begin(115200);

    // Attach interrupt to zero-crossing pin
    attachInterrupt(digitalPinToInterrupt(zeroCrossPin), zeroCrossInterrupt, RISING);
}

void loop() {
    if (zeroCrossDetected) {
        zeroCrossDetected = false;

        // Calculate the dimming time based on the desired speed
        const int halfCycleTime = 10000;  // For 50Hz (use 10000 for 50Hz)
        dimmingTime = (halfCycleTime * (100 - dimmingLevel)) / 100;

        // Debugging output
        Serial.print("Dimming Level: ");
        Serial.println(dimmingLevel);
        Serial.print("Dimming Time (us): ");
        Serial.println(dimmingTime);

        // Wait for calculated delay before triggering TRIAC
        delayMicroseconds(dimmingTime);

        // Trigger TRIAC
        digitalWrite(dimmerPin, HIGH);  // Turn on TRIAC
        delayMicroseconds(50);          // Increase TRIAC on time for better triggering
        digitalWrite(dimmerPin, LOW);   // Turn off TRIAC
    }
}