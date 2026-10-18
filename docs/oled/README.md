# 0.96" OLED (WeAct Studio OLED module V1.0)

Reference files are downloaded locally, not committed (see `.gitignore`).
Source: github.com/WeActStudio/WeActStudio.OLEDModule (`Doc/`, `Hardware/`,
`Examples/12864_OLED/BSP_12864_OLED/OLED.c`).

| File (local only) | What |
|---|---|
| `ssd1315_datasheet.pdf` | Solomon Systech SSD1315 datasheet, Rev 1.0 (Dec 2016) |
| `ssd1306_datasheet.pdf` | SSD1306 datasheet (older, command-compatible controller) |
| `zjy096_2864kswpg16_panel.pdf` | Panel drawing ZJY096-2864KSWPG16: 128x64, white, driver IC SSD1315 |
| `weact_board_shape.pdf` | WeAct 0.96" module V1.0 board drawing and silkscreen |
| `weact_OLED.c` / `.h`, `weact_main.c`, `weact_i2c.c` | WeAct's STM32 HAL example, reference only |

## Facts from the documents

- Module silkscreen: pins `SDA SCL VCC GND`, "VCC: 3.3-5V", "I/O: 3.3V", solder bridge
  SB1 "0x7A". WeAct's driver uses `OLED_ADDR 0x78 // 0x78/0x7A`.
- SSD1315 6.1.5: I2C slave address `011110 SA0`, so 0x3C (write byte 0x78) or 0x3D (0x7A).
- Control byte after the address: `Co D/C# 0 0 0 0 0 0`. Co = 0 means the rest of the
  transfer is all data bytes, D/C# = 0 makes them commands, 1 makes them GDDRAM data
  (`0x00` = command stream, `0x40` = data stream).
- I2C timing (table 9-6): tcycle >= 2.5 us (400 kHz max). The bus runs at 62.5 kHz.
- Power-on with charge pump (6.9.2): wait >= 20 ms after VDD is stable, then `8Dh 14h`
  (charge pump on, 7.5 V), then `AFh`. SEG/COM drive starts within 100 ms.
  Power-off: `AEh`, then `8Dh 10h`.
- GDDRAM: 8 pages x 128 columns, D0 = top row of a page (figure 6-14).
- Horizontal addressing (`20h 00h`) with a `21h`/`22h` column/page window: the column
  pointer wraps to the window start and advances the page; at the window end it returns to
  the start. `oled.c` uses this to send each text run as one transfer.
- Reset values that `oled.c` relies on: 64 MUX, display offset 0, start line 0,
  alternative COM pin config (DAh 12h), contrast 7Fh, normal display, VCOMH ~0.77 VCC.
- `A1h` + `C8h` together rotate the image 180 degrees. WeAct calls that "normal" (pin
  header at the top).
- WeAct's init sends `DBh 40h`; the SSD1315 table only defines A[5:4] for DBh, so `oled.c`
  leaves VCOMH at its reset value.

## Bus sharing

Same MSSP1 bus as the AHT30 (0x38). The silkscreen says the I/O is 3.3 V, which matches
the AHT30 module's 3.3 V pull-ups and the SMBus input levels already set on RC3/RC4.
