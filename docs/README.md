# Project Timeline and Progress Report

## Overview
This document outlines the development history of the air quality monitoring system, detailing the challenges faced and solutions implemented during each phase of the project.

## Timeline of Development

### **Phase 1: Integration of Additional Components**
- **Objective:** Gradually add other system components while maintaining modularity.
- **Components Added:**
  - **OLED Display:** Enabled real-time visualization of PM2.5, PM10, fan speed, and timestamp.
  - **Sensor Control:** Integrated Plantower PMS7003 sensor for accurate air quality readings.
  - **RTC Clock:** Added DS3231 RTC for precise timestamping of data.
  - **Internet Connection:** Established WiFi connectivity for cloud integration.
  - **Firebase Storage:** Implemented Firebase Realtime Database for remote data storage.
  - **SD Card Storage:** Enabled local data logging on a microSD card.

### **Phase 2: AC Fan and AC Dimmer for Speed Control**
- **Objective:** Implement speed control for an AC fan using a TRIAC-based dimmer.
- **Key Milestones:**
  - Successfully controlled fan speed using phase-angle dimming.
  - Encountered integration issues when combining dimmer code with other modules.
  - Timing-sensitive nature of AC dimmer control caused conflicts with additional functionalities like sensors and displays.

### **Phase 3: Transition to DC Fan (PWM Control)**
- **Objective:** Resolve integration issues by replacing the AC fan with a DC fan.
- **Key Milestones:**
  - Acquired a 12V DC fan with PWM speed control capability.
  - Implemented PWM-based speed control, which proved simpler and more reliable than TRIAC-based dimming.
  - Achieved seamless integration with other system components.

### **Phase 4: Hardware Assembly**
- **Objective:** Solder all components onto a dotboard for a compact and durable setup.
- **Key Milestones:**
  - Soldered OLED display, PMS7003 sensor, RTC clock, SD card module, and DC fan onto a dotboard.
  - Integrated the setup with a cylindrical HEPA filter for air purification functionality.
  - Completed testing and validation to ensure stable operation.

## Final Outcome
- The system now works reliably with:
  - Real-time air quality monitoring (PM2.5 and PM10).
  - Adjustable fan speed using PWM control.
  - Data visualization on an OLED display.
  - Timestamped data logging to Firebase and SD card storage.
- The entire setup is compact, functional, and integrated with a cylindrical HEPA filter for effective air purification.

## Lessons Learned
1. AC dimmer control is complex and prone to timing conflicts in multi-functional systems. Transitioning to DC fans simplified the design significantly.
2. Modular development allowed gradual addition of components without overwhelming the system design process.
3. Proper soldering and hardware assembly are crucial for stability and durability in real-world applications.

## Future Plans
1. Add mobile app integration for remote monitoring and control.
2. Implement advanced analytics (e.g., historical trends) using cloud data storage.
3. Explore additional sensors (e.g., temperature, humidity) for enhanced functionality.

