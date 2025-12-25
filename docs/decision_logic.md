# Decision Logic & Autonomous Behavior — Vayu-X

This document describes the deterministic decision-making logic used by Vayu-X.
All behaviors are implemented using rule-based control and timing guards.
No machine learning or probabilistic models are used.

---

## 1. Control Philosophy

Vayu-X follows a deterministic embedded control model:

- All actions are triggered by sensor thresholds or timed events
- Decisions are explainable and reproducible
- The robot remains fully autonomous without external connectivity
- Safety and predictability are prioritized over adaptiveness

This approach ensures reliable operation on constrained hardware
and simplifies validation during demos and testing.

---

## 2. High-Level Operational States

Although implemented procedurally in firmware, the robot behavior
can be modeled as the following logical states:

### 2.1 State Overview

| State | Description |
|------|-------------|
| INIT | System boot, sensor warm-up, OLED initialization |
| MOVE | Normal navigation and air sampling |
| AVOID_OBSTACLE | Triggered when distance threshold is violated |
| PURIFY | Stationary air purification mode |
| IDLE | Transitional wait state for timing guards |

### 2.2 State Transition Summary

- INIT → MOVE  
  After successful sensor initialization and warm-up

- MOVE → AVOID_OBSTACLE  
  Triggered when ultrasonic distance falls below safety threshold

- AVOID_OBSTACLE → MOVE  
  After turn maneuver and obstacle clearance

- MOVE → PURIFY  
  Triggered when AQI ≥ configured stop threshold

- PURIFY → MOVE  
  Triggered when AQI falls below threshold for a sustained duration

- Any State → IDLE  
  Used internally for timing guards and stabilization

---

## 3. Sensor Inputs Driving Decisions

| Sensor | Role in Decision Logic |
|------|------------------------|
| PMS3003 | Primary AQI computation (PM2.5, PM10) |
| MQ-135 | Gas trend awareness (non-AQI, relative) |
| DHT22 | Environmental context (display & logging) |
| Ultrasonic | Obstacle detection and avoidance |

---

## 4. AQI-Based Behavior Logic

AQI is computed using US EPA breakpoints for PM2.5 and PM10.

### 4.1 AQI Thresholds

| AQI Range | Category | Behavior |
|---------|----------|----------|
| 0–50 | Good | Normal movement |
| 51–100 | Moderate | Continue movement, increased monitoring |
| ≥101 | Unhealthy or worse | Enter purification mode |

---

## 5. Purification Mode Logic

When AQI exceeds the configured threshold:

- Robot immediately halts movement
- DC motors are disabled
- Fan + HEPA filter are activated
- Robot remains stationary
- AQI is re-evaluated at fixed intervals

Exit condition:
- AQI falls below the stop threshold for a sustained duration

This prevents oscillation between movement and purification.

---

## 6. Obstacle Avoidance Logic

Obstacle avoidance is always active, regardless of AQI state.

- Ultrasonic distance is sampled periodically
- If distance < safe threshold:
  - Forward motion stops
  - Turn maneuver is executed
  - Control returns to MOVE state

Obstacle logic has higher priority than navigation.

---

## 7. Timing Guards & Non-Blocking Design

To ensure stability:

- Sensor reads are time-gated
- OLED updates are rate-limited
- Motor commands use minimum activation durations
- No blocking delays are used in the main loop

This allows:
- Smooth multitasking
- Predictable response times
- Stable Wi-Fi and web server operation

---

## 8. Fail-Safe Behavior

- Invalid PMS frames are discarded
- Missing sensor updates do not crash the system
- Default behavior favors stopping over unsafe motion
- Monitoring interface is read-only and non-intrusive

---

## 9. Why No ML or AI

Machine learning was intentionally excluded because:

- Decisions are threshold-based and interpretable
- Training data is environment-specific
- ML adds complexity without functional benefit
- Deterministic logic is easier to test and justify

Vayu-X is designed as a reliable embedded system,
not a predictive model.

---

## 10. Summary

The decision logic of Vayu-X is:

- Explicit
- Predictable
- Robust
- Fully explainable

All behaviors observed during demos
are direct results of these rules.
