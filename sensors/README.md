# PMS7003 Air Quality Sensor Integration

## Overview
This module implements PM2.5 and PM10 monitoring using the Plantower PMS7003 laser particle sensor. The system provides real-time particulate matter concentration data through serial communication with automatic data validation and error handling.

## Hardware Setup

### PMS7003 Connections
| Sensor Pin | Connection       | Details                                  |
|------------|------------------|------------------------------------------|
| VCC (5V)   | 5V Power         | Requires stable 5V supply [1][4]         |
| GND        | GND              | Common ground                            |
| TX         | GPIO44 (RX1)     | Sensor data output [1][3]                |
| RX         | GPIO43 (TX1)     | Sensor control input (unused in passive) |

## Code Explanation

### Sensor Initialization

```
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
```

### Data Acquisition Loop

```
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
```


## Installation Requirements
1. **Required Libraries:**
   - [Plantower_PMS7003](https://github.com/jmstriegel/Plantower_PMS7003)
   
2. **Arduino Settings:**
   - Board: ESP32 Dev Module
   - Partition Scheme: Default
   - Enable Hardware Serial1

## Calibration Notes
1. Position sensor vertically with unobstructed airflow [5]
2. Allow 30s warm-up after power-on
3. Avoid placement near:
   - Direct airflow sources
   - High humidity areas
   - Electromagnetic interference

## Diagnostic Features
- Serial output of raw measurements
- 300-frame data acquisition timeout
- Automatic checksum verification
- Error reporting for stale data

## Technical Specifications
| Parameter          | Value              |
|--------------------|--------------------|
| Measurement Range  | 0-1000 µg/m³       |
| Particle Size      | 0.3-10 µm          |
| Update Interval    | 1s (typical)       |
| Power Consumption  | 90mA (operational) |

## Expansion Capabilities
2. Implement data averaging filters
3. Enable active mode control
4. Integrate with weather stations
5. Add particle count diagnostics