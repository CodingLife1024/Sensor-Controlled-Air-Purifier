# Arduino IDE Setup for Smart Air Purifier Project

This guide will help you set up the Arduino IDE to modify and upload code for the Smart Air Purifier project using an ESP32 board.

---

## 1. Install the Arduino IDE

- Download and install the latest Arduino IDE (1.8.x or 2.x) from [arduino.cc](https://www.arduino.cc/en/software).

---

## 2. Install ESP32 Board Support

1. Open Arduino IDE.
2. Go to **File > Preferences**.
3. In the **Additional Boards Manager URLs** field, add:
    ```
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```
4. Click **OK**.
5. Go to **Tools > Board > Boards Manager**.
6. Search for **ESP32** and install **ESP32 by Espressif Systems**.

---

## 3. Required Libraries

The following libraries are required for this project:

- `Plantower_PMS7003` (for PMS7003 air quality sensor)
- `Wire` (I2C communication, built-in)
- `RTClib` (real-time clock)
- `Adafruit_GFX` (graphics library for OLED)
- `Adafruit_SSD1306` (OLED display driver)
- `Adafruit_BusIO` (dependency for Adafruit libraries)
- `FS` (file system, built-in)
- `SD` (SD card support, built-in)
- `SPI` (SPI communication, built-in)
- `WiFi` (ESP32 WiFi, built-in)
- `FirebaseESP32` (for Firebase connectivity)

---

## 4. Install Libraries via Library Manager

1. Go to **Sketch > Include Library > Manage Libraries**.
2. Search for and install the following:
    - **Adafruit SSD1306**
    - **Adafruit GFX Library**
    - **Adafruit BusIO**
    - **RTClib**
    - **Firebase ESP32** (or **Firebase_ESP32_Client**)
    - **Plantower PMS7003** (if not available, see manual install below)

---

## 5. Add Custom Libraries Manually (if needed)

If any libraries (like `Plantower_PMS7003` or `FirebaseESP32`) are not available via Library Manager:

- Copy the corresponding folders from your `libraries` directory (e.g., `Plantower_PMS7003-master`, `Firebase_ESP32_Client`) into your Arduino `libraries` directory (usually `~Documents/Arduino/libraries/`).

---

## 6. Select the Correct Board and Port

- Go to **Tools > Board** and select your ESP32 board (e.g., "XIAO_ESP32S3").
- Go to **Tools > Port** and select the port for your ESP32.

---

## 7. Compile and Upload

- Open your project code in the Arduino IDE.
- Click the **Upload** button.

---

## Summary Table: Required Libraries

| Library Name             | Purpose                             | Install Method         |
|--------------------------|-------------------------------------|-----------------------|
| Plantower_PMS7003        | PMS7003 sensor                      | Manual/Library Manager|
| Wire                     | I2C communication                   | Built-in              |
| RTClib                   | Real-time clock                     | Library Manager       |
| Adafruit_GFX             | OLED graphics primitives            | Library Manager       |
| Adafruit_SSD1306         | OLED display driver                 | Library Manager       |
| Adafruit_BusIO           | Dependency for Adafruit libraries   | Library Manager       |
| FS                       | File system                         | Built-in              |
| SD                       | SD card support                     | Built-in              |
| SPI                      | SPI communication                   | Built-in              |
| WiFi                     | WiFi for ESP32                      | Built-in              |
| FirebaseESP32            | Firebase connectivity               | Manual/Library Manager|

---

## Example: Libraries Section in Code

```
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
```

---

**Notes:**

- Only the libraries listed above are needed for code upload.
- If you encounter missing dependencies, use the Library Manager to install them.
- Restart Arduino IDE after adding new libraries manually.