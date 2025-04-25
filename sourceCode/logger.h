#pragma once

#include <SD.h>
#include <RTClib.h>
#include "Adafruit_GPS.h"
#include "config.h"
#include "globals.h"

void logHeader() {
  logFile = SD.open(logFileName, FILE_APPEND);
  if (logFile) {
    logFile.println("# RobinLog Format");
    logFile.println("# timestamp,type,data...");
    logFile.println("# I: AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,MagX,MagY,MagZ");
    logFile.println("# G: Latitude,Longitude,Speed,Course");
    logFile.println("# W: WindSpeed,WindDirection");
    logFile.println("# T: ExternalTempC");
    logFile.println("");
    logFile.close();
  }
}

void flushLogBuffer() {
  if (logIndex == 0) return;

  logFile = SD.open(logFileName, FILE_APPEND);
  if (logFile) {
    logFile.write((uint8_t*)logBuffer, logIndex);
    logFile.close();
  }

  logIndex = 0;
  lastFlush = millis();
}

void appendToLogBuffer(const char* line) {
  size_t len = strlen(line);
  if (logIndex + len >= LOG_BUFFER_SIZE) {
    flushLogBuffer();
  }

  strncpy(&logBuffer[logIndex], line, LOG_BUFFER_SIZE - logIndex - 1);
  logIndex += len;
}

void setupLogFile() {
  if (!SD.begin()) {
    #if DEBUG
    Serial.println("SD card init failed!");
    #endif
    return;
  }

  randomSeed(esp_random());
  int randNum = random(10000, 99999);

  snprintf(logFileName, sizeof(logFileName), "/LOG_%d.log", randNum);
  #if DEBUG
  Serial.print("Log file: ");
  Serial.println(logFileName);
  #endif

  File testFile = SD.open(logFileName, FILE_WRITE);
  if (testFile) {
    // testFile.println("=== UNTAMED LOGGER START ===");
    // testFile.close();
    // logHeader();
  } else {
    #if DEBUG
    Serial.println("Failed to create log file!");
    #endif
  }
}

void setupRTC() {
  if (!rtc.begin()) {
    #if DEBUG
    Serial.println("Couldn't find RTC");
    #endif
    while (1) delay(10);
  }
}

uint32_t getCurrentTimestamp() {
  return rtc.now().unixtime();
}

// Optionally call this from readGPS() when GPS.fix is true
void syncRTCWithGPS() {
  if (GPS.fix && !rtcSyncedWithGPS) {
    rtc.adjust(DateTime(
      2000 + GPS.year,
      GPS.month,
      GPS.day,
      GPS.hour,
      GPS.minute,
      GPS.seconds
    ));
    rtcSyncedWithGPS = true;
    #if DEBUG
    Serial.println("RTC synced with GPS time.");
    #endif
  }
}

void updateLogFileIfNeeded() {
  unsigned long nowMs = millis();
  if (nowMs - currentLogStartTime >= LOG_FILE_DURATION_MINUTES * 60UL * 1000UL) {
    flushLogBuffer();

    if (!SD.begin()) {
      Serial.println("SD card failed while updating log file.");
      return;
    }

    DateTime now = rtc.now();
    int flooredMin = (now.minute() / 5) * 5;
    snprintf(logFileName, sizeof(logFileName), "/%04d%02d%02d%02d%02d.log",
             now.year(), now.month(), now.day(), now.hour(), flooredMin);

    Serial.print("New log file: ");
    Serial.println(logFileName);

    logFile = SD.open(logFileName, FILE_WRITE);
    if (logFile) {
      // logFile.println("=== UNTAMED LOGGER START ===");
      // logFile.close();
      // logHeader();
    }

    currentLogStartTime = nowMs;
    currentLogUploaded = false;
  }
}

void logAllSensors() {
  char line[128];
  unsigned long now = millis();

  updateLogFileIfNeeded();

  // === IMU Logging ===
  if (now - lastIMULog >= IMU_LOG_INTERVAL_MS) {
    readIMU();
    lastIMULog = now;

    snprintf(line, sizeof(line),
      "%lu,I,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.1f,%.1f,%.1f\n",
      getCurrentTimestamp(),
      accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
      gyro.gyro.x, gyro.gyro.y, gyro.gyro.z,
      mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);
    appendToLogBuffer(line);
  }

  // === Wind Logging ===
  if (now - lastWindLog >= WIND_LOG_INTERVAL_MS) {
    readWindSensor();
    lastWindLog = now;

    snprintf(line, sizeof(line),
      "%lu,W,%.2f,%.0f\n",
      getCurrentTimestamp(), windSpeed, windDirection);
    appendToLogBuffer(line);
  }

  // === GPS Logging ===
  if (GPS.fix && now - lastGPSLog >= GPS_LOG_INTERVAL_MS) {
    lastGPSLog = now;

    snprintf(line, sizeof(line),
      "%lu,G,%.5f,%.5f,%.2f,%.0f\n",
      getCurrentTimestamp(), GPS.latitude, GPS.longitude, GPS.speed, GPS.angle);
    appendToLogBuffer(line);
  }

  // === Temp Logging ===
  if (now - lastTempLog >= TEMP_LOG_INTERVAL_MS) {
    readTempSensor();
    lastTempLog = now;

    snprintf(line, sizeof(line),
      "%lu,T,%.1f\n", getCurrentTimestamp(), temperatureC);
    appendToLogBuffer(line);
  }

  if (now - lastFlush > LOG_FLUSH_INTERVAL_MS || logIndex > LOG_BUFFER_SIZE - 150) {
    flushLogBuffer();
  }
}
