# AHT30 Datasheet — Temperature and Humidity Sensor (English translation)

Translated from `aht30_datasheet.pdf` (Aosong / ASAIR, Chinese, 13 pages, September 2023).
This is an unofficial working translation; the Chinese PDF is authoritative. Figures are not
reproduced; where a figure carries data (pinout, timing, formulas) it is transcribed as text.

**Features:** fully calibrated · digital output, I²C interface · excellent long-term stability ·
fast response, strong interference immunity · wide supply range 2.2–5.5 VDC

## Product Summary

The AHT30 is a new temperature and humidity sensor with low power consumption and high
accuracy. It comes in a dual-row flat no-lead (DFN-style) SMD package suitable for reflow
soldering, with a rugged housing and tiny size, so it integrates easily into demanding designs.

It contains a newly optimized dedicated ASIC, an improved MEMS semiconductor capacitive
humidity sensing element and a standard on-chip temperature sensing element. Performance is
significantly improved and remains stable even in harsh environments.

The AHT30 helps equipment reduce power consumption and is very cost-effective, which suits
volume production by companies that control cost tightly but care about quality.

**Applications:** smart home, consumer electronics, medical, automotive, industrial,
meteorology, e.g. HVAC, dehumidifiers and refrigerators, test and measurement equipment and
other temperature/humidity monitoring and control products.

*Figure 1: AHT30 package drawing (unit: mm, unspecified tolerance ±0.1 mm).*

## Sensor Performance

### Relative humidity — Table 1

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| Resolution | typical | – | 0.024 | – | %RH |
| Accuracy error ¹ | typical | – | ±3 | – | %RH |
| | maximum | see Figure 2 | | | %RH |
| Repeatability | – | – | ±0.1 | – | %RH |
| Hysteresis | – | – | ±1 | – | %RH |
| Nonlinearity | – | – | < 0.1 | – | %RH |
| Response time ² | τ 63% | – | < 8 | – | s |
| Measuring range, extended ³ | | 0 | – | 100 | %RH |
| Drift ⁴ | normal | – | < 1 | – | %RH/yr |

*Figure 2: typical and maximum relative-humidity error at 25 °C.*

### Electrical characteristics — Table 2

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| Supply voltage | typical | 2.2 | 3.3 | 5.5 | V |
| Supply current, IDD ⁵ | sleep | – | – | 0.2 | µA |
| | measuring | – | 570 | 600 | µA |
| Power consumption ⁵ | sleep | – | – | 0.8 | µW |
| | measuring | – | 1.8 | 1.9 | mW |
| Communication | two-wire digital interface, standard I²C protocol | | | | |

Footnotes:

1. Accuracy as tested at the factory in clean air at 25 °C; excludes hysteresis and nonlinearity.
2. Time to reach 63 % of a first-order response at 25 °C and 1 m/s airflow.
3. For readings outside the 8–85 %RH range, see User Guide section 1.1 for sensor deviation.
4. Measured in clean air. If volatile solvents, strong-smelling tapes, adhesives or packaging
   materials are near the sensor, this parameter may be larger. See the User Guide.
5. Min and max supply current and power are measured at 25 °C, VDD = 3.3 V. The typical value
   is for one measurement per second.

### Temperature — Table 3

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| Resolution | typical | – | 0.01 | – | °C |
| Accuracy error | typical | – | ±0.5 | – | °C |
| | maximum | see Figure 3 | | | °C |
| Repeatability | – | – | ±0.1 | – | °C |
| Hysteresis | – | – | ±0.1 | – | °C |
| Response time ⁶ | τ 63% | 5 | – | 30 | s |
| Operating range | – | −40 | – | 120 | °C |
| Drift | – | – | < 0.1 | – | °C/yr |

⁶ Response time depends strongly on PCB layout and mounting.

*Figure 3: typical and maximum temperature error.*

### Packaging — Table 4

| Sensor model | Packaging | Quantity |
|---|---|---|
| AHT30 | tape and reel | 5000 pcs/reel (max) |

---

# AHT30 User Guide

## 1. Extended performance

### 1.1 Operating conditions

Within the recommended "normal range" the sensor performs stably (Figure 4). Long exposure
outside the normal range, especially at high temperature and humidity (for example more than
12 hours at 85 °C / 85 %RH), can cause a temporary drift error, generally within the typical
accuracy +3 %RH. After returning to the normal range the sensor slowly recovers to the typical
accuracy of Table 1. To speed recovery see section 2.3 "Recovery treatment". Prolonged use
outside the "maximum range" shortens product life and affects performance.

