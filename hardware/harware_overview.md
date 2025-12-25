# Hardware Overview — Vayu-X

This document describes the physical hardware architecture of the Vayu-X autonomous air quality robot, including component selection, power design, and system-level considerations.

---

## 1. Core Processing Unit

### ESP32 (38-pin variant)

The ESP32 serves as the central controller for Vayu-X.

**Responsibilities:**
- Sensor data acquisition
- AQI computation
- Decision logic execution
- Motor and fan control
- OLED display updates
- Wi-Fi Access Point and web server hosting

**Why ESP32:**
- Dual-core MCU suitable for multitasking
- Built-in Wi-Fi eliminates external modules
- Adequate GPIO, ADC, UART support
- Well-suited for real-time embedded applications

---

## 2. Sensor Hardware

### PMS3003 — Particulate Matter Sensor
- Measures PM2.5 and PM10 concentrations
- UART-based digital sensor
- Positioned in the frontal airflow zone
- Uses voltage level protection for ESP32 RX pin

### MQ-135 — Gas Sensor
- Analog output gas sensor
- Used for relative air quality trend detection
- Not used directly in AQI computation
- Placed to avoid direct airflow from the fan

### DHT22 — Temperature & Humidity Sensor
- Digital sensor
- Provides environmental context
- Read at controlled intervals to avoid blocking

### Ultrasonic Sensor (HC-SR04)
- Measures distance for obstacle avoidance
- Provides basic indoor navigation safety
- Polled periodically with reduced timeout

---

## 3. Actuation Hardware

### DC Motors & Drive System
- Differential drive configuration
- Two DC motors with caster wheel support
- Enables forward motion and turning maneuvers

### Motor Driver (TB6612FNG)
- Dual H-bridge motor driver
- Separates motor power from logic power
- Controlled via PWM for speed regulation

### Air Filtration System
- DC fan paired with HEPA filter
- Activated during high AQI conditions
- Operates independently of robot movement

---

## 4. Power Architecture

### Battery System
- 3S battery pack used as primary power source
- Provides sufficient voltage headroom for motors and peripherals

### Voltage Regulation
- Buck converters generate:
  - 5V rail for sensors and fan
  - 3.3V rail for ESP32 and logic components

### Power Design Considerations
- Motors powered separately to reduce noise
- Common ground maintained across system
- Power stability prioritized over compactness

---

## 5. Physical Layout Considerations

- PMS3003 placed at the front for unobstructed air intake
- HEPA filter aligned with airflow direction
- Battery positioned near center for balance
- ESP32 and motor driver placed away from high-current paths

---

## 6. Design Trade-offs

- Chose modular sensors over integrated environmental ICs
- Used deterministic sensors instead of AI-based air analysis
- Prioritized reliability and ease of debugging
- Avoided complex mechanical assemblies

---

## 7. Summary

The hardware architecture of Vayu-X is designed for:
- Stability
- Modularity
- Ease of debugging
- Real-world demo reliability

All components were selected to support deterministic control
and predictable behavior under constrained embedded conditions.
