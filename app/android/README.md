# Android Monitoring App — Vayu-X

This directory documents the optional Android application developed to monitor Vayu-X in real time.

The robot is **fully autonomous** and does not depend on the app for any functionality.

---

## Purpose of the App

The Android app serves as a **read-only monitoring interface** for Vayu-X.

It allows users to:
- View live air quality data
- Understand robot behavior and environment state
- Observe AQI trends during operation

No control commands are sent to the robot.

---

## Communication Model

- ESP32 runs in **Wi-Fi Access Point (AP) mode**
- User connects phone directly to the ESP32 network
- ESP32 hosts a lightweight HTTP server
- App fetches data from a JSON endpoint


Endpoint:
GET /data

## Data Exposed by ESP32

The following data is provided via the `/data` endpoint:

- AQI (numerical)
- Air quality category
- PM2.5 and PM10 values
- Gas level indicator (MQ-135)
- Temperature
- Humidity

All values are generated internally by the robot firmware.

---

## Design Constraints

The following constraints were enforced by design:

- Read-only communication
- No motor or fan control
- No override of autonomous logic
- No cloud or internet dependency
- Portrait-only orientation
- Local-only connectivity

These constraints ensure system safety and predictability.

---

## Technology Stack

- Android Studio
- Kotlin
- HTTP JSON polling
- No external cloud services

---

## Rationale for Read-Only Design

The app was intentionally limited to monitoring to:

- Preserve autonomous integrity
- Avoid unsafe remote overrides
- Reduce system complexity
- Keep embedded logic authoritative

This separation ensures the robot remains reliable
even if the app disconnects or fails.

---

## Summary

The Android app enhances visibility into Vayu-X operation
without influencing or altering its behavior.

It is an accessory module designed for demonstration,
analysis, and understanding of the system.