*Figure 4: operating conditions.*

### 1.2 Relative-humidity accuracy at different temperatures

Figure 2 gives the humidity error at 25 °C; Figure 5 gives the typical humidity error at other
temperatures.

*Figure 5: typical relative-humidity error Δ%RH over 0–80 °C.*

Note: these are typical errors (excluding hysteresis) against a chilled-mirror precision
dew-point meter used as the relative-humidity reference.

## 2. Application information

### 2.1 Soldering instructions

To prevent oxidation and optimize soldering, the pads on the underside of the sensor are
plated with Ni/Au. On the PCB, the I/O contact surface ⁷ should be 0.2–0.3 mm longer and
0.1–0.2 mm wider than the sensor's I/O package pad; the inner part should match the shape of
the I/O pad, and the lead width to SMD package pad width ratio is 1:1 (Figure 8). For the
stencil and solder-mask design ⁸, a solder-mask opening larger than the metal pad (copper-defined
pad, SMD) is recommended. For SMD pads, if the gap between the copper pad and the solder mask
is 60–75 µm, the solder-mask opening should exceed the pad size by 120–150 µm. The square part
of the package pad should match a square solder-mask opening so that enough solder mask remains
(especially at corners) to prevent solder bridging. Each pad needs its own solder-mask opening,
forming a solder-mask web around adjacent pads.

*Figure 8: recommended sensor PCB design dimensions (unit: mm); the outer dashed line is the SMD package outline.*

For solder-paste printing, a laser-cut stainless steel stencil with electropolished trapezoidal
walls is recommended, 0.125 mm thick. Stencil openings at the pads must be 0.1 mm longer than
the PCB pads and placed 0.1 mm from the package center area. The stencil for the exposed pad
must cover 70–90 % of the pad area, i.e. 1.4 mm × 2.3 mm at the center of the thermal area.
Because the SMD mounting height is low, no-clean type-3 solder paste ⁹ is recommended, with
nitrogen purge during reflow.

*Figure 9: JEDEC standard soldering profile, T_P < 260 °C, t_P < 30 s, lead-free soldering;
T_L < 180 °C, t_L < 150 s; heating and cooling rates during soldering should be < 5 °C/s.*

Use a standard reflow oven. The sensor conforms to IPC/JEDEC J-STD-020D. Best reflow peak
temperature is below 230 °C; the maximum tolerable soldering temperature is 260 °C, and time at
peak T_P must be under 30 s (Figure 9). A low reflow temperature of 180 °C is recommended.

Note: readings may be temporarily low after soldering.

Footnotes:

7. The contact surface is the metal layer on the PCB where the SMD pad is soldered.
8. The solder mask is the insulating layer over the traces on the PCB top.
9. Solder type refers to particle size in the solder. Type 3 is a 25–45 µm powder.

### 2.2 Storage conditions and handling

A temperature/humidity sensor is not an ordinary electronic component; users must take care to
protect it. Long exposure to high concentrations of chemical vapor causes reading drift. Keep
the sensor in its anti-static bag at 10–50 °C. If it is not sealed in an ESD bag, storage
humidity must be 20–60 %RH. Sensors removed from the original packaging should be stored in
anti-static bags made of metallized PET/AL/CPE.

During production and transport, avoid exposing the sensor to high concentrations of chemical
solvents or prolonged exposure to open air. Avoid volatile glues, tapes, stickers and volatile
packaging materials such as foam foil and foam. The production area should be well ventilated.

### 2.3 Recovery treatment

Exposure to extreme operating conditions or chemical vapors makes readings drift. The sensor
can be restored to its calibrated state as follows:

- Humidity reads high: dry out. Leave at 60 °C and 5 %RH for 6 hours.
- Humidity reads low: rehydrate. Leave at 25 °C and 75 %RH for 24 hours.

### 2.4 Temperature effects

Relative humidity of a gas is computed from ambient temperature and dew point, so ambient
temperature strongly affects it. When measuring relative humidity, keep all sensors measuring
the same humidity at the same temperature, or comparing their readings is meaningless.

