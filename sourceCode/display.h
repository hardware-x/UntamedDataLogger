#pragma once

#include <Adafruit_SH110X.h>
#include "config.h"

extern Adafruit_SH1107 display;
extern const char* footer;
extern float batteryPercentage;
extern float windSpeed, windDirection;
extern float temperatureC;
extern sensors_event_t mag;
extern unsigned long currentLogStartTime;
extern int uploadSuccessCount;
extern Adafruit_GPS GPS;

void setupDisplay() {
  display.begin(0x3D, true);
  display.setRotation(0);
  display.setTextWrap(false);
  display.clearDisplay();
  display.display();
}

void renderIntroDisplay() {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  const char* line1 = "UNTAMED LOGGER";
  const char* line2 = "DATA LOGGER V0.4";

  display.setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(line1, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 30);
  display.print(line1);

  display.getTextBounds(line2, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 42);
  display.print(line2);

  display.getTextBounds(footer, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 80);
  display.print(footer);

  display.display();
  delay(5000);
  display.clearDisplay();
  display.display();
}

void renderDisplay() {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  char buffer[32];
  int16_t x1, y1;
  uint16_t w, h;

  // Battery percentage and voltage inline (centered)
  display.setTextSize(2);
  snprintf(buffer, sizeof(buffer), "%d%% %.2fV", (int)batteryPercentage, batteryVoltage);
  display.getTextBounds(buffer, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 0);
  display.print(buffer);

  // Define vertical layout
  const int VERTICAL_PIX_OFFSET = 22;
  const int VERTICAL_PIX_INC = 12;
  int verticalIncCounter = 0;

  display.setTextSize(1);

  // Time
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", GPS.hour, GPS.minute, GPS.seconds);
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  display.print("TIME: ");
  display.print(buffer);
  verticalIncCounter++;

  // Wind
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  display.print("WIND: ");
  snprintf(buffer, sizeof(buffer), "%.1fm/s %.0f", windSpeed, windDirection);
  display.print(buffer); display.print((char)248); // degree symbol
  verticalIncCounter++;

  // GPS status
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  snprintf(buffer, sizeof(buffer), "GPS: %s Q:%d", GPS.fix ? "OK" : "X ", GPS.fix ? GPS.fixquality : 0);
  display.print(buffer);
  verticalIncCounter++;

  // Speed
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  snprintf(buffer, sizeof(buffer), "SPD : %.1fkn", GPS.speed);
  display.print(buffer);
  verticalIncCounter++;

  // Heading
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  snprintf(buffer, sizeof(buffer), "GHEAD: %.0f", GPS.angle);
  display.print(buffer); display.print((char)248);
  verticalIncCounter++;

  // Magnetometer heading
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  float magHeading = atan2(mag.magnetic.y, mag.magnetic.x) * 180.0 / PI;
  if (magHeading < 0) magHeading += 360.0;
  snprintf(buffer, sizeof(buffer), "MHEAD: %.0f", magHeading);
  display.print(buffer); display.print((char)248);
  verticalIncCounter++;

  // External temperature
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    snprintf(buffer, sizeof(buffer), "TEMP: %.1fC", temperatureC);
  } else {
    snprintf(buffer, sizeof(buffer), "TEMP: --.-C");
  }
  display.print(buffer);
  verticalIncCounter++;

  // Upload countdown
  display.setCursor(0, VERTICAL_PIX_OFFSET + verticalIncCounter * VERTICAL_PIX_INC);
  unsigned long elapsedSinceStart = millis() - currentLogStartTime;
  unsigned long timeUntilTx = (WIFI_TX_INTERVAL_MS > elapsedSinceStart) ? (WIFI_TX_INTERVAL_MS - elapsedSinceStart) : 0;
  snprintf(buffer, sizeof(buffer), "UP:%d | TX IN: %lus", uploadSuccessCount, timeUntilTx / 1000);
  display.print(buffer);
  verticalIncCounter++;

  // Footer
  display.getTextBounds(footer, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 120);
  display.print(footer);

  display.display();
}
