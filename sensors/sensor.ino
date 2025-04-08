/*********************************************************************
This code reads real-time PM2.5, PM10 from the Plantower PMS7003
sensor.
**********************************************************************/

#include "Plantower_PMS7003.h"

// Sensor Variables
int pm25 = 0;
int pm10 = 0;

// PMS7003 (Using HardwareSerial instead of SoftwareSerial)
HardwareSerial ss(1); // Use Serial1 for PMS7003
Plantower_PMS7003 pms7003;

void setup(){
    // Serial Monitor
    Serial.begin(115200);
    Serial.println("Setup: Initializing system...");

    // PMS7003 Setup
    Serial.println("Setup: Initializing PMS7003 Sensor...");
    ss.begin(9600, SERIAL_8N1, 44, 43); // RX = GPI44, TX = GPI43
    pms7003.init(&ss);
    Serial.println("PMS7003 Sensor initialized.");
}

void loop(){
    // Get PMS7003 sensor values for PM2.5 and PM10
    Serial.println("Reading PMS7003 sensor values...");
    pmsRead();
    Serial.print("PM2.5: ");
    Serial.print(pm25);
    Serial.print(" µg/m3, PM10: ");
    Serial.print(pm10);
    Serial.println(" µg/m3");
}

void pmsRead(){
    Serial.println("PMS7003: Waiting for new data...");
    for (int i = 0; i < 300; i++) {
        pms7003.updateFrame();
        if (pms7003.hasNewData()) {
        pm25 = pms7003.getPM_2_5();
        pm10 = pms7003.getPM_10_0();
        Serial.println("PMS7003: New data received.");
        return;
        }
    }
    Serial.println("PMS7003: Warning - No new data received!");
}