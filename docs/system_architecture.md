# System Architecture — Vayu-X

This document describes the high-level system architecture of the Vayu-X autonomous air quality robot, focusing on how hardware, firmware, and control logic interact as a unified system.

---

## Architectural Overview

Vayu-X follows a **layered embedded system architecture** designed for reliability, real-time responsiveness, and explainability.

The system can be logically divided into four layers:

1. Sensing Layer  
2. Processing & Decision Layer  
3. Actuation Layer  
4. Monitoring & Interface Layer  

Each layer operates independently but communicates through well-defined data flows.

---

## 1. Sensing Layer

The sensing layer is responsible for acquiring real-world environmental data.

### Sensors Used
- **PMS3003**  
  Measures particulate matter concentrations (PM2.5, PM10) via UART communication.

- **MQ-135**  
  Provides analog gas concentration readings for general air quality estimation.

- **DHT22**  
  Measures ambient temperature and humidity.

- **Ultrasonic Sensor (HC-SR04)**  
  Provides distance measurements for obstacle detection and navigation.

### Design Considerations
- Sensors are polled in a **non-blocking manner**
- PMS3003 uses buffered UART frame parsing
- DHT22 readings are rate-limited to avoid blocking delays

---

## 2. Processing & Decision Layer

The ESP32 acts as the central processing unit for the system.

### Responsibilities
- Sensor data aggregation
- AQI computation (US EPA standard)
- Decision-making based on pollution thresholds
- Navigation and motion decisions
- Web server handling for external monitoring

### AQI Logic
- AQI is calculated independently for PM2.5 and PM10
- The higher AQI value determines the system state
- Threshold-based logic ensures deterministic behavior

This approach was chosen over ML models to guarantee:
- Predictable execution time
- Explainable decisions
- Reliability on constrained hardware

---

## 3. Actuation Layer

The actuation layer physically interacts with the environment.

### Components
- **DC Motors + Motor Driver**
  - Enables autonomous movement
  - Supports forward motion and turning maneuvers

- **Fan + HEPA Filter**
  - Activated when AQI exceeds defined thresholds
  - Provides active air purification

### Control Strategy
- Motor updates are time-controlled (non-blocking)
- Obstacle avoidance overrides normal navigation
- High pollution levels trigger a stationary purification mode

---

## 4. Monitoring & Interface Layer

This layer provides visibility into system state and sensor data.

### OLED Display
- Displays AQI, PM values, temperature, humidity, and system status
- Updated periodically to avoid blocking the main loop

### Wi-Fi & Web Interface
- ESP32 operates in Wi-Fi Access Point mode
- Exposes a lightweight JSON endpoint at `/data`
- Enables external monitoring via a mobile application

The robot remains fully autonomous and operational without any external interface.

---

## Data Flow Summary

1. Sensors capture environmental data  
2. ESP32 processes readings and computes AQI  
3. Decision logic determines movement and purification actions  
4. Actuators execute decisions  
5. System state is displayed locally and exposed via JSON  

---

## Architectural Principles

- Deterministic control over probabilistic models
- Non-blocking execution for real-time safety
- Separation of concerns between sensing, logic, and actuation
- Minimal external dependencies

---

## Conclusion

The architecture of Vayu-X emphasizes robustness, clarity, and real-world deployability.  
By combining autonomous mobility with closed-loop air purification, the system demonstrates a practical embedded solution to indoor air quality monitoring and improvement.
