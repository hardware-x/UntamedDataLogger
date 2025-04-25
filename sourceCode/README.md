# UNTAMED Embedded Firmware

**Project UNTAMED** is an environmental and motion monitoring system developed by HardwareX Engineering for the "UNTAMED" https://untamed.team/ initiative. This firmware powers the UNTAMED system and orchestrates sensor sampling, data logging, real-time monitoring, and efficient wireless data uplinking — optimized for embedded deployment in harsh and remote environments.

---

## 📦 System Overview

This firmware runs on an **ESP32-S3 Mini** board and manages the following components:

- **ICM-20948 9-DoF IMU** (acceleration, gyro, magnetometer)
- **DS18B20 Waterproof Temperature Sensor**
- **PA1010D GPS Module**
- **ULP Wind Sensor Pro (Ultrasonic)**
- **128x128 SH1107 OLED Display**
- **BQ24074 Power Path Charger (with dynamic system load)**
- **10050 mAh ICR18650 Li-ion battery**

---

## 🚀 Features

- High-resolution data logging with time-synced IMU, GPS, temperature, and wind data
- SD card logging in efficient buffered format
- Automatic log file rotation every 5 minutes
- Display interface showing GPS fix, battery %, system status, wind and motion
- Accurate battery voltage and SoC calculation (with correction and curve-fitting)
- Automatic WiFi-based log upload (when enabled)
- Power optimizations (CPU throttling, radio disablement)
- RTC synchronization via GPS
- Robust boot and I2C handling

---

## 📁 File Structure

| File               | Description |
|--------------------|-------------|
| `UntamedFirmware.ino` | Main firmware entry point |
| `battery.h`        | Voltage divider reading, battery % curve, calibration |
| `display.h`        | OLED display rendering functions |
| `gps.h`            | GPS setup and read logic |
| `logger.h`         | Logging to SD, log file management, and formatting |
| `uplink.h`         | WiFi data upload routines |
| `globals.cpp`      | Global variable definitions and config includes |
| `config.h`         | WiFi credentials, timing constants, cloud settings |

---

## 🔋 Battery Monitoring and SoC Estimation

The system reads the battery voltage using a calibrated voltage divider and compensates for ESP32-S3 ADC error. A curve-fitting method maps the measured voltage (after compensation) to real-world battery state-of-charge, based on empirical measurements using precision lab equipment. The system considers 3.60V as the minimum operating threshold.

---

## ⏱ Power Optimization

- Radios disabled when idle (`disableRadios()`)
- CPU frequency scaling supported via `setCpuFrequencyMhz()`
- ADC averaging with calibrated offset and slope correction
- Ultra-lightweight display refresh logic

---

## 📊 Logging Format

Each log file is stored with the following format:
timestamp,type,data...
I: AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,MagX,MagY,MagZ
G: Latitude,Longitude,Speed,Course
W: WindSpeed,WindDirection
T: ExternalTempC

Data is flushed every 2 minutes or when buffer is full. Logs are uploaded periodically via WiFi.

---

## 🛠 Build & Flashing

1. Use PlatformIO or Arduino IDE
2. Target Board: `ESP32-S3 Dev Module`
3. Dependencies:
   - `Adafruit_Sensor`
   - `Adafruit_ICM20948`
   - `Adafruit_GFX`, `Adafruit_SH110X`
   - `TinyGPS++`, `OneWire`, `DallasTemperature`
   - `SD`, `WiFi`, `HTTPClient`, `RTClib`

---

## 🧪 Testing Notes

- Verified ADC calibration across voltages 3.4–4.2V
- Real-time graphs generated to validate voltage-to-SoC mapping
- GPS sync verified in both standalone and logged modes
- Tested with Nordic Power Profiler Kit for current draw profiling

---

## 🌐 Server Uploads

The system optionally uploads log snapshots and full logs to a configured backend. It sends full `.log` files using HTTP with custom headers and basic authorization.

---

## 📄 Licensing

This firmware is released under the MIT License.

---


