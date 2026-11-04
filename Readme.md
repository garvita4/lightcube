# LED Cube
An 8×8×8 (512 LED) volumetric LED cube driven by an ESP32, 74HC595 shift registers, and TIP127 Darlington drivers. The design uses hierarchical KiCad schematics with one sheet per layer (LED_L0–LED_L7) plus a shared components sheet.

## Features & Hardware Design
8×8×8 LED Matrix
Eight independent 8×8 layers (LED_L0 through LED_L7).

Each layer is a full matrix of discrete LEDs with column anodes and a common cathode per layer.

Total: 512 LEDs.
Column Drivers (74HC595)
Eight 74HC595 serial-in/parallel-out shift registers (IC1–IC8) provide the 64 column lines.

They are daisy-chained and controlled via SPI (SER, SRCLK, RCLK, OE) from the ESP32.
Row / Layer Drivers
One additional 74HC595 drives eight TIP127 PNP Darlington transistors.

These switch the common cathode of each layer (L0_A … L7_A), enabling classic multiplexed scanning of the cube.
Microcontroller – ESP32
ESP32-DevKitC (or equivalent) is the main controller.

## It handles:

SPI communication to all shift registers
Multiplexing timing
Pattern / animation generation
Optional audio-reactive effects via the onboard microphone

- Microphone
A simple electret / MEMS microphone input is provided for sound-reactive visualisations.
Power
- 5 V input via barrel jack / power connector with local bulk and decoupling capacitors.

- All logic and LED drive runs from the 5 V rail (with appropriate current handling for the TIP127 stage).

## Schematic Hierarchy
textledcube (root)
├── LED_L0 … LED_L7   → individual 8×8 matrices (identical topology, different net labels)
└── components        → all drivers, ESP32, MIC, power

## LED_Lx sheets: identical 8×8 LED matrices (only reference designators and hierarchical labels change).
components sheet: 74HC595s, TIP127 row drivers, ESP32, microphone, and power input.


## Full Schematics
(Place exported PDF / PNG renders of the hierarchical sheets here)

## Root hierarchy
LED_L0 … LED_L7 (one example + note that the rest are identical)
Components page (drivers + ESP32 + power)

Current status: Schematics only.

PCB layout and mechanical CAD are not yet started.

How the Multiplexing Works (short)

ESP32 shifts a 64-bit column pattern into the eight 74HC595s.
One layer is selected by driving the corresponding TIP127 low (common cathode).
The pattern is displayed briefly.
Next layer is selected and the process repeats at a high enough rate for persistence of vision.

This classic layer-by-layer scan keeps pin count and current requirements manageable while still allowing full 3-D voxel control.

# Bill of Materials (preliminary – from schematic)


# Firmware Status
No firmware has been written yet.

The hardware is designed for straightforward SPI-driven multiplexing + optional microphone sampling, so any existing open-source 8×8×8 LED cube firmware (or a clean ESP-IDF / Arduino implementation) can be adapted once the board is fabricated.