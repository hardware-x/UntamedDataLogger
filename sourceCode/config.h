#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 0  // Enable (1) or disable (0) serial debug output

#define ONEWIRE_PIN 3  // GPIO pin connected to the DS18B20 1-Wire temperature sensor

#define WIND_SENSOR_I2C_ADDR 0x15  // I2C address of the ULP Wind Sensor PRO
#define WIND_SENSOR_REG_RAW  0x10  // Register to read raw wind speed and direction

#define BATT_PIN 4  // ADC pin connected to the battery voltage divider

#define R1 469700.0  // Resistor R1 (ohms) in battery voltage divider (top)
#define R2 151370.0  // Resistor R2 (ohms) in battery voltage divider (bottom)

#define BATT_READINGS 20  // Number of battery ADC readings to average

#define LOG_BUFFER_SIZE 8192  // Size of log buffer in bytes before flushing to SD

#define IMU_LOG_INTERVAL_MS     100     // Log IMU data every 100ms (10 Hz)
#define WIND_LOG_INTERVAL_MS    1000    // Log wind data every 1000ms (1 Hz)
#define GPS_LOG_INTERVAL_MS     10*1000   // Log GPS data every 10s 
#define TEMP_LOG_INTERVAL_MS    60000   // Log temperature every 60s (1 per minute)
#define LOG_FLUSH_INTERVAL_MS   60000*3   // Flush log buffer to SD every x*m if not full

#define LOG_FILE_DURATION_MINUTES 5  // Duration (in minutes) per log file before rotation

// Wi-Fi transmission interval (just before log file rotation to ensure timely upload)
#define WIFI_TX_INTERVAL_MS (LOG_FILE_DURATION_MINUTES * 60UL * 1000UL - 10000UL)

#define WIFI_SSID "UNTAMED"  // Wi-Fi SSID for data upload
#define WIFI_PASS "UNTAMED4"  // Wi-Fi password for data upload

#define CLOUD_UPLOAD_URL "xxx"  // Endpoint for cloud data upload
#define WIFI_UPLOAD_AUTH "xxx"  // Auth token for cloud uploads

#endif