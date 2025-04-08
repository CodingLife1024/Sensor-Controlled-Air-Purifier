# Real-Time Clock Integration for Air Quality System

## Overview
This module adds precise timestamp functionality to the air quality monitoring system using a DS3231 RTC (Real-Time Clock) chip. Maintains accurate timekeeping even during power outages and enables time-stamped data logging.

## Hardware Setup

### RTC Connections
| Module Pin | Connection       | Details                                  |
|------------|------------------|------------------------------------------|
| VCC        | 3.3V/5V          | Power supply (5V recommended)          |
| GND        | GND              | Common ground                            |
| SDA        | GPIO5 (SDA)      | I2C data line                            |
| SCL        | GPIO6 (SCL)      | I2C clock line                           |

## Code Explanation

### RTC Initialization

```
RTC_DS3231 rtc; // Create RTC object
String datetime = "";

void setup() {
    if (!rtc.begin()) { // Initialize communication
        Serial.println("Couldn't find RTC!");
        while(1); // Halt system on failure
    }

    // Auto-compile time setting (uncomment once)
    // rtc.adjust(DateTime(F(DATE), F(TIME)));
}
```

### Time Fetching Function

```
void timeStamp(){
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    DateTime now = rtc.now(); // Fetch the current time from the RTC
    datetime = String(now.year()) + "|" + String(now.month()) + "|" + String(now.day()) + "::"; // Format: YYYY|MM|DD
    datetime += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()); // Format: HH:MM:SS

    // Log the updated time for debugging purposes
    Serial.print("Timer Reset: ");
    Serial.println(datetime);
}
```


## Installation Requirements
1. **Required Libraries:**
   - RTClib (DS3231 specific version)
   - Wire (I2C communication)

2. **Arduino Settings:**
   - Enable I2C support
   - Set correct board type (ESP32)
   - 115200 baud serial rate

## Calibration Notes
1. **Initial Time Setting:**
   - Uncomment `rtc.adjust()` line after upload
   - Re-comment after first run to prevent reset

2. **Accuracy Maintenance:**
   - ±2ppm accuracy (≈±1 minute/year)
   - 3V lithium battery backup (CR1220)

## Diagnostic Features
- Startup initialization check
- Continuous time logging every 5 seconds
- Serial output verification
- Error trapping for RTC disconnection

## Expansion Capabilities
1. Create time-based fan speed profiles
2. Integrate alarm functions for maintenance reminders