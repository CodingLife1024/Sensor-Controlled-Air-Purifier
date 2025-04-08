/*********************************************************************
This version reads real-time PM2.5, PM10 from the Plantower PMS7003
sensor and adjusts the fan speed based on PM2.5. It also displays
the time, PM2.5, PM10, and fan speed on an OLED display.
**********************************************************************/

#include "Plantower_PMS7003.h"
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FS.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Speed control variables
const int pwmPin = 4;  // GPIO4 for PWM output
int fanSpeed = 10;

const int inPin = D2;

void setup() {
    Serial.begin(115200);
    Serial.println("Setup: Initializing system...");
    // ...
    // Fan Speed control initialization
    pinMode(pwmPin, OUTPUT);
    // analogWrite(pwmPin, 255); // Optional full speed
    Serial.println("Fan control initialized");
    // ...
}

void acdimmer(int pm25) {
  Serial.print("Adjusting fan speed based on PM2.5: ");
  Serial.println(pm25);

  if (pm25 < 15) {
    fanSpeed = 30; // Very low fan speed
  }
  else if (pm25 >= 15 && pm25 < 50) {
    fanSpeed = 45; // Low fan speed
  }
  else if (pm25 >= 50 && pm25 < 100) {
    fanSpeed = 60; // Moderate fan speed
  }
  else if (pm25 >= 100 && pm25 < 150) {
    fanSpeed = 70; // High fan speed
  }
  else if (pm25 >= 150 && pm25 < 200) {
    fanSpeed = 90; // Very high fan speed
  }
  else {
    fanSpeed = 100; // Maximum fan speed
  }

  int fanSpeedAnalog = map(fanSpeed, 0, 100, 0, 255);  // Map percentage to analog value
  analogWrite(pwmPin, fanSpeedAnalog);  // Apply PWM to fan
  Serial.print("PWM value on GPIO4 (D3): ");
  Serial.println(fanSpeedAnalog);

  Serial.print("Fan Speed Set To: ");
  Serial.print(fanSpeed);
  Serial.println("%");
}
