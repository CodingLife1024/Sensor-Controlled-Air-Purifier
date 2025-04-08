/*********************************************************************
This code
**********************************************************************/

#include <Wire.h>
#include "RTClib.h"

// RTC
RTC_DS3231 rtc;
String datetime = "";

void setup()
{
    // Serial Monitor
    Serial.begin(115200);
    Serial.println("Setup: Initializing system...");

    // RTC Setup
    Serial.println("Setup: Initializing RTC...");
    if (!rtc.begin()) {
        Serial.println("Error: Couldn't find RTC!");
        while (1) delay(10);
    }
    Serial.println("RTC initialized successfully.");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //reset this after every new upload

}

void loop(){
    Serial.println("\n--- New Loop Iteration ---");

    // Get the current time from RTC
    Serial.println("Getting current time...");
    timeStamp();
    Serial.print("Current Time: ");
    Serial.println(datetime);

    delay(5000); // Wait 5 seconds before the next loop
}

void timeStamp(){
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    DateTime now = rtc.now(); // Fetch the current time from the RTC
    datetime = String(now.year()) + "|" + String(now.month()) + "|" + String(now.day()) + "::"; // Format: YYYY|MM|DD
    datetime += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()); // Format: HH:MM:SS

    // Log the updated time for debugging purposes
    Serial.print("Timer Reset: ");
    Serial.println(datetime);
}