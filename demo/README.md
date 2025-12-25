# Demo Media — Vayu-X

This section provides visual evidence of the working Vayu-X prototype.

All images shown here correspond to the final, demo-tested system and its iterative development stages.

---

## Prototype Overview

![Vayu-X Full View](robot_full_view.jpg)

Autonomous mobile robot with onboard sensing, actuation, power management, and air filtration integrated into a single platform.

---

## Electronics & Control System

![Electronics Top View](electronics_view.jpg)

ESP32-based control system with integrated sensor interfaces, motor driver connections, and power regulation modules.

This layout reflects a functional prototype focused on reliability and rapid iteration rather than enclosure aesthetics.

---

## Air Filtration Unit

![HEPA Filter Unit](hepa_filter_unit.jpg)

Fan-driven HEPA filtration unit used for active air purification when AQI exceeds defined thresholds.

**Note on sensor placement:**  
During later iterations, the PMS3003 particulate sensor was repositioned closer to the HEPA filtration unit after adding a protective cardboard enclosure.  
This change was made to improve airflow consistency and protect the sensor during operation.

---

## Live System Feedback

![OLED Display](oled_display.jpg)

Onboard OLED display providing real-time feedback including AQI, particulate values, temperature, humidity, and system status.

---

## Obstacle Detection

![Ultrasonic Sensor Front View](ultrasonic_front.jpg)

Front-mounted ultrasonic sensor used for obstacle detection and basic indoor navigation safety.

Obstacle avoidance logic operates independently of air quality state.
