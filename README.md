# ESP32-S3 Emergency Alert System

An IoT emergency tracking and alert system built with ESP32-S3, featuring multi-sensor alert triggers, virtualized LoRa/GPS telemetry, and visual/audible feedback.

## Features
- **Multi-Source Alerting:** Manual panic button, high-impact motion detection, and sound threshold triggering.
- **Visual Status:** Green LED remains solid during normal operation and blinks rapidly during active alerts.
- **Audible Signal:** Integrated buzzer for immediate local emergency notification.
- **Telemetry Processing:** Simulates LoRa network payload delivery and GPS location tracking.

## Pin Mapping
- **Potentiometer (Mic Simulation):** GPIO 1
- **Emergency Button:** GPIO 3
- **Green Status LED:** GPIO 4
- **Buzzer:** GPIO 5
- **MPU6050 (I2C):** SDA (GPIO 8), SCL (GPIO 9)

## How to Run in Wokwi
1. Open Wokwi and select an ESP32-S3 board setup.
2. Replace `sketch.ino` with the code in `emergency_system.ino`.
3. Replace `diagram.json` with the layout contents provided in `diagram.json`.
4. Run the simulation and interact with the button, potentiometer, or IMU to trigger alerts.
