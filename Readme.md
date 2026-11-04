# LightCube

An 8×8×8 (512 LED) volumetric LED cube controlled by an ESP32.

This project uses hierarchical KiCad schematics, nine 74HC595 shift registers, TIP127 Darlington transistors for layer driving, and an I2S digital microphone for audio-reactive effects. The cube is designed to display 3D animations and respond to sound.

I made this project as part of Half Life YSWS by Hack Club to learn about LED multiplexing, shift registers, and volumetric displays.

## How it works

- 8 layers of 8×8 LEDs (total 512 LEDs)
- 8 × 74HC595 control the 64 columns
- 1 × 74HC595 + 8 × TIP127 control the 8 layers (cathodes)
- ESP32 drives everything over SPI and runs the animations
- I2S MEMS microphone (pins: VDD, GND, WS, SCK, SD) for future audio-reactive modes

The ESP32 rapidly scans through the 8 layers while updating the column data, creating the illusion of a full 3D volume using persistence of vision.

## Schematics

Root hierarchy:

![Root Hierarchy](images/root_page.png)

One of the LED matrix layers:

![LED Matrix](images/mat1.png)

Main components page (shift registers, ESP32, microphone, power):

![Components](images/all_components.png)

Row control + TIP127 drivers:

![Row Control](images/row_ctrl_ic.png)

ESP32 section:

![ESP32](images/esp32.png)

Microphone & Power:

![Mic and Power](images/mic_and_pwr.png)

## CAD

3D printed enclosure files are available in the `CAD/` folder.

## Firmware

Firmware is written in Arduino (C++) for ESP32.  
it is purely vibecoded rn and not tested.
See the `firmware/` folder or the main `.ino` file.

## Current Status

- Full hierarchical schematics completed
- Mechanical CAD completed
- Basic firmware written
- PCB layout still pending