# SCD41 CO2 sensor (Sensirion) — planned

Chosen as the CO2 sensor. It is photoacoustic (true CO2), unlike
"semiconductor"/MOX modules (e.g. AliExpress JW01-CO2), which only estimate eCO2 from
VOCs and do not follow added CO2.

Datasheets are kept locally, not committed (see `.gitignore`).
Datasheet (SCD4x v1.5, July 2023, covers SCD40/SCD41):
https://sensirion.com/media/documents/48C4B7FB/64C134E7/Sensirion_SCD4x_Datasheet.pdf
(Mouser's copy returns a web page, not the PDF, when fetched by a script.)

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

## Verified against the SCD4x datasheet v1.5

- I2C address 0x62 (table 8, p.7). No clash with AHT30 0x38, OLED 0x3C/0x3D or a 24LC256
  at 0x50-0x57. Max SCL 400 kHz (table 7, p.6), so the 333 kHz bus is fine.
- Supply VDD 2.4-5.5 V, typical 3.3 or 5.0 V (table 4, p.4). VDDH (IR source) must be
  tied to VDD. Absolute max on SDA/SCL is VDD + 0.3 V; VDD abs max 6.0 V (table 5).
- Input levels: VIH = 0.65 x VDD, VIL = 0.3 x VDD (table 4). The microcontroller must
  only drive SDA/SCL low (p.5), which matches the open-drain bus code in `i2c1.c`.
  Example pull-up is 10 kohm; the existing 4.7 kohm modules' pull-ups are within that
  spirit but bus capacitance still decides (p.5).
- Supply current: peak 175 mA typ / 205 max at 3.3 V (115 / 137 mA at 5 V); average
  15 / 18 mA in 5 s periodic mode at 3.3 V (11 / 13 mA at 5 V); 3.2 / 3.5 mA in the ~30 s
  low-power periodic mode; 0.45 / 0.5 mA single shot every 5 min (SCD41 only) (table 4).
- Supply must stay constant: unloaded ripple/drops <= 30 mV, and a separate low-noise LDO
  that handles the peak current is recommended (p.5).
- Power-up <= 30 ms after VDD >= 2.25 V; soft reset 30 ms (table 7, p.6).
- Data format: 16-bit unsigned words, MSB first, each followed by an 8-bit CRC (poly
  0x31, init 0xFF, no reflection, no final XOR; CRC(0xBEEF) = 0x92). A CRC is mandatory
  on writes, optional on reads. Commands are not followed by a CRC (p.7, p.21).
- read_measurement (0xEC05): wait 1 ms, then read 9 bytes = CO2, temperature, RH words
  each with a CRC. CO2 [ppm] = word 0 directly, so it fits a 16-bit field. T [C] =
  -45 + 175 x word1 / 65535; RH [%] = 100 x word2 / 65535. The buffer empties on read; a
  read with no data is NACKed, so poll get_data_ready_status (0xE4B8; ready when the low
  11 bits are non-zero) (p.9, p.15).
- CO2 output range 0-40,000 ppm; SCD41 accuracy specified over 400-5,000 ppm (p.1, p.3).
  Response time tau63 typically 60 s (p.3).
- Measurement modes: periodic every 5 s (start 0x21B1), low-power periodic about every
  30 s (0x21AC), and SCD41-only single shot (0x219D, 5 s to complete; RHT-only 0x2196,
  50 ms). While a periodic measurement runs only read_measurement, get_data_ready_status,
  stop_periodic_measurement and the ambient-pressure commands are allowed; stop takes
  500 ms (p.8-10, p.17-18).
- Single shot: discard the first reading after a power cycle. ASC is tuned for a 5 minute
  interval and is unavailable if the sensor is power-cycled between readings (p.17).
- Settings (temperature offset, altitude, ASC) live in RAM until persist_settings
  (0x3615, 800 ms); that EEPROM is guaranteed for only 2000 writes (p.15).
- Default temperature offset is 4 C; the on-chip T/RH are specified +/-0.8 C and +/-6 %RH
  (p.3, p.10), so the AHT30 stays the better temperature/humidity source.

## Consequences for this board

- **Supply and bus levels.** At VDD = 5 V the SCD41 needs 3.25 V to read a high, and the
  3.3 V bus gives 3.3 V, a 50 mV margin: not reliable. At VDD = 3.3 V the threshold is
  2.145 V. Power it from 3.3 V, from its own LDO rated for the 205 mA peak (not the AHT30
  module's ME6231 regulator, whose current rating is not verified). A breakout's own
  regulator, pull-ups and level shifting are unverified.
- **Auto self-calibration (ASC) is a risk in an enclosed space.** ASC is on by default and
  assumes the sensor sees 400 ppm air at least once a week (p.1, p.13). A space that never
  drops to outdoor air will have its reading drift low. Either guarantee a weekly fresh-air
  exposure, or disable ASC (0x2416 = 0, then persist_settings) and recalibrate with a
  forced recalibration against a known reference (p.13).
- **Logging.** Run periodic mode (a new reading every 5 s) and keep min, max and average CO2
  over each 30 minute window rather than one snapshot; the sensor's 60 s response time
  means individual 5 s readings are smoothed anyway. Single shot is lower power
  (0.45 mA) but only gives instantaneous values.
- Set altitude or ambient pressure for best CO2 accuracy (p.10).

## Not yet verified

- A breakout's regulator, pull-up resistors and the voltage they pull up to.
- Output-low voltage (table 4 gives 0.66 V at 3 mA sink; the min/typ/max column is
  unclear in the extracted text) against what the PIC reads as low.
- The Sensirion application note "Low Power Operation SCD4x" (referenced p.17).
