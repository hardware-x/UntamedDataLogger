#pragma once

#include <Adafruit_GPS.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <Adafruit_SH110X.h>
#include <FS.h>

// External declarations for globals
extern Adafruit_ICM20948 icm;
extern Adafruit_Sensor *icm_temp, *icm_accel, *icm_gyro, *icm_mag;
extern OneWire oneWire;
extern DallasTemperature tempSensor;
extern Adafruit_GPS GPS;
extern Adafruit_SH1107 display;
extern RTC_DS3231 rtc;

extern File logFile;
extern char logFileName[32];
extern const char* footer;
extern char logBuffer[2048];
extern size_t logIndex;
extern unsigned long lastFlush;
extern unsigned long lastIMULog;
extern unsigned long lastWindLog;
extern unsigned long lastGPSLog;
extern unsigned long lastTempLog;
extern unsigned long lastWiFiSend;
extern unsigned long currentLogStartTime;
extern int uploadSuccessCount;
extern bool currentLogUploaded;
extern bool rtcSyncedWithGPS;

extern float temperatureC, temperatureF;
extern float windSpeed, windDirection;
extern uint8_t windBuffer[8];
extern float batteryVoltage, batteryPercentage;
extern sensors_event_t accel, gyro, temp, mag;
