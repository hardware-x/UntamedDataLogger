#pragma once

#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include "logger.h"

extern int uploadSuccessCount;
extern bool currentLogUploaded;
extern char logFileName[32];
extern unsigned long lastWiFiSend;

void disableRadios() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();
}

void sendLogToServer() {
  HTTPClient http;
  http.begin(CLOUD_UPLOAD_URL);

  http.addHeader("Content-Type", "application/octet-stream");
  http.addHeader("Authorization", WIFI_UPLOAD_AUTH);
  http.addHeader("X-Filename", "snapshot.log");

  char payload[256];
  snprintf(payload, sizeof(payload),
    "%lu,S,%.1f,%.0f,%.5f,%.5f,%.2f,%.0f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
    getCurrentTimestamp(),
    temperatureC,
    batteryPercentage,
    GPS.latitude,
    GPS.longitude,
    GPS.speed,
    GPS.angle,
    accel.acceleration.x,
    accel.acceleration.y,
    accel.acceleration.z,
    gyro.gyro.x,
    gyro.gyro.y,
    gyro.gyro.z
  );

  #if DEBUG
  Serial.print("Uploading snapshot: ");
  Serial.println(payload);
  #endif

  int httpResponseCode = http.POST((uint8_t*)payload, strlen(payload));

  #if DEBUG
  if (httpResponseCode > 0) {
    Serial.print("Snapshot sent. HTTP ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Snapshot upload failed. HTTP ");
    Serial.println(httpResponseCode);
  }
  #endif

  http.end();
}

void handleWiFiUpload() {
  if (millis() - lastWiFiSend > WIFI_TX_INTERVAL_MS && !currentLogUploaded) {
    #if DEBUG
    Serial.println("WIFI_TX_INTERVAL_MS Passed. Attempting upload.");
    #endif

    lastWiFiSend = millis();

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
      delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
      #if DEBUG
      Serial.println("Connected. Uploading latest file...");
      #endif

      HTTPClient http;
      http.begin(CLOUD_UPLOAD_URL);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("Authorization", WIFI_UPLOAD_AUTH);
      http.addHeader("X-Filename", logFileName + 1); // remove leading '/'

      File f = SD.open(logFileName);
      if (f) {
        int httpResponseCode = http.sendRequest("POST", &f, f.size());
        #if DEBUG
        Serial.printf("HTTP %d\n", httpResponseCode);
        #endif
        if (httpResponseCode > 0) {
          uploadSuccessCount++;
          currentLogUploaded = true;
        }
        http.end();
        f.close();
      } else {
        #if DEBUG
        Serial.println("Failed to open log file for upload");
        #endif
      }
    } else {
      #if DEBUG
      Serial.println("WiFi failed");
      #endif
    }

    disableRadios();
  }
}
