# SCD41 CO2 sensor (Sensirion) — planned

Chosen as the CO2 sensor. It is photoacoustic (true CO2), unlike
"semiconductor"/MOX modules (e.g. AliExpress JW01-CO2), which only estimate eCO2 from
VOCs and do not follow added CO2.

Datasheets are kept locally, not committed (see `.gitignore`).
Datasheet: https://www.mouser.com/datasheet/3/1278/1/CD_DS_SCD4x_Datasheet_D1.pdf
(covers SCD40/SCD41).

## Mouser options (checked 2026-10-17, qty 1)

| Part | Mouser # | What | Price | Stock |
|---|---|---|---|---|
| SCD41-D-R2 | 403-SCD41-D-R2 | Bare sensor, reel/cut tape, Sensirion | $21.77 | 10,766 (lead time 70 days) |
| SCD40-D-R2 | 403-SCD40-D-R2 | Bare sensor, lower-spec sibling | $18.66 | 1,510 |
| 5190 | 485-5190 | Adafruit SCD-41 breakout, STEMMA QT / Qwiic | $49.95 | 400 |
| SEN-22396 | 474-SEN-22396 | SparkFun SCD41 breakout | $76.88 | 10 |
| SEK-SCD41-Sensor | 403-SEK-SCD41-SENSOR | Sensirion eval board (End of Life; replacement SEK-SCD43-Sensor) | $74.87 | 25 |

The bare SCD41-D-R2 is a surface-mount part (needs a PCB or adapter board); the breakouts
can be wired straight to the header.

## Not yet verified (check against the datasheet before wiring)

- Supply voltage range and I/O levels against this 5 V board and the 3.3 V AHT30/OLED bus.
- I2C address (must not clash with AHT30 0x38 or OLED 0x3C/0x3D) and maximum bus speed
  (bus runs at 333 kHz).
- Peak supply current during a measurement.
- Command set, measurement interval, CRC, and self-calibration (ASC) behaviour.
- For a breakout: its regulator, pull-up resistors and the voltage they pull up to.
