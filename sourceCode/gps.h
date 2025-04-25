#pragma once

#include <Adafruit_GPS.h>
#include "RTClib.h"

extern Adafruit_GPS GPS;
extern RTC_DS3231 rtc;
extern bool rtcSyncedWithGPS;

void setupGPS() {
  GPS.begin(0x10);  // I2C address of GPS module
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPS.println(PMTK_Q_RELEASE);
}

void readGPS() {
  const int MAX_CHARS = 100;
  int charsRead = 0;

  while (charsRead < MAX_CHARS) {
    char c = GPS.read();
    if (!c) break;
    #if DEBUG
    Serial.print(c);
    #endif
    charsRead++;

    if (GPS.newNMEAreceived()) {
      char nmea[120];
      strncpy(nmea, GPS.lastNMEA(), sizeof(nmea));
      nmea[sizeof(nmea) - 1] = '\0';

      if (GPS.parse(nmea)) {
        #if DEBUG
        Serial.println("\n--- GPS DATA PARSED ---");
        Serial.printf("Time: %02d:%02d:%02d\n", GPS.hour, GPS.minute, GPS.seconds);
        Serial.printf("Date: %02d/%02d/20%02d\n", GPS.day, GPS.month, GPS.year);
        Serial.printf("Fix: %d | Quality: %d\n", GPS.fix, GPS.fixquality);

        if (GPS.fix) {
          Serial.printf("Location: %.4f%c, %.4f%c\n", GPS.latitude, GPS.lat, GPS.longitude, GPS.lon);
          Serial.printf("Speed (knots): %.2f\n", GPS.speed);
          Serial.printf("Angle: %.2f\n", GPS.angle);
          Serial.printf("Altitude: %.2f\n", GPS.altitude);
          Serial.printf("Satellites: %d\n", GPS.satellites);
        }
        #endif

        if (GPS.fix && !rtcSyncedWithGPS) {
          rtc.adjust(DateTime(2000 + GPS.year, GPS.month, GPS.day,
                              GPS.hour, GPS.minute, GPS.seconds));
          rtcSyncedWithGPS = true;
          #if DEBUG
          Serial.println("RTC synced with GPS time.");
          #endif
        }

        #if DEBUG
        Serial.println("------------------------\n");
        #endif
      } else {
        #if DEBUG
        Serial.println("Failed to parse NMEA sentence.");
        #endif
      }
    }
  }
}
