#pragma once

#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>

extern Adafruit_ICM20948 icm;
extern Adafruit_Sensor *icm_temp, *icm_accel, *icm_gyro, *icm_mag;
extern sensors_event_t accel, gyro, temp, mag;

void setupIMU() {
  if (!icm.begin_I2C()) {
    #if DEBUG
    Serial.println("Failed to find ICM20948 chip");
    #endif
    while (1) delay(10);
  }

  #if DEBUG
  Serial.println("ICM20948 Found!");
  #endif

  icm_temp = icm.getTemperatureSensor();
  icm_accel = icm.getAccelerometerSensor();
  icm_gyro = icm.getGyroSensor();
  icm_mag = icm.getMagnetometerSensor();

  #if DEBUG
  icm_temp->printSensorDetails();
  icm_accel->printSensorDetails();
  icm_gyro->printSensorDetails();
  icm_mag->printSensorDetails();
  #endif
}

void readIMU() {
  icm_temp->getEvent(&temp);
  icm_accel->getEvent(&accel);
  icm_gyro->getEvent(&gyro);
  icm_mag->getEvent(&mag);

  #if DEBUG
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.printf("Accel X: %.2f\tY: %.2f\tZ: %.2f m/s²\n",
                accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);

  Serial.printf("Gyro  X: %.2f\tY: %.2f\tZ: %.2f rad/s\n",
                gyro.gyro.x, gyro.gyro.y, gyro.gyro.z);

  Serial.printf("Mag   X: %.2f\tY: %.2f\tZ: %.2f uT\n",
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);
  #endif
}
