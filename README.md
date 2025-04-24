# UntamedDataLogger

A robust ESP32-S3-based battery-powered data logging system designed by HardwareX Engineering LTD in collaboration with the UNTAMED team for long-term environmental monitoring. It supports wind speed/direction sensing, GPS tracking, IMU data acquisition, and temperature logging. The system features an OLED interface, onboard SD card logging, and Wi-Fi upload functionality.

---

## 📂 Repository Structure

- **/sourceCode**  
  Contains the full Arduino-based firmware for the ESP32-S3, including sensor drivers, logging routines, Wi-Fi upload, and display rendering logic.

- **/CAD_Files**  
  3D printable and manufacturing-ready enclosure models, mounting inserts, and internal mechanical layout designed for robustness and modular sensor attachment.

- **/images**  
  Reference visuals showing the system’s internal assembly, deployment configuration, live OLED interface, and sensor connectivity.

---

## 🔧 Features

- ESP32-S3 with optimized power draw (~100 mA average)
- GPS (PA1010D) with 6 Hz update rate
- Ultra-low power wind meter (1 Hz)
- 9-DoF IMU (ICM-20948)
- Waterproof DS18B20 temperature sensor
- 1.12" 128x128 OLED display with live stats
- Wi-Fi upload via configurable hotspot
- SD card logging with timestamped CSV entries
- Voltage calibration for precise battery percentage display
- Safe cutoff at 3.60 V for stable operation
- Compatible with 10050 mAh Li-ion batteries
- USB-C port for firmware updates and charging

---

## 🛠️ Getting Started

1. Connect sensors and battery as per provided CAD layout.
2. Flash the firmware via the **red USB-C** using the `/sourceCode` directory.
3. Power on using the **metal pushbutton**.
4. Ensure Wi-Fi hotspot is named `UNTAIMED` with password `UNTAIMED4` for upload to function.
5. Monitor real-time readings on the OLED screen.

---

## ⚙️ Calibration & Accuracy

Voltage readings are corrected with a linear mapping based on precision measurements (±0.02 V accuracy). This ensures accurate battery level tracking and protects the system from brownouts.

---

## 📸 Visual Reference

See the `/images` folder for setup examples, internal wiring, and field deployment configurations.

---

## 🔐 License

This project is licensed under the **MIT License**. See the `LICENSE` file for full terms.

---

**P.S.** If you're working on rugged outdoor IoT deployments or need tailored embedded design—feel free to reach out!
