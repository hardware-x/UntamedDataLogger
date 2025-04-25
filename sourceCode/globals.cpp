#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <Adafruit_SH110X.h>
#include <FS.h>
#include "config.h"

Adafruit_ICM20948 icm;
Adafruit_Sensor *icm_temp, *icm_accel, *icm_gyro, *icm_mag;
OneWire oneWire(3);
DallasTemperature tempSensor(&oneWire);
Adafruit_GPS GPS(&Wire);
Adafruit_SH1107 display = Adafruit_SH1107(128, 128, &Wire);
RTC_DS3231 rtc;

// Logging-related globals
File logFile;
char logFileName[32];
const char* footer = "POWERED BY HARDWAREX";
char logBuffer[LOG_BUFFER_SIZE];
size_t logIndex = 0;
unsigned long lastFlush = 0;
unsigned long lastIMULog = 0;
unsigned long lastWindLog = 0;
unsigned long lastGPSLog = 0;
unsigned long lastTempLog = 0;
unsigned long lastWiFiSend = 0;
unsigned long currentLogStartTime = 0;
int uploadSuccessCount = 0;
bool currentLogUploaded = false;
bool rtcSyncedWithGPS = false;

// Sensor readings
float temperatureC = 0, temperatureF = 0;
float windSpeed = 0.0, windDirection = -1.0;
uint8_t windBuffer[8];
float batteryVoltage = 0, batteryPercentage = 0;
sensors_event_t accel, gyro, temp, mag;
