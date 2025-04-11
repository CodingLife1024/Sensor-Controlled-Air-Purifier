# AC Fan Speed Control Module (TRIAC-based)

## Overview
This module implements **phase-angle control** for AC fan speed regulation using zero-cross detection and TRIAC triggering. It currently exhibits timing instability when run alongside other system components, due to blocking delays and lack of timer-based control.

## Hardware Setup

### Critical Components
| Component           | GPIO Pin | Function                                |
|---------------------|----------|-----------------------------------------|
| Zero-Cross Detector | GPIO D2  | Detects 0V crossing of 50/60Hz AC wave  |
| TRIAC Gate          | GPIO D3  | Fires TRIAC to control AC phase angle   |

## Code Overview

### Blocking Delay Method (Basic Implementation)

```cpp
void IRAM_ATTR zeroCrossInterrupt() {
    zeroCrossDetected = true; // Minimal ISR
}

void loop() {
    if (zeroCrossDetected) {
        zeroCrossDetected = false;
        delayMicroseconds(dimmingTime);       // Delay to shift phase angle
        digitalWrite(dimmerPin, HIGH);        // Trigger TRIAC
        delayMicroseconds(50);                // Maintain gate pulse
        digitalWrite(dimmerPin, LOW);         // End trigger
    }
}
```

## Speed Control Logic

### Principle
This system uses **phase-angle dimming**, where the TRIAC is triggered at a controlled delay after each zero-crossing, allowing only part of each AC half-cycle to power the fan.

---

### Timing Diagram (50Hz AC)



```
Half-Cycle Timing Diagram (50Hz AC)
------------------------------------
0µs                        10000µs
│────────────┬───────────────│
│   DELAY    │  TRIAC CONDUCTS  │
│            └──▶ Trigger Point
│
└── Zero Crossing Detection
```

1. **Zero-Cross Detection**  
   The interrupt triggers at each AC waveform's zero-crossing point (0V transition).

2. **Delay Calculation**  
   `dimmingTime = (10000µs × (100 - dimmingLevel)) / 100`  
   Example at 50% level:  
   `(10000µs × 50) / 100 = 5000µs delay`

3. **Power Delivery**  
   | Dimming Level | Approx Voltage | Fan Speed |
|---------------|----------------|-----------|
| 0%            | 0V             | Off       |
| 30%           | ~70V           | Low       |
| 70%           | ~170V          | Medium    |
| 100%          | 230V           | Full      |

⚠️ Induction fans may stall below 30%, and show **non-linear response** between 30–70%.

---

## Issues in Current Implementation

### 1. Blocking Delays
- `delayMicroseconds()` halts **all operations**, including interrupt handling.
- Delays longer than 10ms (one half-cycle at 50Hz) may **miss zero-cross events**.

### 2. Interrupt Starvation
- No queuing or debouncing of zero-cross events.
- Under CPU load, critical TRIAC trigger timing may be skipped.

### 3. Timing Jitter
- Software delay jitter: ±5–10µs.
- Optocoupler-based detectors introduce additional ±100–200µs jitter.

### 4. Weak State Handling
- Global flag `zeroCrossDetected` is **vulnerable to race conditions**.
- Assumes constant 50Hz frequency; no adaptive timing compensation.

---

## Safety Considerations

⚠️ **High Voltage Handling**
- Use **optoisolated zero-cross detectors**.
- Ensure **galvanic isolation** between AC and logic circuits.
- Add **snubber networks** to protect TRIAC from inductive loads.
- Use **UL-rated fire-retardant enclosures** for AC portions.
- Follow **local electrical safety regulations**.
- Add **fuses and thermal cutoffs** to limit damage in fault conditions.

---

## Recommended Enhancements

### Hardware
- Replace software delay with **hardware PWM timer**.
- Add **RC filter** on zero-cross signal to reduce bounce.
- Consider dedicated phase-angle control ICs (e.g., **TCA785**, or **MOC3021 + BT136**).

---

## Improved Code (Using Hardware Timer)

```cpp
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer(){
    digitalWrite(dimmerPin, HIGH);
    delayMicroseconds(50); // Gate pulse duration
    digitalWrite(dimmerPin, LOW);
}

void IRAM_ATTR zeroCrossInterrupt() {
    timerAlarmWrite(timer, dimmingTime, false); // µs-level accuracy
    timerRestart(timer);  // Non-blocking trigger delay
}
```

## Diagnostic Features to Add
- Phase delay measurement (debug timing accuracy)
- Fan current sensor to detect stalls or overload
- TRIAC temperature monitoring
- Misfire counters and automatic retry mechanisms
- Real-time sync verification with zero-cross signal

## Performance Metrics
| Parameter          | Current Code | Recommended Target |
|--------------------|--------------|--------------------|
| Timing Accuracy    | ±50µs        | ±2µs               |
| Max Interrupt Latency | 10µs      | <1µs               |
| Misfire Rate       | 15%          | <0.1%              |

## Expansion Capabilities
1. Add PID feedback loop
2. Implement soft-start functionality
3. Enable power factor correction
4. Add RS485 MODBUS interface
5. Develop thermal derating profiles