If the sensor shares a PCB with components that generate heat, take this into account in the
circuit design. Minimize heat transfer, for example by keeping the housing well ventilated,
keeping the copper pour shared between the sensor and the rest of the PCB as small as
possible, or leaving a 1.5 mm slot between them (Figure 10).

*Figure 10: top view of the sensor PCB; the milled slot minimizes heat transfer.*

Also, if measurements are taken too often, the sensor's self-heating affects accuracy. To keep
self-heating below 0.1 °C, use an I²C clock of 10 kHz–400 kHz (not higher) and a sampling
period longer than 1 s (at most one sample per second).

### 2.5 Product-level design for the application

Design the product so the sensor has these properties:

1. Full contact with outside air. *Figure 11: a suitable window in the housing gives a good
   measurement path and fuller air exchange.*
2. Complete isolation from air inside the housing. *Figure 12: isolating the sensor from the
   enclosed air inside the housing minimizes its influence.*
3. A small measurement dead zone around the sensor. *Figure 13: a small dead zone lets the
   sensor detect environmental change quickly and from all directions.*
4. Isolation from heat sources. *Figure 14: isolating the sensor from internal heat sources
   minimizes their effect on measurement.*
5. Controllable sensor power. To improve system stability, the following switchable-power
   scheme is provided (*Figure 15: typical application circuit*):
   - The host MCU supplies the sensor at 2.2–5.5 V.
   - On power-up, the MCU supplies VDD first; only after 5 ms may SCL and SDA be set high.
   - VDD of the sensor needs an RC filter, R1 and C1 in the figure.
6. PCB routing rules. To improve reliability, avoid routing traces or copper pour under the
   sensor. Do not use the sensor in corrosive gases or where condensation forms.

### 2.6 Materials for sealing and packaging

Some materials absorb and slowly release water vapor, degrading the AHT30's response. Choose
materials around the AHT30 carefully. Recommended: metals, LCP, POM (Delrin), PTFE (Teflon),
PE, PEEK, PP, PB, PPS, PSU, PVDF and PVF.

Where glue sealing or bonding is needed, epoxy potting compound, silicone grease, polyurethane
sealant and UV-cured encapsulant are recommended. Gases released by these materials may
contaminate the sensor (see 2.2), so before and after assembly steps involving the AHT30,
ensure good ventilation or bake at 60 °C to release residual odor.

### 2.7 Routing rules and signal integrity

If SCL and SDA run parallel and very close together, crosstalk can cause communication
failure. Separate the lines with VDD or GND between them, and use shielded cable. Lowering the
SCL frequency may also improve signal integrity.

## 3. Interface definition

### Table 5: AHT30 pinout (top view)

| Pin | Name | Description |
|---|---|---|
| 1 | NC | Leave floating |
| 2 | VDD | Supply voltage |
| 3 | SCL | Serial clock, bidirectional |
| 4 | SDA | Serial data, bidirectional |
| 5 | GND | Ground |
| 6 | NC | Leave floating |

### 3.1 Serial clock, SCL

SCL synchronizes communication between the microprocessor and the AHT30. The interface uses
fully static logic, so there is no minimum SCL frequency.

### 3.2 Serial data, SDA

The SDA pin is used for data input and output. When sending a command to the sensor, SDA is
valid on the rising edge of the serial clock (SCL) and must remain stable while SCL is high.
After the falling edge of SCL, the SDA value may change. To ensure safe communication, SDA's
valid time must be extended before the SCL rising edge and after the falling edge by T_SU and
T_HO respectively (see Figure 16). When reading data from the sensor, SDA is valid after SCL
goes low (T_V) and stays valid until the next falling edge of SCL.

To avoid signal contention, the microcontroller (MCU) may only drive SDA and SCL low; an
external pull-up resistor (for example 2.0–4.7 kΩ) pulls the signals high. See Tables 7 and 8
for detailed input/output characteristics.

## 4. Electrical characteristics

### 4.1 Absolute maximum ratings

Electrical characteristics are defined in Table 2. The absolute maximum ratings in Table 6 are
stress ratings and are given for information only. Functional operation of the device under
these conditions is not advised. Prolonged exposure to absolute-maximum conditions may affect
sensor reliability.

**Table 6: electrical absolute maximum ratings**

| Parameter | Min | Max | Unit |
|---|---|---|---|
| VDD to GND | −0.3 | 5.5 | V |
| Digital I/O pins (SDA, SCL) to GND | −0.3 | VDD + 0.3 | V |
| Input current per pin | −10 | 10 | mA |

