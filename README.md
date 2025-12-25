# Vayu-X

Vayu-X is an ESP32-based autonomous mobile robot designed to monitor indoor air quality and actively improve it using deterministic, real-time decision logic.

The system combines mobility, environmental sensing, and closed-loop actuation to detect air pollution and respond autonomously without relying on cloud services or machine learning models.

---

## Overview

Indoor air pollution often goes unnoticed until it begins affecting health.  
Most existing solutions are either passive monitors or stationary air purifiers.

Vayu-X addresses this gap by:
- autonomously navigating indoor spaces
- continuously sampling air quality
- activating an onboard filtration system when pollution exceeds safe limits

The robot operates fully independently and does not require external connectivity.

---

## System Highlights

- Autonomous indoor navigation with obstacle avoidance  
- Real-time air quality monitoring (PM, gas concentration, temperature, humidity)  
- AQI computation based on **US EPA standards**  
- Closed-loop purification (detect → act → verify)  
- Deterministic, explainable embedded control logic  
- Optional local monitoring via Wi-Fi (read-only)

---

## System Architecture & Design

Detailed documentation for each subsystem is provided below:

- 📐 **System Architecture**  
  [`docs/system_architecture.md`](docs/system_architecture.md)

- 🧠 **Decision Logic & Autonomous Behavior**  
  [`docs/decision_logic.md`](docs/decision_logic.md)

- ⚙️ **Hardware Architecture & Power Design**  
  [`hardware/hardware_overview.md`](hardware/hardware_overview.md)

- 📱 **Android Monitoring App (Optional, Read-Only)**  
  [`app/android/README.md`](app/android/README.md)

- 🎥 **Demo Media & Prototype Images**  
  [`demo/README.md`](demo/README.md)

---

## Software Design Philosophy

Vayu-X intentionally avoids AI/ML-based decision-making.

Instead, it uses:
- threshold-based AQI logic
- state-driven autonomous behavior
- timing guards and non-blocking execution

This approach was chosen to ensure:
- predictable behavior
- explainable decisions
- reliability on constrained embedded hardware

---

## Current Status

- Fully functional autonomous prototype  
- Demonstrated in an offline evaluation environment  
- All core subsystems operational and integrated  

---

## Limitations

- Indoor-only operation  
- Fixed AQI thresholds  
- No long-term data storage or cloud integration  

---

## Future Improvements

- Adaptive AQI thresholds  
- Improved localization and navigation  
- Enhanced data logging for long-term analysis  
- Refined mechanical enclosure  

---

## Development Context

This project was developed by **Team Vayu-X Innovators** as part of a national-level technical competition and evaluated through live, offline demonstrations.

---

## License

This project is intended for academic and educational use.
