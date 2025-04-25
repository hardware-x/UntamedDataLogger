#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

extern DallasTemperature tempSensor;
extern float temperatureC, temperatureF;

void setupTempSensor() {
  tempSensor.begin();
}

void readTempSensor() {
  tempSensor.requestTemperatures();
  temperatureC = tempSensor.getTempCByIndex(0);
  temperatureF = tempSensor.getTempFByIndex(0);

  #if DEBUG
  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: DS18B20 not responding");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
    Serial.print(temperatureF);
    Serial.println(" °F");
  }
  #endif
}
