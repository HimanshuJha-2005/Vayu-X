# Vayu-X

Vayu-X is an ESP32-based autonomous mobile robot designed to monitor indoor air quality and actively improve it using real-time decision logic.

---

## Overview

Indoor air pollution often goes unnoticed until it begins affecting health. Most available solutions are either passive monitors or stationary air purifiers.

Vayu-X addresses this gap by combining **mobility**, **environment sensing**, and **closed-loop actuation** into a single autonomous system.

The robot continuously navigates an indoor environment, detects air quality degradation, and activates an onboard filtration system when pollution exceeds safe limits.

---

## System Highlights

- Autonomous indoor navigation with obstacle avoidance  
- Real-time air quality monitoring (PM, gas concentration, temperature, humidity)  
- AQI computation based on US EPA standards  
- Closed-loop air purification (detect → act → verify)  
- Deterministic embedded control logic for reliability  

---

## Hardware Architecture

**Core Controller**
- ESP32

**Sensors**
- PMS3003 – particulate matter (PM2.5, PM10)
- MQ-135 – gas concentration
- DHT22 – temperature and humidity
- Ultrasonic sensor – obstacle detection

**Actuation**
- DC motors with motor driver
- Fan with HEPA filtration unit

**Power**
- Battery-powered embedded system with onboard regulation

---

## Software Design

The system follows a deterministic, state-based control approach suitable for real-time embedded environments.

Key design decisions:
- Threshold-based AQI logic instead of ML models
- Non-blocking sensor polling
- Predictable behavior under constrained resources

This ensures explainability, reliability, and consistent performance during autonomous operation.

---

## Autonomous Behavior

- Continuous environmental sampling while navigating
- AQI-based decision making
- Automatic activation of filtration system when air quality degrades
- Reduced movement and focused sampling in high-pollution zones

---

## Current Status

- Fully functional autonomous prototype
- Tested and demonstrated in an offline competition environment

---

## Limitations

- Indoor-only operation
- Fixed AQI thresholds
- No cloud or long-term data storage

---

## Future Improvements

- Adaptive AQI thresholds
- Enhanced localization
- Mobile application for live monitoring
- Long-term data logging and analytics

---

## Competition Context

This project was developed and demonstrated by **Team Vayu-X Innovators** as part of a national-level technical competition.
