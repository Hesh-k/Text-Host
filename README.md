# Text-Host

# ESP32 Pastebin

A self-hosted Pastebin-like web server running on an ESP32 with an SD card for text file storage. Users can create, view, edit, and copy text files via a web dashboard accessible over Wi-Fi. The dashboard is built with HTML, CSS, and JavaScript, served directly from the SD card.

## Features

- **Web Server**: ESP32 acts as a web server, accessible locally or over the internet.
- **Text File Storage**: Save and manage text files on the SD card.
- **Web Dashboard**: Simple interface to create, view, edit, and copy text files.
- **SD Card Hosted UI**: HTML, CSS, and JS files are stored on the SD card, making updates easy.
- **Wi-Fi Support**: Operates in Access Point (AP) or Station mode.

## Hardware Requirements

- ESP32-CAM module (camera not required, but SD card reader must work)
- MicroSD card (formatted as FAT32)
- USB-to-Serial adapter for programming (if not using a dev board)
- Optional: 5V power supply for standalone operation

## Software Requirements

- Arduino IDE or PlatformIO
- ESP32 Arduino core (`espressif/arduino-esp32`)
- Libraries:
  - `ESPAsyncWebServer`
  - `AsyncTCP`
  - `SD` (or `SD_MMC`)
  - `WiFi`

## Installation

1. **Set Up Arduino IDE**:
   - Install the ESP32 board support via Boards Manager.
   - Install required libraries using the Library Manager or manually.

2. **Prepare SD Card**:
   - Format the SD card as FAT32.
   - Copy the `web` folder (containing `index.html`, `styles.css`, `script.js`) to the SD card root.
   - Insert the SD card into the ESP32-CAM.

3. **Configure Sketch**:
   - Open `esp32_pastebin.ino` in Arduino IDE.
   - Update Wi-Fi credentials (for Station mode) or AP settings in the sketch:
     ```cpp
     const char* ssid = "YourWiFiSSID";
     const char* password = "YourWiFiPassword";
