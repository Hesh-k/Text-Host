#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD_MMC.h>

// Wi-Fi credentials (update with your network details)
const char* ssid = "Alfa";
const char* password = "1234567777";

// SD card pins for ESP32-CAM
#define SD_MMC_CLK  14
#define SD_MMC_CMD  15
#define SD_MMC_DATA 2

AsyncWebServer server(80);

// Generate a unique filename
String generateFileName() {
  static unsigned long counter = 0;
  return "/paste_" + String(millis()) + "_" + String(counter++) + ".txt";
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("ESP32 TextHost Starting...");

  // Initialize SD card
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_DATA);
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD Card Mount Failed! Check card insertion or format (FAT32).");
    return;
  }
  Serial.println("SD Card Mounted");

  // Create /pastes directory if it doesn't exist
  if (!SD_MMC.exists("/pastes")) {
    SD_MMC.mkdir("/pastes");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWi-Fi Connection Failed!");
    return;
  }
  Serial.println("\nConnected! IP Address: " + WiFi.localIP().toString());

  // Serve static files from SD card
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (SD_MMC.exists("/web/index.html")) {
      request->send(SD_MMC, "/web/index.html", "text/html");
    } else {
      request->send(404, "text/plain", "index.html not found on SD card");
    }
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (SD_MMC.exists("/web/styles.css")) {
      request->send(SD_MMC, "/web/styles.css", "text/css");
    } else {
      request->send(404, "text/plain", "styles.css not found");
    }
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (SD_MMC.exists("/web/script.js")) {
      request->send(SD_MMC, "/web/script.js", "application/javascript");
    } else {
      request->send(404, "text/plain", "script.js not found");
    }
  });

  // List files
  server.on("/files", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "[";
    File root = SD_MMC.open("/pastes");
    if (!root || !root.isDirectory()) {
      SD_MMC.mkdir("/pastes");
      root = SD_MMC.open("/pastes");
    }
    File file = root.openNextFile();
    bool first = true;
    while (file) {
      if (!file.isDirectory()) {
        if (!first) json += ",";
        json += "\"" + String(file.name()) + "\"";
        first = false;
      }
      file = root.openNextFile();
    }
    json += "]";
    request->send(200, "application/json", json);
  });

  // Save file
  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("content", true)) {
      String content = request->getParam("content", true)->value();
      String filename = generateFileName();
      File file = SD_MMC.open("/pastes" + filename, FILE_WRITE);
      if (file) {
        file.print(content);
        file.close();
        request->send(200, "text/plain", "File saved: " + filename);
      } else {
        request->send(500, "text/plain", "Error saving file");
      }
    } else {
      request->send(400, "text/plain", "Missing content");
    }
  });

  // Read file
  server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("name")) {
      String filename = "/pastes/" + request->getParam("name")->value();
      if (SD_MMC.exists(filename)) {
        request->send(SD_MMC, filename, "text/plain");
      } else {
        request->send(404, "text/plain", "File not found");
      }
    } else {
      request->send(400, "text/plain", "Missing filename");
    }
  });

  // Edit file
  server.on("/edit", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("name", true) && request->hasParam("content", true)) {
      String filename = "/pastes/" + request->getParam("name", true)->value();
      String content = request->getParam("content", true)->value();
      if (SD_MMC.exists(filename)) {
        File file = SD_MMC.open(filename, FILE_WRITE);
        if (file) {
          file.print(content);
          file.close();
          request->send(200, "text/plain", "File updated");
        } else {
          request->send(500, "text/plain", "Error updating file");
        }
      } else {
        request->send(404, "text/plain", "File not found");
      }
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });

  // Delete file
  server.on("/delete", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("name", true)) {
      String filename = "/pastes/" + request->getParam("name", true)->value();
      if (SD_MMC.exists(filename)) {
        SD_MMC.remove(filename);
        request->send(200, "text/plain", "File deleted");
      } else {
        request->send(404, "text/plain", "File not found");
      }
    } else {
      request->send(400, "text/plain", "Missing filename");
    }
  });

  // Handle 404
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });

  // Start server
  server.begin();
  Serial.println("Web Server Started");
}

void loop() {
  delay(1000);
}