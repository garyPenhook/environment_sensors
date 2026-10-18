
# pic_test

PIC18F27Q10 at 5 V, HFINTOSC 16 MHz. Timer0 interrupt toggles RC2 every 500 ms and
triggers a once-a-second read of a WeAct AHT30 temperature/humidity module over MSSP1
I2C (RC3 = SCL1, RC4 = SDA1, SMBus input levels). Each reading is shown on a WeAct 0.96"
SSD1315 OLED on the same bus (temperature, humidity, sensor status). Unused pins are
driven low; RB6/RB7 (ICSP) are left as inputs.

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
