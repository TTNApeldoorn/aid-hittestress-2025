# HittseStress2025 PCB Design

This directory contains the KiCad project files for the HittseStress2025 printed circuit board (PCB). This PCB is designed to interface the TTGO T-Beam ESP32 LoRa module with the SPS30 particulate matter sensor and other components for the climate sensor system.

## Contents

- `HittseStress2025.kicad_pcb` - PCB layout file
- `HittseStress2025.kicad_sch` - Schematic file
- `HittseStress2025.kicad_pro` - KiCad project file
- `gerbers/` - Gerber files for PCB manufacturing
- `my-KiCad-library/` - Custom KiCad symbols and footprints
- `HittseStress2025-backups/` - Automatic backup files

## Purpose

The PCB serves as a shield/breakout board that provides:
- Power management connections
- Interface connections for the SPS30 particulate matter sensor
- Proper pin routing for the ESP32-based TTGO T-Beam module
- Support for battery charging and power supply from the lamppost

This board enables the climate sensor to be mounted in lampposts and operate on battery power with charging capability.
