# ESP32 TextHost

ESP32 TextHost is a self-hosted, Pastebin-like web server running on an ESP32-CAM module with an SD card for text file storage. Users can create, view, edit, copy, and delete text files through a simple web dashboard accessible over Wi-Fi. The dashboard is built with HTML, CSS, and JavaScript, served directly from the SD card, making updates easy without reprogramming the ESP32.

This project repurposes an ESP32-CAM (even with a non-functional camera) to act as a lightweight text-sharing platform, ideal for local or internet-based use.

## Features

- **Web Server**: Runs on ESP32-CAM, accessible via Wi-Fi in Station or Access Point mode.
- **Text File Management**:
  - Create new text files with unique names.
  - List all stored files.
  - View, edit, or delete existing files.
  - Copy file content to clipboard.
- **SD Card Storage**: Stores both the web dashboard (HTML/CSS/JS) and text files on the SD card.
- **Responsive Dashboard**: Mobile-friendly interface for easy text management.
- **Lightweight**: Uses minimal resources, optimized for ESP32-CAM.

## Hardware Requirements

- ESP32-CAM module (SD card reader must be functional; camera not required)
- MicroSD card (formatted as FAT32, up to 32GB recommended)
- USB-to-Serial adapter or development board for programming
- Optional: 5V power supply for standalone operation

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- ESP32 Arduino core: [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
- Libraries:
  - [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) by ESP32Async
  - [AsyncTCP](https://github.com/ESP32Async/AsyncTCP) by ESP32Async
  - `SD_MMC` (included with ESP32 Arduino core)
  - `WiFi` (included with ESP32 Arduino core)

## Installation

### 1. Set Up Arduino IDE
- Install the ESP32 board support:
  - Go to **File > Preferences**, add `https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json` to Additional Boards Manager URLs.
  - In **Tools > Board > Boards Manager**, search for `esp32` and install the latest version (e.g., 2.0.17+).
- Install libraries:
  - In **Sketch > Include Library > Manage Libraries**, search for `ESPAsyncWebServer` and `AsyncTCP`, or download from:
    - [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer)
    - [AsyncTCP](https://github.com/ESP32Async/AsyncTCP)
  - Place them in your Arduino libraries folder (e.g., `~/Arduino/libraries/`).

### 2. Prepare SD Card
- Format the SD card as FAT32 (use a tool like [SD Formatter](https://www.sdcard.org/downloads/formatter/)).
- Create two folders on the SD card root:
  - `/web/`: Copy `index.html`, `styles.css`, and `script.js` from the `web/` folder in this repository.
  - `/pastes/`: Leave empty; the sketch will store text files here.
- Insert the SD card into the ESP32-CAM.

### 3. Configure Sketch
- Open `Text_Host.ino` in Arduino IDE.
- Update Wi-Fi credentials in the sketch:
  ```cpp
  const char* ssid = "YourWiFiSSID";
  const char* password = "YourWiFiPassword";
- Optionally, enable Access Point mode by uncommenting the AP section and commenting out Wi-Fi client code:
  ```cpp

  const char* ap_ssid = "TextHost-AP";
  const char* ap_password = "12345678";
