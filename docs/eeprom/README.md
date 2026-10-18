# 24LC256 I2C EEPROM (Microchip) — planned

Optional data logger memory for the shared MSSP1 bus. Datasheet DS20001203Y
(24AA256/24LC256/24FC256) is kept locally, not committed (see `.gitignore`).

## Mouser options (checked 2026-10-18, qty 1)

| Part | Mouser # | Package | Price | Stock |
|---|---|---|---|---|
| 24LC256-I/P | 579-24LC256-I/P | 8-pin PDIP | $1.22 | 5,687 |
| 24LC256-I/SN | 579-24LC256-I/SN | 8-pin SOIC | $1.16 | 8,421 |
| 24LC256-I/MF | 579-24LC256-I/MF | 8-lead DFN | $1.40 | 5,872 |
| 24LC256T-I/MS | 579-24LC256T-I/MS | MSOP-8 (reel, cut tape available) | $1.23 | 15,724 |
| 24LC256T-I/MNY | 579-24LC256T-I/MNY | TDFN (reel, cut tape available) | $1.17 | 1,321 |

24LC256-I/PG is Not Recommended for New Designs; Mouser lists 24LC256-I/P as the replacement.

## Verified against DS20001203Y

- Supply 2.5-5.5 V for the 24LC256 (1.7 V parts are the 24AA256/24FC256), max 400 kHz at
  that supply (p.1, table 1-2 p.4). Absolute max VCC 6.5 V; inputs -0.6 V to VCC + 1.0 V (p.3).
- Input levels: VIH = 0.7 VCC, VIL = 0.3 VCC for VCC >= 2.5 V; VOL <= 0.40 V at 3.0 mA
  @ 4.5 V or 2.1 mA @ 2.5 V (D1, D2, D4, p.3).
- Address: control code 1010 then A2 A1 A0, so 7-bit addresses 0x50-0x57 (p.10). No clash
  with the AHT30 (0x38) or OLED (0x3C/0x3D). SCD41 address still unverified.
- MSOP: A0 and A1 are not connected, A0/A1 bits must be sent as 0, so at most two devices
  (p.7, p.10). PDIP/SOIC/DFN/TDFN decode all three pins.
- A0, A1, A2 and WP must be tied to VSS or VCC, never floating. WP to VCC blocks all
  writes (the device still ACKs and ignores them); WP to VSS enables them (p.7, p.11).
- Timing at 2.5-5.5 V: THIGH >= 600 ns, TLOW >= 1300 ns, rise <= 300 ns, fall <= 300 ns
  (rise/fall not 100% tested), TBUF >= 1300 ns (table 1-2, p.4-5).
- Addressing: 16-bit word address, high byte first, upper bit a don't-care (p.10).
- Writes: 64-byte page, TWC <= 5 ms for a byte or page write. A page write that crosses
  a 64-byte boundary wraps to the start of the same page and overwrites it. The device
  does not ACK during the write cycle, so poll for the ACK (sections 6, 7, p.11-12).
- Endurance 1,000,000 cycles at 25 C, 5.5 V, page mode, specified per page (a short write
  still cycles the whole page); retention > 200 years (p.1, p.5, p.11).
- Currents: read 400 uA max (5.5 V, 400 kHz), write 3 mA max, standby 1 uA max at 3.6 V,
  1.5 uA at 5.5 V (I-temp) (D8-D10, p.3).
- Recommended SDA pull-up: 10 kohm at 100 kHz, 2 kohm at 400 kHz (p.7).

## Consequences for this board

- **Power it from 3.3 V, not 5 V.** The bus idles at 3.3 V. At VCC = 5 V, VIH = 3.5 V,
  so the EEPROM would read a 3.3 V bus high level as undefined. At 3.3 V, VIH = 2.31 V.
  The 3.3 V rail is the AHT30 module's on-board LDO, which this project does not use as
  a supply for other parts, so a separate 3.3 V source is needed. Not yet decided.
- 333 kHz is under the 400 kHz limit. The half period at 333 kHz is about 1.5 us, which
  clears THIGH (600 ns) and TLOW (1300 ns), but TLOW only by about 200 ns, so check the
  MSSP's actual low time on a scope.
- The 300 ns rise-time limit allows about 75 pF of bus capacitance with the 4.7 kohm
  AHT30 pull-ups (20-80% rise ~ 0.85 RC). The datasheet recommends 2 kohm at 400 kHz.
  Keep the wiring short; the added load of a third device is small.
- Capacity: 32 KB. At 4 bytes per sample once a minute that is about 5.7 days; with a
  6-byte record (adding CO2) about 3.8 days. Written as a ring buffer, a page is
  rewritten once per pass, so the 1M-cycle endurance is not a concern at that rate.
- Writes block for up to 5 ms. The AHT30 conversion is already non-blocking, so write
  one page per sample, outside the 20 ms Timer0 tick handler.

## Not verified

- 24CS256 (Mouser 579-24CS256T-E/Q4B, $0.64): different datasheet (DS20005998), not read.
- The PIC18F27Q10 VOL and the SMBus VIL against the EEPROM's 0.3 VCC at 3.3 V.
- The OLED module's pull-up values (affects the combined pull-up and rise time).