ESD complies with JEDEC JESD22-A114 (human body model ±4 kV) and JEDEC JESD22-A115 (machine
model ±200 V). If test conditions exceed the stated limits, the sensor needs additional
protection circuitry.

### 4.2 Input/output characteristics

Electrical characteristics such as power consumption and input/output high and low voltage
levels depend on the supply voltage. For smooth communication it is important to keep the
signal design strictly within the ranges given in Tables 7 and 8 and Figure 16.

**Table 7: DC characteristics of the digital I/O pads.** Unless otherwise stated,
VDD = 2.2 V to 5.5 V, T = −40 °C to 120 °C.

| Parameter | Condition | Min | Typ | Max | Unit |
|---|---|---|---|---|---|
| Output low voltage V_OL | VDD = 3.3 V, −4 mA < I_OL < 0 mA | 0 | – | 0.4 | V |
| Output high voltage V_OH | – | 70 % VDD | – | VDD | V |
| Input low voltage V_IL | – | 0 | – | 30 % VDD | V |
| Input high voltage V_IH | – | 70 % VDD | – | VDD | V |

*Figure 16: timing diagram of the digital input/output; abbreviations are explained in Table 7.
The bold SDA line is driven by the sensor, the normal SDA line by the microcontroller. Note
that the SDA valid read time is triggered by the falling edge of the preceding transition.*

**Table 8: timing characteristics of the I²C digital input/output** (meanings shown in
Figure 16, unless otherwise noted). The original table labels the columns "I²C standard mode"
and "I²C high-speed mode" (the latter is the 400 kHz fast mode).

| Symbol | Parameter | Standard mode min | Standard mode max | Fast mode min | Fast mode max | Unit |
|---|---|---|---|---|---|---|
| f(SCL) | SCL clock frequency | 0 | 100 | 0 | 400 | kHz |
| tw(SCLL) | SCL low time | 4.7 | – | 1.3 | – | µs |
| tw(SCLH) | SCL high time | 4.0 | – | 0.6 | – | µs |
| tsu(SDA) | SDA setup time | 250 | – | 100 | – | ns |
| th(SDA) | SDA data hold time | 0.09 | 3.45 | 0.02 | 0.9 | µs |

Notes:

- For both pins, measurements are taken from 0.2 VDD and 0.8 VDD.
- The I²C timing above is determined with the following internal delays:
  1. The internal SDI input pin is delayed relative to the SCK pin; typical value 100 ns.
  2. The internal SDI output pin is delayed relative to the SCK falling edge; typical value 200 ns.

## 5. Sensor communication

The AHT30 communicates with the standard I²C protocol. For information on the I²C protocol
beyond the sections below, see the website www.aosong.com, which provides sample programs.

### 5.1 I²C timing and command format

*Figure 17: I²C bus timing diagram (START condition, SDA input / SDA change, STOP condition).*

*Figure 18: read/write data description.*

- `0x70 0xAC 0x33 0x00` is the byte sequence that writes the measurement command to the sensor
  (0x70 = 7-bit address 0x38 plus write bit; read is 0x71).
- `Status` is the status byte, see Table 9.
- `S_RH[19:0]` is the 20-bit relative-humidity data.
- `S_T[19:0]` is the 20-bit temperature data.
- `CRC` is the check byte: the CRC8 of Status, S_RH[19:0] and S_T[19:0].

Read frame (after `0x71` + ACK), 7 bytes, host ACKs every byte except the last, which it NACKs
before STOP:

`Status`, `S_RH[19:12]`, `S_RH[11:4]`, `S_RH[3:0] | S_T[19:16]`, `S_T[15:8]`, `S_T[7:0]`, `CRC`

After the write sequence, the figure marks a wait of "≥ 80 ms".

**Table 9: status word**

| Bit | Meaning | Description |
|---|---|---|
| 7 | Busy indication | 1 = sensor busy, measurement in progress; 0 = sensor idle, in sleep state |
| 6:5 | Mode status | 00 = currently in NOR mode; 01 = currently in CYC mode; 1x = currently in CMD mode |
| 4 | CRC_flag | 1 = OTP memory data integrity test (CRC) passed; 0 = integrity test failed, indicating an error in the OTP data |
| 3 | Calibration enable | 0 = calibration computation disabled, output is raw ADC data; 1 = calibration computation enabled, output is calibrated data |
| 2 | CMP interrupt | 0 = calibrated capacitance data is within the CMP interrupt threshold range; 1 = calibrated capacitance data is outside the CMP interrupt threshold range |
| 1 | Reserved | – |
| 0 | Reserved | – |

