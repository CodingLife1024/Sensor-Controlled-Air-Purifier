# Air Quality Monitoring System with OLED Display

## Overview
This extension to the PM2.5 controlled fan system adds real-time air quality visualization using an SSD1306 OLED display and serial data logging. The system shows PM2.5, PM10, fan speed, and timestamp information on a 128x32 pixel display while logging CSV-formatted data to the serial monitor.

## Hardware Setup

### OLED Connections
| Display Pin | Connection       | Details                                  |
|-------------|------------------|------------------------------------------|
| VCC         | 3.3V/5V          | Power supply (check display specs)       |
| GND         | GND              | Common ground                            |
| SCL         | GPIO22 (SCL)     | I2C clock line                           |
| SDA         | GPIO21 (SDA)     | I2C data line                            |

### Enhanced Sensor Integration
- PMS7003 UART RX → Arduino TX (D1)
- RTC SDA → GPIO21
- RTC SCL → GPIO22

## Code Explanation

### Display Initialization

```
#define SCREEN_WIDTH 128 // OLED dimensions
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        while(1);
    }
}
```


### Real-Time Display Function

```
void oled(String dateTime, int pm25, int pm10, int fanSpeed){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Display Date and Time
    display.setCursor(0, 0);
    display.println(dateTime);

    // Display PM2.5 and PM10 values
    display.setCursor(0, 10);
    display.print("PM2.5: ");
    display.print(pm25);
    display.print(", PM10: ");
    display.println(pm10);

    // Display Fan Speed
    display.setCursor(0, 20);
    display.print("Fan Speed: ");
    display.println(fanSpeed);

    display.display();
    Serial.println("OLED: Display updated.");
}
```

### Data Logging Implementation

```
// CSV-formatted serial output
String dataLine = String(datetime) + "," + String(pm10) + ","
+ String(pm25) + "," + String(fanSpeed);
Serial.println(dataLine);
```


## Installation Requirements
1. **Required Libraries:**
   - Adafruit_GFX (graphics core)
   - Adafruit_SSD1306 (display driver)
   - RTClib (for timestamp functionality)

2. **Arduino Settings:**
   - I2C enabled
   - 115200 baud serial rate
   - SSD1306 128x32 configuration

## Calibration Notes
1. Adjust display contrast with `SSD1306_SWITCHCAPVCC` value
2. Modify screen refresh rate in `delay(5000)`
3. Customize CSV format in `dataLine` construction

## Diagnostic Features
- Serial output verification
- Display initialization checks
- Continuous update confirmation messages
- Error trapping for display failures

## Expansion Capabilities
1. Add secondary display page with historical data
2. Add touch interface for mode switching
2. Integrate LED status indicators
