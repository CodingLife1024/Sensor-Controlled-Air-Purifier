#include "FS.h"
#include <SD.h>
#include <SPI.h>

#define CS_PIN D0  // CS Pin of the SD Card connects to this in the XIAO ESP32-S3 Board

const char *filename = "/data.csv";

void setup(){
    // Serial Monitor
    Serial.begin(115200);
    Serial.println("Setup: Initializing system...");

    // SD Card Setup
    while (!Serial);  // Wait for Serial to initialize
    Serial.println("Initializing SD card...");
    if (!SD.begin(CS_PIN)) {
        Serial.println("Card Mount Failed");
        return;
    }
    Serial.println("SD card initialized successfully.");

    // Display SD card info
    displaySDCardInfo();

    // Perform read/write performance test
    performReadWriteTest();
    writeToFile(filename, "Time, PM10, PM2.5, Fan");
    delay(1000);
}

void loop(){
    appendToFile(filename, line.c_str());
    readFromFile(filename);

    delay(5000);  // Delay for 5 second before the next loop iteration
}

// Function to display SD card information
void displaySDCardInfo() {
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("Card Type: ");
    switch (cardType) {
        case CARD_MMC:
            Serial.println("MMC");
            break;
        case CARD_SD:
            Serial.println("SDSC");
            break;
        case CARD_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("UNKNOWN");
    }

    uint64_t totalBytes = SD.totalBytes();
    uint64_t usedBytes = SD.usedBytes();

    Serial.printf("Total space: %lluMB\n", totalBytes / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", usedBytes / (1024 * 1024));
}

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

// Function to read and display the contents of the file
void readFromFile(const char *filename) {
    Serial.printf("Reading from file: %s\n", filename);
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.println("File contents:");
    while (file.available()) {
        char c = file.read();  // Read a character
        Serial.print(c);       // Print it to the Serial Monitor
    }
    file.close();              // Close the file
    Serial.println("\nFile reading completed.");
}

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