### 5.2 Sensor read procedure

1. **Send the measurement command.** After the sensor's VDD powers up, wait 5 ms, send the
   write-measurement command `0x70 0xAC 0x33 0x00`, and wait 80 ms for the measurement to
   complete.
2. **Get the calibrated temperature/humidity data.** After the 80 ms wait, send `0x71` to read
   the sensor. This returns the status word Status, calibrated humidity and temperature data
   S_RH[19:0] and S_T[19:0], and the check byte CRC (see Figure 18; status word per Table 9).
3. **CRC check.** Run CRC8 over the read Status, S_RH[19:0] and S_T[19:0]. The CRC initial
   value is 0xFF and the CRC8 polynomial is CRC[7:0] = 1 + x⁴ + x⁵ + x⁸. CRC code:

   ```c
   //CRC check type: CRC8
   //Polynomial: X8+X5+X4+1
   //Poly: 0011 0001  0x31
   unsigned char Calc_CRC8(unsigned char *message, unsigned char Num)
   {
       unsigned char i;
       unsigned char byte;
       unsigned char crc = 0xFF;
       for (byte = 0; byte < Num; byte++)
       {
           crc ^= (message[byte]);
           for (i = 8; i > 0; --i)
           {
               if (crc & 0x80)
                   crc = (crc << 1) ^ 0x31;
               else
                   crc = (crc << 1);
           }
       }
       return crc;
   }
   ```

4. **Compute temperature and humidity.**

   Relative humidity: `RH[%] = (S_RH / 2^20) * 100`

   Temperature: `T[°C] = (S_T / 2^20) * 200 - 50`

   Example: S_T = 0x2FFAB, which is 196523 decimal;
   T = (196523 / 1048576) * 200 − 50 = −12.5 °C.

Note: the sensor needs time to acquire. If the host does not wait 80 ms after the measurement
command (0xAC, 0x33, 0x00), status bit 7 may still be 1, and the data read is the temperature
and humidity from the previous measurement command. After a measurement completes, the sensor
goes to sleep until woken by the next communication.

## 6. Environmental stability

If the sensor is used in equipment or machinery, make sure the sensor used for measurement and
the one used as reference sense the same temperature and relative humidity. If the sensor is
placed inside equipment its response time lengthens, so leave enough measurement time in the
program design. The sensor is tested to Aosong's corporate temperature/humidity sensor
standard. Performance under other test conditions is not guaranteed and is not part of the
sensor's performance. In particular, no commitment is made for specific user-required
scenarios.

## 7. Packaging

Tape-and-reel packaging in anti-static bags, 5000 pcs per reel. A label (Figure 20) on the
outer anti-static bag provides tracking information.

*Figure 19: reel. Figure 20: label on the anti-static bag (PN AHT30, Q: 5000, MSL 1 / 260 °C,
Pb-free, RoHS).*

## Warning and personal injury

Do not use this product in safety protection devices or emergency-stop equipment, or in any
other application where product failure could cause personal injury, unless there is a specific
purpose or authorization. Consult the product datasheet and instructions before installing,
handling, using or maintaining the product. Failure to follow this advice may result in death
or serious personal injury. The company assumes no liability for personal injury or death
arising from this, and is released from any claims against its managers, employees, affiliated
agents, distributors and others, including costs, claim expenses and attorney fees.

## Quality assurance

Guangzhou Aosong Electronics Co., Ltd. gives direct purchasers of its products the quality
warranty below (counted from the ship date), against the specifications in the Aosong product
datasheet. If a product is shown to be defective within the warranty period, the company will
repair or replace it free of charge.

| Product category | Warranty period |
|---|---|
| AHT30 temperature and humidity sensor | 12 months |

The company is responsible only for defects arising when the product is used within its
technical conditions. It makes no guarantee for use in non-recommended special scenarios, and
makes no commitment about reliability when the product is used with other companies' products
or circuits.

This manual may be changed without notice. The company holds the right of final interpretation
of this product. Copyright © 2023, ASAIR®
