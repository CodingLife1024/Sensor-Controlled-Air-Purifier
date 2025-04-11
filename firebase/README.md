# Firebase Cloud Integration for Air Quality System

## Overview
This module adds cloud connectivity to the air quality monitoring system using Firebase Realtime Database. It securely transmits PM2.5, PM10, fan speed, and timestamp data to the cloud every 5 seconds using ESP32's WiFi capabilities.

## Firebase Setup Requirements

### Project Configuration
1. **Enable Realtime Database**
   - Create new Firebase project
   - Select Realtime Database (Legacy mode)
   - Set security rules to public for testing

2. **Authentication**
   - Generate legacy database secret
   - Whitelist ESP32's IP address (optional)

### Credential Management
| Credential Type       | Placeholder              | Location in Code            |
|-----------------------|--------------------------|-----------------------------|
| WiFi SSID             | `<"your-SSID">`          | `const char* ssid`          |
| WiFi Password         | `<"your-PASSWORD">`      | `const char* password`      |
| Firebase Host         | `<"your-project-id">`    | `#define FIREBASE_HOST`     |
| Database Secret       | `<"your-database-secret">` | `#define FIREBASE_AUTH`   |

## Code Explanation

### WiFi Initialization

```cpp
// WiFi credentials
const char* ssid = <"your-SSID">; // Replace with your network SSID
const char* password = <"your-PASSWORD">; // Replace with your network password

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Serial.println("Setup: Initializing system...");

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
```

### Firebase Configuration

```cpp
// Firebase credentials
#define FIREBASE_HOST <"your-project-id.firebaseio.com">
#define FIREBASE_AUTH <"your-database-secret">

// Firebase objects
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

void setup(){
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
}
```

### JSON Data Transmission

```cpp
// Function to send JSON data
void sendData(String dateTime, int pm25, int pm10, int fanSpeed) {
    unsigned long timestamp = millis();  // Unique timestamp

    // Construct the Firebase path
    String path = "/data/" + dateTime;

    // Use FirebaseJson instead of ArduinoJson
    FirebaseJson json;
    json.set("timestamp", dateTime);
    json.set("pm25", pm25);
    json.set("pm10", pm10);
    json.set("fanSpeed", fanSpeed);

    // Send JSON data to Firebase
    if (Firebase.setJSON(firebaseData, path, json)) {
        Serial.println("Data added successfully:");
        Serial.println(json.raw());  // Print JSON data for debugging
    }
    else {
        Serial.println("Failed to add data: " + firebaseData.errorReason());
    }
}
```

## Database Structure

/data
└─ 2025-04-08::14:59:00
    ├─ timestamp: "2025-04-08::14:59:00"
    ├─ pm25: 45
    ├─ pm10: 78
    └─ fanSpeed: 60

## Installation Requirements
1. **Required Libraries:**
   - `WiFi.h` (ESP32 core)
   - `FirebaseESP32` (v3.10.0+)

2. **Arduino Settings:**
   - Enable WiFi and HTTPClient
   - Set board to ESP32 Dev Module
   - Partition Scheme: Minimal SPIFFS

## Calibration Notes
1. Replace all `<>` placeholders with actual credentials
2. Adjust transmission interval in `delay(5000)`
3. Modify JSON structure in `sendData()` for additional sensors
4. Handle Firebase errors in `firebaseData.errorReason()`

## Diagnostic Features
- Serial output of raw JSON payloads
- Automatic WiFi reconnection
- Detailed error reason reporting

## Security Considerations
- Use project-specific service accounts for production
- Implement proper database security rules
- Rotate database secrets periodically
- Consider using TLS encryption

## Expansion Capabilities
1. Add historical data querying
2. Implement Firebase user authentication
3. Create web dashboard using Firebase API
4. Add over-the-air (OTA) updates
5. Enable server-side timestamps
6. Set up cloud-based alerts/notifications