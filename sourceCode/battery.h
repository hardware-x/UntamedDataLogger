#pragma once

#include <Arduino.h>

#define BATT_READINGS 20           // Number of ADC samples to average
#define BATT_PIN 4                 // ADC pin connected to voltage divider

// Measured resistor values in the voltage divider
#define R1 469700.0                // Bottom resistor (GND side)
#define R2 151370.0                // Top resistor (VBAT side)

extern float batteryVoltage;
extern float batteryPercentage;

// ==== Voltage Correction Function ===
// Corrects the measured voltage (from ADC + divider)
// using real-world calibration data (manual piecewise mapping)
float correctVoltage(float measured) {
  struct CorrectionPoint {
    float measured;  // What the system reads (V)
    float real;      // Actual measured voltage (V)
  };

  const CorrectionPoint table[] = {
    {3.41, 3.60},
    {3.53, 3.70},
    {3.65, 3.80},
    {3.78, 3.90},
    {3.91, 4.00},
    {4.07, 4.10},
    {4.21, 4.20}
  };

  const int count = sizeof(table) / sizeof(table[0]);

  if (measured <= table[0].measured) return table[0].real;
  if (measured >= table[count - 1].measured) return table[count - 1].real;

  // Linear interpolation between known data points
  for (int i = 0; i < count - 1; i++) {
    if (measured >= table[i].measured && measured <= table[i + 1].measured) {
      float x1 = table[i].measured;
      float y1 = table[i].real;
      float x2 = table[i + 1].measured;
      float y2 = table[i + 1].real;
      return y1 + (measured - x1) * (y2 - y1) / (x2 - x1);
    }
  }

  return measured;  // Fallback (should not reach here)
}

// ==== Voltage to SoC Mapping ===
// Approximates Li-Ion battery percentage from voltage
float voltageToPercentage(float v) {
  if (v >= 4.20) return 100.0;
  if (v <= 3.60) return 0.0;

  struct VoltageSoC {
    float voltage;
    float percent;
  };

  const VoltageSoC socCurve[] = {
    {4.20, 100.0},
    {4.10, 90.0},
    {4.00, 75.0},
    {3.90, 60.0},
    {3.80, 40.0},
    {3.70, 20.0},
    {3.60, 0.0}
  };

  for (int i = 0; i < sizeof(socCurve)/sizeof(socCurve[0]) - 1; i++) {
    float v1 = socCurve[i].voltage;
    float p1 = socCurve[i].percent;
    float v2 = socCurve[i + 1].voltage;
    float p2 = socCurve[i + 1].percent;

    if (v <= v1 && v >= v2) {
      return p1 + (p2 - p1) * (v - v1) / (v2 - v1);
    }
  }

  return 0.0;
}

// ==== Main Battery Reading Function ===
// Reads and corrects the battery voltage,
// then maps to a usable % range
void readBatteryPercentage() {
  analogSetPinAttenuation(BATT_PIN, ADC_11db);  // Enables 0–3.3V range

  float sumVoltages = 0;

  for (int i = 0; i < BATT_READINGS; i++) {
    int analogValue = analogRead(BATT_PIN);             // 12-bit raw ADC value
    float vOut = (analogValue / 4095.0) * 3.3;           // Convert to voltage at ADC pin
    float vBat = vOut * (R1 + R2) / R1;                  // Reverse divider math
    sumVoltages += vBat;
    delay(1);                                           // Small delay between samples
  }

  float measured = sumVoltages / BATT_READINGS;          // Averaged raw voltage
  batteryVoltage = correctVoltage(measured);             // Calibrated voltage

  batteryPercentage = voltageToPercentage(batteryVoltage); // Map voltage to battery %
  batteryPercentage = constrain(batteryPercentage, 0, 100); // Clamp 0–100%

  if (DEBUG) {
    Serial.print("Battery Voltage: ");
    Serial.println(batteryVoltage, 3);
    Serial.print("Battery %: ");
    Serial.print(batteryPercentage, 1);
    Serial.println("%");
  }
}
