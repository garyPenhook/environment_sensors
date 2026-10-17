# AHT30 (WeAct Studio AHTxx module)

| File | What |
|---|---|
| `aht30_datasheet.pdf` | Aosong AHT30 datasheet (Chinese, A2, 2023-09), from the WeAct repo |
| `aht30_datasheet_en.md` | English translation of the AHT30 datasheet (unofficial; the PDF is authoritative) |
| `aht20_datasheet_en.pdf` | English AHT20 datasheet (same protocol family; used for the RH formula) |
| `weact_module_v10_schematic.pdf` | WeAct AHTxx module V1.0 schematic |
| `me6231_datasheet.pdf` | Microne ME6231 LDO family datasheet (module regulator) |
| `weact_aht.c` / `weact_aht.h` | WeAct's STM32 HAL example, reference only |

Source: github.com/WeActStudio/WeActStudio.AHTxxModule

## Facts from the datasheets

- I2C address 0x38 (write byte 0x70, read byte 0x71). Module schematic: "ADDR: 0x38".
- Chip supply 2.2-5.5 V. Wait 5 ms after VDD before driving SCL/SDA high.
- Measure: write `0xAC 0x33 0x00`, wait 80 ms, read 7 bytes:
  `Status, H[19:12], H[11:4], H[3:0]|T[19:16], T[15:8], T[7:0], CRC`.
- Status: bit7 busy, bit4 CRC_flag (OTP check), bit3 calibration enabled.
- CRC8: init 0xFF, poly 0x31, over the first 6 bytes.
- `T[C] = ST / 2^20 * 200 - 50`; `RH[%] = SRH / 2^20 * 100` (both formulas are also in the AHT30 sheet, section 5.2 step 4).
- Sample no faster than once per second and keep I2C at 10-400 kHz to keep self-heating under 0.1 C.
- The AHT30 sheet has no init sequence; the AHT20 sheet's 0x1B/0x1C/0x1E init does not appear in it.

## Module (WeAct V1.0) and voltages

- On-board 3.3 V LDO, ME6231A33 (schematic marks it ME6231A33XG; the family datasheet
  covers the A33 3.3 V fixed, no-enable variants). Operating input 3-18 V, absolute max 20 V,
  dropout 125 mV at 100 mA. A 5 V VCC (PICkit 4 or the PIC's rail) is inside its range and
  leaves 1.7 V of headroom; the module should not be fed from 3.3 V.
- The AHT30 runs from the 3.3 V LDO output (AHT30 range 2.2-5.5 V, abs max 5.5 V).
- 4.7 kohm pull-ups on SCL/SDA to the 3.3 V rail (schematic R1/R2), so the bus idles at 3.3 V.
  AHT30 I/O abs max is VDD + 0.3 V, so the PIC must never drive or pull SCL/SDA to 5 V:
  the MSSP drives them open-drain, WPUC resets to 0 (no weak pull-ups), and RC3/RC4 are
  left as inputs.
- 3.3 V is below the PIC's I2C input threshold (0.7 VDD = 3.5 V at 5 V) but above the SMBus
  threshold (VIH 2.1 V, DS40002043F table 39-4). Set `SSP1STAT.CKE = 1` ("Enables
  SMBus-specific inputs", I2C mode), which `i2c1_init()` does. INLVLC does not select this:
  on RC3/RC4 the MSSP uses its own I2C/SMBus buffers.
- Pins: RC3 = SCL1, RC4 = SDA1 (PPS input defaults `SSP1CLKPPS`/`SSP1DATPPS`; outputs mapped
  with `RC3PPS = 0x0F`, `RC4PPS = 0x10`).
