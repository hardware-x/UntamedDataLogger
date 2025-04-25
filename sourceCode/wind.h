#pragma once

#include <Wire.h>

#define WIND_SENSOR_I2C_ADDR 0x15
#define WIND_SENSOR_REG_RAW  0x10

extern float windSpeed;
extern float windDirection;
extern uint8_t windBuffer[8];

void setupWindSensor() {
  #if DEBUG
  Serial.println("Initializing Wind Sensor...");
  #endif
}

bool readWindSensor() {
  Wire.beginTransmission(WIND_SENSOR_I2C_ADDR);
  Wire.write(WIND_SENSOR_REG_RAW);
  if (Wire.endTransmission() != 0) {
    #if DEBUG
    Serial.println("Wind sensor I2C write failed.");
    #endif
    return false;
  }

  Wire.requestFrom(WIND_SENSOR_I2C_ADDR, (uint8_t)7);
  unsigned long start = millis();
  while (Wire.available() < 7) {
    if (millis() - start > 100) {
      #if DEBUG
      Serial.println("Wind sensor I2C read timeout.");
      #endif
      return false;
    }
  }

  for (int i = 0; i < 7; i++) {
    windBuffer[i] = Wire.read();
  }

  windSpeed = ((windBuffer[2] << 8) | windBuffer[3]) / 100.0;
  windDirection = ((windBuffer[4] << 8) | windBuffer[5]) / 100.0;

  if (windSpeed == 0) {
    windDirection = -1.0;
  }

  #if DEBUG
  if (windSpeed == 0) {
    Serial.println("Wind: 0.00 m/s (No wind)");
  } else {
    Serial.print("Wind: ");
    Serial.print(windSpeed, 2);
    Serial.print(" m/s  Dir: ");
    Serial.print(windDirection, 2);
    Serial.println("°");
  }
  #endif

  return true;
}
