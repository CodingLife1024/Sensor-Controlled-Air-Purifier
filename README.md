# Sensor-controlled Smart Air Purifier

A smart air purifier system designed to monitor air quality using a PMS7003 particulate matter sensor and dynamically adjust fan speed based on PM2.5 and PM10 concentrations. The system uses a Seeed Xiao ESP32S3 microcontroller to collect and store data, reduce noise pollution, and sync with a Firebase Realtime Database. It is designed to integrate with an actual air purifier unit, although currently the sensing and filtering systems are separate.

## Features

- **Real-time air quality monitoring** (PM2.5, PM10)
- **Dynamic fan speed control** based on air quality readings
- **Noise reduction** by adapting fan speed intelligently
- **Local data logging** on SD card with timestamps
- **Cloud sync** with Firebase Realtime Database
- **OLED Display** for on-device visualization of air quality and fan speed
- **Compact and efficient** microcontroller-based setup (Xiao ESP32S3)
- Designed to be integrated into **real air purifier hardware**

## Hardware Used

| Component              | Description                                           |
|------------------------|-------------------------------------------------------|
| Plantower PMS7003 PM Sensor      | Detects particulate matter (PM1.0, PM2.5, PM10)       |
| Seeed Xiao ESP32S3     | Microcontroller for data collection and control       |
| Fan (PWM Controlled)   | Adjusts speed according to air quality                |
| MicroSD Card + Module  | Local storage for logs                                |
| OLED Display (SSD1306) | Displays PM readings and fan speed in real time       |
| Power Supply           | USB / Battery-powered depending on deployment / AC Power Source         |
| Physical Air Purifier  | (Optional) Filter and fan system (currently separate) |

## Software Architecture

### Data Collection

- The PMS7003 sensor is polled regularly.
- PM1.0, PM2.5, and PM10 readings are parsed from the sensor's binary output.
- The microcontroller calculates average concentrations over short intervals.

### Fan Speed Control

- Fan speed is adjusted using PWM signals based on pollution levels:
  - **Low PM** → **Low fan speed**
  - **Moderate PM** → **Medium fan speed**
  - **High PM** → **High fan speed**
- The goal is to reduce noise and energy usage in low-pollution scenarios.

### Data Logging

- Readings are saved as CSV records on the SD card:
  ```
  Time,PM10,PM2.5,Fan
  ```

### Cloud Sync

- Readings are also uploaded to a **Firebase Realtime Database** in near real-time when Wi-Fi is available.
- Database fields:
  - `timestamp`
  - `pm25`
  - `pm10`
  - `fanSpeed`

### On-Device Display

- An **OLED display (SSD1306)** is used to show:
  - Current PM2.5 and PM10 values
  - Fan speed as a percentage
  - Device status (e.g., Wi-Fi sync, SD card logging)

## Firebase Structure

```json
{
  "data": {
    "2025|3|5::19:13:55": {
      "pm10": 25,
      "pm25": 49,
      "fanSpeed": 80,
      "timestamp":"2025|3|5::19:13:55",
    },
    ...
  }
}
```

## Folder Structure

```
/smart-air-purifier/
├── sensors/                     # Sensor interfacing
│   ├── sensor.ino
│   ├── sensorpins.jpg
│   └── README.md
│
├── control/                     # Fan speed and PWM control
│   ├── control.ino
│   ├── fanwires.jpg
│   └── README.md
│
├── ACDimmer/                    # Fan speed with AC fan
│   ├── acdimmer.ino
│   └── README.md
│
├── display/                     # OLED Screen Display
│   ├── display.ino
│   └── README.md
│
├── clock/                       # Real Time Clock
│   ├── clock.ino
│   └── README.md
│
├── storage/                     # SD card logging utilities
│   ├── storage.ino
│   └── README.md
│
├── firebase/                    # Firebase client interface
│   ├── firebase.ino
│   └── README.md
│
├── logs/                        # Sample json logs
│   └── sample_data.json
│
├── docs/                        # Documentation and schematics
│   ├── schematics.png
│   ├── schematics.jpg
│   └── README.md
│
├── full_code.ino                # The Complete Code with all components integrated
│
└── README.md                    # This file
```

## Setup Instructions

1. **Connect Hardware** as per schematics in `/docs/`.
2. **Flash Firmware** to ESP32S3 using Arduino IDE or PlatformIO.
3. **Insert SD Card** (FAT32 formatted).
4. **Configure Wi-Fi & Firebase** credentials.
5. **Power On** the system and monitor logs on serial output.

## Planned Enhancements

### Hardware & Sensor Improvements
- [ ] Add temperature and humidity sensors (e.g., DHT22 or BME280) to correlate air quality with environmental conditions.
- [ ] Include a gas sensor (like MQ135 or CCS811) for detecting CO₂, VOCs, and other harmful gases.
- [ ] Battery backup system with charging support for uninterrupted monitoring during power cuts.

### Fan Control & Airflow Optimization
- [ ] Implement a more granular PID-based fan speed control for smoother operation and better response to pollution spikes.
- [ ] Implement a more detailed fan speed control based on historical sensor data rather than instanteneous measurements.
- [ ] Introduce a "quiet night mode" with reduced fan speed and dimmed display during sleep hours.

### Connectivity & Data Logging
- [ ] Add offline queuing for Firebase updates when the internet is down and auto-sync when reconnected.
- [ ] Add MQTT support for integration with smart home systems.

### Dashboard & UX Enhancements
- [ ] Add historical trend visualization with time-based filtering (e.g., daily, weekly, monthly).
- [ ] Provide health recommendations based on AQI (e.g., "Limit outdoor activity" when AQI > 150).
- [ ] Support push notifications or email alerts via MQTT or Firebase Cloud Messaging when air quality degrades.

### Mobile Integration & App Features
- [ ] Build a companion mobile app using Flutter or React Native for live monitoring and remote control.

### Analytics & Intelligence
- [ ] Correlate external weather API data (e.g., wind speed, rainfall) with indoor air quality for pattern analysis.

### Alerts
- [ ] Add buzzer alert system for high PM levels

### Visualization
- [ ] Develop a dashboard for visualizing air quality trends


## 📈 Traffic

[View Full Report](./traffic_report.md)

![Traffic Graph](./traffic_plot.png)

## Authors

- **Riddhidipta Pal** – Final Year Undergraduate, Computer Science and Engineering, IIT Delhi  
  _This project is supervised by [Prof. Shahzad Gani](https://www.linkedin.com/in/shahzadgani/) and [Prof. Rijurekha Sen](https://www.linkedin.com/in/rijurekha-sen-070731103/)._  
  _Guidance was also provided by Kajal, PhD student under Prof. Shahzad Gani._
