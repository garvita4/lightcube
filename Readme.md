# LightCube

![Hero](images/root_page.png)

An **8×8×8 (512 LED)** volumetric LED cube driven by an ESP32, nine 74HC595 shift registers, and TIP127 Darlington drivers.  
Features hierarchical KiCad schematics, full mechanical CAD, and an I2S digital microphone for audio-reactive effects.

---

## Features & Hardware Design

### 8×8×8 LED Matrix
Eight independent 8×8 layers (LED_L0 → LED_L7).  
Each layer is a full matrix of discrete LEDs.  
**Total: 512 LEDs**.

![LED Matrix Example](images/mat1.png)

### Column Drivers
Eight 74HC595 shift registers provide the 64 column lines.  
Daisy-chained and controlled via SPI from the ESP32.

![All ICs](images/all_ic.png)

### Row / Layer Drivers
One additional 74HC595 drives eight **TIP127** PNP Darlington transistors.  
These switch the common cathode of each layer for classic multiplexed scanning.

![Row Control](images/row_ctrl_ic.png)

### Microcontroller – ESP32
ESP32-DevKitC handles:
- SPI communication to all shift registers
- High-speed multiplexing
- Pattern & animation generation
- Audio-reactive effects via I2S microphone

![ESP32](images/esp32.png)

### Digital Microphone (I2S)
5-pin I2S MEMS microphone (INMP441 style)  
**Pins:** `VDD`, `GND`, `WS`, `SCK`, `SD`

![Mic & Power](images/mic_and_pwr.png)

### Power
5 V input with local bulk and decoupling capacitors.

---

## Schematic Hierarchy
ledcube (root)
├── LED_L0 ... LED_L7   → individual 8×8 matrices
└── components          → drivers, ESP32, MIC, power


![Root Hierarchy](images/root_page.png)

### LED Layer Sheets
All eight LED layers are identical in topology (only net labels differ):

| Layer | Image |
|-------|-------|
| LED_L0 | ![mat1](images/mat1.png) |
| LED_L1 | ![mat2](images/mat2.png) |
| LED_L2 | ![mat3](images/mat3.png) |
| LED_L3 | ![mat4](images/mat4.png) |
| LED_L4 | ![mat5](images/mat5.png) |
| LED_L5 | ![mat6](images/mat6.png) |
| LED_L6 | ![mat7](images/mat7.png) |
| LED_L7 | ![mat8](images/mat8.png) |

### Components Page
Full driver, MCU, microphone and power section:

![All Components](images/all_components.png)

---

## Mechanical Design (CAD)

The cube is housed in a custom 3D-printed enclosure.

**Files included:**
CAD/
├── design/
│   └── final.stl
└── productions/
├── cover.stl
├── holding plate.stl
└── main bodyu.stltext


---

## How the Multiplexing Works

1. ESP32 shifts a 64-bit column pattern into the eight 74HC595s.
2. One layer is selected by driving the corresponding TIP127.
3. Pattern is displayed briefly.
4. Next layer is selected → process repeats at high speed for persistence of vision.

This classic layer-by-layer scan keeps pin count and current requirements manageable while allowing full 3-D voxel control.

---

## Bill of Materials

| Part                        | Description                                      | Qty | Notes |
|-----------------------------|--------------------------------------------------|-----|-------|
| LED                         | 5 mm Clear / Diffused LED                        | 512 | Choose consistent colour & brightness |
| 74HC595                     | 8-bit Shift Register                             | 9   | 8 column + 1 row |
| TIP127                      | PNP Darlington Transistor (TO-220)               | 8   | Layer drivers |
| ESP32-DevKitC               | Main MCU                                         | 1   | - |
| I2S MEMS Microphone         | Digital Mic (VDD, GND, WS, SCK, SD)              | 1   | INMP441 style |
| Capacitors                  | 100 nF + bulk (10–100 µF)                        | ~25 | Decoupling + power |
| Resistors                   | Assorted (base, pull-up, etc.)                   | ~40 | - |
| Power Connector             | 5.5×2.1 mm DC jack or screw terminal             | 1   | 5 V input |

> Detailed `bom.csv` is included in the repository.

---

## Firmware Status

Firmware is currently under development.  
The hardware is designed for straightforward SPI-driven multiplexing + I2S audio input, so existing open-source 8×8×8 LED cube firmware can be adapted relatively easily.

---

## Project Status

- [x] Full hierarchical schematics
- [x] Mechanical CAD (STL files)
- [ ] PCB layout
- [ ] Firmware
- [ ] First physical prototype

---


