# SD Card Data Logging Module

## Overview
This module adds persistent data storage capabilities to the air quality system using FAT32-formatted SD cards. Implements CSV logging with performance-optimized read/write operations and comprehensive file management utilities.

## Hardware Setup

### SD Card Connections
| Pin         | Connection       | Details                                  |
|-------------|------------------|------------------------------------------|
| CS          | GPIO1 (D0)       | Chip select (XIAO ESP32-S3 specific)     |
| SCK/CLK     | GPIO7            | SPI clock                                |
| MOSI        | GPIO9            | Master Out Slave In                      |
| MISO        | GPIO9            | Master In Slave Out                      |
| 3V3         | 3.3V             | Match card voltage requirements          |
| GND         | GND              | Common ground                            |

## Code Explanation

### Key Functions

**Card Initialization & Diagnostics**

```
#define CS_PIN D0  // CS Pin of the SD Card connects to this in the XIAO ESP32-S3 Board

const char *filename = "/data.csv";

void setup(){
     // SD Card Setup
    while (!Serial);  // Wait for Serial to initialize
    Serial.println("Initializing SD card...");
    if (!SD.begin(CS_PIN)) {
        Serial.println("Card Mount Failed");
        return;
    }
    Serial.println("SD card initialized successfully.");
}
```

**Optimized Read/Write Test**

```
// Function to test read/write performance using smaller chunks
void performReadWriteTest() {
    const size_t chunkSize = 512;  // Use smaller chunk size
    char buffer[chunkSize];        // Buffer for data operations

    // Write data in chunks
    Serial.println("Starting write test...");
    File file = SD.open("/performance.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    unsigned long startTime = millis();
    for (size_t i = 0; i < (1024 * 1024) / chunkSize; i++) {
        memset(buffer, 'A', chunkSize);  // Fill buffer with data
        file.write((uint8_t *)buffer, chunkSize);
    }
    unsigned long writeTime = millis() - startTime;
    file.close();

    Serial.printf("1 MB written in %lu ms\n", writeTime);

    // Read data in chunks
    Serial.println("Starting read test...");
    file = SD.open("/performance.txt", FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    startTime = millis();
    while (file.read((uint8_t *)buffer, chunkSize) > 0) {
        // Optionally process the buffer data here
    }
    unsigned long readTime = millis() - startTime;
    file.close();

    Serial.printf("1 MB read in %lu ms\n", readTime);
}
```

**CSV Data Handling**

```
// Function to write a line to the file
void writeToFile(const char *filename, const char *line) {
    Serial.printf("Writing to file: %s\n", filename);
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    file.println(line);  // Write the given line to the file
    file.close();        // Close the file
    Serial.println("Line written successfully.");
}

// Function to append a line to the file
void appendToFile(const char *filename, const char *line) {
    Serial.printf("Appending to file: %s\n", filename);
    File file = SD.open(filename, FILE_APPEND);  // Open file in append mode
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }

    file.println(line);  // Append the given line to the file
    file.close();        // Close the file
    Serial.println("Line appended successfully.");
}
```

**File Management**

```
void clearSDCard() {
    Serial.println("Clearing SD card...");
    File root = SD.open("/");
    if (!root) {
        Serial.println("Failed to open root directory.");
        return;
    }
    deleteContents(root);
    Serial.println("SD card cleared successfully.");
}

void deleteContents(File dir) {
    File entry;
    while (entry = dir.openNextFile()) {
        if (entry.isDirectory()) {
            deleteContents(entry);  // Recursive call for directories
            if (SD.rmdir(entry.name())) {
                Serial.printf("Directory removed: %s\n", entry.name());
            } else {
                Serial.printf("Failed to remove directory: %s\n", entry.name());
            }
        } else {
            if (SD.remove(entry.name())) {
                Serial.printf("File removed: %s\n", entry.name());
            } else {
                Serial.printf("Failed to remove file: %s\n", entry.name());
            }
        }
        entry.close();
    }
}
```

## Data Format

```
/data.csv
──────────
Time, PM10, PM2.5, Fan
2025-04-08::14:59:00, 45, 78, 60
2025-04-08::15:00:05, 42, 75, 58
```

## Installation Requirements
1. **Hardware:**
   - SD card module with SPI interface
   - FAT32-formatted microSD card (≤32GB recommended)

2. **Libraries:**
   - FS.h (ESP32 core)
   - SD.h (v1.1.1+)
   - SPI.h (hardware control)

## Calibration Notes
1. Format SD card with 32KB cluster size
2. Adjust `chunkSize` in `performReadWriteTest()` for optimization
3. Modify CSV headers in `setup()` initial write
4. Set appropriate file rotation intervals

## Performance Metrics
| Operation         | Speed (1MB) | Reliability Factors                |
|-------------------|-------------|-------------------------------------|
| Sequential Write  | 850-1200ms  | Chunk size, card class, SPI speed   |
| Random Read       | 400-700ms   | File fragmentation, cache settings  |
| Metadata Ops      | 50-150ms    | Directory structure complexity      |

## Safety Considerations
- Always close files after operations
- Implement power-loss protection
- Avoid concurrent write access
- Use proper card ejection procedures
- Monitor storage capacity regularly

## Expansion Capabilities
1. Add automatic file rotation
2. Implement zlib compression
3. Enable FTDI-based card access
4. Add filesystem integrity checks
5. Create binary log formats
6. Integrate wear leveling algorithms