/*
  ------------------------------------------------------------------------------
  UNTAMED DATALOGGER — Firmware v0.4
  ------------------------------------------------------------------------------
  Developed by HardwareX Engineering LTD
  📍 CYENS Centre of Excellence | 🔗 www.hardwarex.cy | ✉️ contact@hardwarex.cy
  ------------------------------------------------------------------------------
  ABOUT HARDWAREX
  HardwareX is a deep-tech R&D firm specializing in custom embedded systems,
  industrial IoT, and TinyML. With proven experience in on-device AI, low-power
  electronics, and advanced manufacturing, we deliver innovative hardware
  tailored for extreme environments and high-impact applications.

  HardwareX leads the development of intelligent, production-ready embedded
  solutions for research, environmental sensing, and industrial systems across
  Europe.

  ------------------------------------------------------------------------------
  PROJECT: UNTAMED — Wind & Motion Environmental Data Logger
  ------------------------------------------------------------------------------
  The UNTAMED logger is a ruggedized, low-power, field-deployable platform
  designed to collect synchronized environmental and motion data using:
    - 9-DoF IMU (ICM-20948)
    - Ultrasonic wind sensor (I2C)
    - DS18B20 temperature probe (1-Wire)
    - GPS positioning (PA1010D)
    - Battery voltage monitor
    - 128x128 OLED Display for field feedback

  Data is stored in SD logs and optionally uploaded via WiFi to a remote
  cloud endpoint. Logging includes timestamps from a real-time clock (DS3231),
  synchronized with GPS when possible.

  ------------------------------------------------------------------------------
  FIRMWARE STRUCTURE
  ------------------------------------------------------------------------------
  • UntamedFirmware.ino  - Main loop and core orchestrator
  • config.h             - Pinouts, intervals, and global constants
  • display.h            - OLED graphics and status display
  • imu.h                - ICM-20948 motion sensor interface
  • temp.h               - DS18B20 temperature sensor logic
  • gps.h                - Adafruit GPS NMEA parsing & RTC sync
  • wind.h               - Ultrasonic wind sensor handler
  • battery.h            - Battery monitoring and percentage calc
  • logger.h             - Sensor data logging, file rotation
  • uplink.h             - WiFi/HTTP data upload routines
  • globals.h            - Definitions of shared global variables

  This modular design ensures clarity, testability, and future scalability.

  ------------------------------------------------------------------------------
*/

#include "config.h"
#include "imu.h"
#include "temp.h"
#include "gps.h"
#include "wind.h"
#include "battery.h"
#include "display.h"
#include "logger.h"
#include "uplink.h"
#include "globals.h"
#include <BDL.h>

BDL dbl;  // Required to call dbl.begin(); — this powers I2C LDO2 rail

void setup() {
  #if DEBUG
  Serial.begin(115200);
  #endif

  setCpuFrequencyMhz(80);  // Lower CPU speed for power saving (options: 240, 160, 80, 40)

  dbl.begin();              // Power up sensors & OLED
  delay(1000);
  Wire.setClock(100000);    // Set I2C speed

  setupDisplay();
  delay(200);               // OLED boot delay
  setupIMU();
  setupGPS();
  setupTempSensor();
  setupWindSensor();
  setupRTC();
  setupLogFile();

  renderIntroDisplay();

  // Initial readings before loop
  readIMU();
  readTempSensor();
  readGPS();
  readWindSensor();
  readBatteryPercentage();

  disableRadios(); // Start in low-power mode
}

unsigned long lastShortRefresh = 0;
unsigned long lastLongRefresh = 0;

void loop() {
  unsigned long now = millis();

  // Refresh GPS, battery, and WiFi upload every 200ms
  if (now - lastShortRefresh >= 200) {
    readGPS();
    lastShortRefresh = now;
  }

  // Refresh display every 950ms
  if (now - lastLongRefresh >= 950) {
    renderDisplay();
    handleWiFiUpload();
    readBatteryPercentage();
    lastLongRefresh = now;
  }

  logAllSensors();
  delay(40);
}
