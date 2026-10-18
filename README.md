# pic_test

A small temperature and humidity monitor on a PIC18F27Q10. Once a second it reads a
WeAct AHT30 sensor and shows the temperature, relative humidity and sensor status on a
WeAct 0.96" OLED. Both modules share one I2C bus. Written in C for XC8.

## Parts

| Part | Role |
|------|------|
| PIC18F27Q10 (28-pin), 5 V, HFINTOSC 16 MHz | Microcontroller |
| WeAct AHT30 module (AHTxx V1.0) | Temperature / humidity sensor, I2C 0x38, on-board 3.3 V LDO and 4.7 kohm pull-ups |
| WeAct 0.96" OLED module V1.0 | 128x64 display, SSD1315 controller, I2C 0x3C |
| PICkit 4 | Programming and debugging (ICSP on RB6/RB7) |

An SCD41 CO2 sensor is planned; see `docs/scd41`.

## What it does

- Timer0 interrupts every 20 ms. RC2 toggles every 500 ms (heartbeat) and every 50 ticks
  (1 s) a sample is triggered.
- The AHT30 measurement is non-blocking: start a conversion, do other work, fetch the
  result about 120 ms later and check its CRC.
- Temperature and humidity are shown in large text with a status line (OK, bus error, busy,
  CRC error). Only the characters that changed are redrawn, with a full redraw about every
  minute. A display that was unplugged or glitched is re-initialised on the next sample.
- A failed I2C transaction runs a bus clear (clock SCL until a stuck slave releases SDA,
  then STOP) so one hung device cannot block the other.

## Wiring

| Signal | Pin |
|--------|-----|
| I2C SCL (MSSP1) | RC3 |
| I2C SDA (MSSP1) | RC4 |
| Heartbeat toggle | RC2 |
| ICSP clock / data | RB6 / RB7 (left as inputs) |

The bus is pulled up to 3.3 V on the modules, so the PIC only ever pulls SCL/SDA low and
uses SMBus input levels. Unused pins are driven low.

## Structure

| Path                        | Purpose                                                                                                                             |
|-----------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
| _build                      | The [CMake build tree](https://cmake.org/cmake/help/latest/manual/cmake.1.html#introduction-to-cmake-buildsystems), can be deleted. |
| cmake                       | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added                                        |
| .vscode                     | See [VSCode](https://code.visualstudio.com/docs/getstarted/settings)                                                                |
| .vscode/settings.json       | Workspace specific settings                                                                                                         |
| .vscode/pic_test.mplab.json | The MPLAB project file, should not be deleted                                                                                       |
| out                         | Final build artifacts                                                                                                               |
| src                         | C sources: `main.c` (clock, GPIO, Timer0, main loop), `i2c1.c` (MSSP1 master), `aht30.c` (sensor driver), `oled.c` (SSD1315 text driver) |
| inc                         | Headers for the sources above, plus `board.h` (`_XTAL_FREQ`)                                         |
| docs/aht30                  | AHT30 and module datasheets, schematic, WeAct example, and a README of verified facts and voltages   |
| docs/oled                   | README of verified SSD1315 / WeAct OLED facts (datasheets kept local, not committed)                 |
| docs/scd41                  | Planned SCD41 CO2 sensor: Mouser part options and facts still to verify (datasheets kept local)      |
