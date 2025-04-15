#include "SD_MMC.h"
#include "esp_camera.h"

// Pin definitions for ESP32-CAM SD card (adjust if your board uses different pins)
#define SD_MMC_CLK  14  // CLK pin
#define SD_MMC_CMD  15  // CMD pin
#define SD_MMC_DATA 2   // D0 pin (1-bit mode)

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for Serial to initialize
  }
  Serial.println("\n\n=== ESP32-CAM SD Card Test ===");

  // Configure SD_MMC in 1-bit mode (ESP32-CAM default)
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_DATA);

  // Attempt to initialize SD card
  if (!SD_MMC.begin("/sdcard", true)) { // true = 1-bit mode
    Serial.println("SD Card Mount Failed! Check:");
    Serial.println("- SD card insertion");
    Serial.println("- SD card formatting (FAT32 recommended)");
    Serial.println("- Wiring/pin configuration");
    return;
  }

  // Get SD card type
  uint8_t cardType = SD_MMC.cardType();
  Serial.print("SD Card Type: ");
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  } else if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("Unknown");
  }

  // Get SD card size and usage
  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024); // Size in MB
  uint64_t totalBytes = SD_MMC.totalBytes() / (1024 * 1024); // Total space in MB
  uint64_t usedBytes = SD_MMC.usedBytes() / (1024 * 1024);   // Used space in MB
  uint64_t freeBytes = totalBytes - usedBytes;                // Free space in MB

  Serial.printf("SD Card Size: %llu MB\n", cardSize);
  Serial.printf("Total Space: %llu MB\n", totalBytes);
  Serial.printf("Used Space: %llu MB\n", usedBytes);
  Serial.printf("Free Space: %llu MB\n", freeBytes);

  Serial.println("=== SD Card Test Complete ===");
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}