# Automatic Plant Irrigation System with ESP32

## Project Description

This project is an automatic irrigation system for a plant based on an **ESP32-WROOM-32** microcontroller and a **Capacitive Soil Moisture Sensor v2.0**. The system continuously monitors the soil condition and automatically activates a **12V mini peristaltic pump** when the humidity drops below a defined threshold.

The ESP32 reads the analog output of the soil moisture sensor, converts the raw value into an estimated humidity percentage, and decides whether the pump should be turned on or off. A MOSFET module is used to switch the pump safely, while an external **12V / 2A DC power supply** provides the energy required by the pump.

The control logic uses two separate thresholds in order to avoid unstable rapid switching. The pump starts when the soil humidity falls below **35%** and stops when the humidity rises above **45%**. This hysteresis-based approach makes the irrigation process more stable and more reliable for practical use.

The project was developed in **Arduino IDE** and represents a simple smart irrigation prototype that can be extended in the future with mobile monitoring features. The current version focuses on automatic soil monitoring, automatic watering, and serial debug feedback for testing and calibration.

---

## Project Photo

> Add your project image in the repository and update the path below if needed.

![Project Setup](docs/project-photo.jpg)

---

## Schematics

The schematic must include all hardware components used in the project. The wires should be clearly separated, should not overlap as much as possible, and should use different colors so that the connections are easy to understand.

### Wiring Description

#### 1. Soil Moisture Sensor - Controller
- **AUTO** - yellow wire - **D34**
- **VCC** - red wire - **3V3**
- **GND** - black wire - **GND**

#### 2. MOSFET - Controller
- **GND** - black wire - **GND**
- **TRIG/PWM** - white wire - **D26**

#### 3. Power Module - MOSFET
- **- module** - red wire - **VIN-**
- **+ module** - brown wire - **VIN+**
- The **1000µF electrolytic capacitor** is connected in parallel on the VIN input.

#### 4. MOSFET - Pump
- **VOUT-** - red wire - **pump-**
- **VOUT+** - orange wire - **pump+**
- The pump protection stage includes a **1N5819 Schottky diode**.

### Functional Overview

- The **soil moisture sensor** measures the humidity level in the soil.
- The **ESP32** reads the sensor value through GPIO34.
- The **MOSFET module** receives the control signal from GPIO26.
- The **external 12V power supply** powers the pump circuit.
- The **peristaltic pump** moves water through the tubing when irrigation is needed.
- The **capacitor** helps stabilize the power input.
- The **Schottky diode** provides protection in the pump power stage.

---

## Pre-requisites

### Hardware Components

1. **ESP32 Development Board, ESP-WROOM-32, 30 pins, USB Type-C**  
   Specification: [ESP-WROOM-32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)

2. **Capacitive Soil Moisture Sensor v2.0**  
   Specification: [Capacitive Soil Moisture Sensor v2.0 Datasheet](https://rajguruelectronics.com/Product/5538/Capacitive%20Soil%20Moisture%20Sensor%20V2%281%29.0.pdf)

3. **Mini Peristaltic Pump, 12V, 5W**  
   Representative specification page: [12V DC Peristaltic Pump Example Specification](https://grobotronics.com/peristaltic-liquid-pump-12v-dc.html?sl=en)

4. **MOSFET Switching Module**  
   Used to control the 12V pump from the ESP32 digital output.

5. **12V 2A DC Power Supply for CCTV Systems**  
   Representative specification page: [12V 2A CCTV Adapter Example Specification](https://uk.annke.com/products/12v-2a-power-adapter)

6. **5.5 x 2.1 mm Female DC Connector with Screw Terminals**  
   Specification: [5.5x2.1 mm Female Terminal Block Connector](https://www.techly.com/dc-power-adapter-5-5x2-1mm-female-terminal-block-2-pin.html)

7. **Schottky Rectifier Diode - STMicroelectronics 1N5819, 40V, 1A**  
   Specification: [STMicroelectronics 1N5819](https://www.st.com/en/diodes-and-rectifiers/1n5819.html)

8. **Electrolytic Capacitor - SAMXON KM 1000U/25V, 1000µF, 25V, 105°C**  
   Specification: [SAMXON KM1000/25 Specification](https://www.tme.eu/en/details/km1000_25/tht-electrolytic-capacitors/samxon/ekm108m1eg16rrs0p/)

9. **Jumper Wires / Dupont Wires**

10. **Flexible Water Tubes**

11. **Plant Pot and Soil for Testing**

12. **USB Type-C Cable for Programming**

---

### Software Components

1. **Arduino IDE**  
   Official page: [Arduino IDE](https://www.arduino.cc/en/software)

2. **ESP32 Board Package for Arduino IDE**  
   Official documentation: [Arduino-ESP32 Installation Guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

---

## Setup and Build

### 1. Install Arduino IDE
Download and install Arduino IDE from the official page:

- [Arduino IDE Download](https://www.arduino.cc/en/software)

### 2. Install ESP32 Board Support
Open **Arduino IDE** and go to:

- **File -> Preferences**
- In **Additional Boards Manager URLs**, add:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
