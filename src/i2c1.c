#include <xc.h>

#include "../inc/board.h"
#include "../inc/i2c1.h"

// Busy-wait budget for one MSSP operation, in polls of SSP1IF (~7 instruction cycles
// each at -Os): 255 x 7 = ~1785 Tcy = ~7 ms at FOSC/4 = 250 kHz. The longest normal
// operation (byte + ACK = 9 SCL periods at 62.5 kHz) is 144 us = 36 Tcy, so this only
// expires if the bus is stuck or the slave stretches SCL for milliseconds.
#define I2C1_TIMEOUT 255U

// SCL = FOSC / (4 * (SSP1ADD + 1)) = 1 MHz / (4 * 4) = 62.5 kHz (AHT30: 10-400 kHz).
// SSP1ADD values 0x00-0x02 are not valid for the I2C baud rate generator
// (DS40002043F, 28.7), so 3 is the fastest setting at FOSC = 1 MHz.
#define I2C1_ADD 3U

// PPS output codes for RC3PPS/RC4PPS; i2c1_reset() swaps them out and back.
#define PPS_SCL1 0x0F             // MSSP1 SCK/SCL
#define PPS_SDA1 0x10             // MSSP1 SDO/SDA
#define PPS_LAT  0x00             // LATxy

// SSP1CON1: SSPEN = 1, SSPM = 1000 (I2C master, clock from SSP1ADD); WCOL = SSPOV = 0
#define SSP1CON1_MASTER 0x28

// SSP1IF is set by hardware when a Start, Stop, byte + ACK, received byte or
// Acknowledge sequence completes (DS40002043F, 28.6). Each operation clears it
// before starting, so a flag left over from an aborted operation can't satisfy this.
static bool wait_done(void)
{
    uint8_t n = I2C1_TIMEOUT;

    while (!PIR3bits.SSP1IF) {
        if (--n == 0) {
            return false;
        }
    }
    return true;
}

void i2c1_init(void)
{
    // TTL thresholds for PORT reads of RC3/RC4, used only by the bus clear in
    // i2c1_reset(): the bus idles at 3.3 V, under the ST VIH of 0.8 VDD = 4.0 V but over
    // the TTL VIH of 2.0 V (DS40002043F table 39-4, D320/D322). The MSSP itself uses its
    // own I2C/SMBus buffers (INLVLC note, 16.5.38). Changed while the MSSP is off, as 16.2.6
    // asks.
    INLVLC &= (uint8_t)~0x18;

    // Both pins are bidirectional: map the same pin to the PPS input and output.
    SSP1CLKPPS = 0x13;        // RC3 (PORT = C, PIN = 3)
    SSP1DATPPS = 0x14;        // RC4 (PORT = C, PIN = 4)
    RC3PPS = PPS_SCL1;
    RC4PPS = PPS_SDA1;

    SSP1ADD = I2C1_ADD;
    SSP1CON2 = 0x00;
    SSP1STAT = 0xC0;          // SMP = 1 (no slew control, <= 1 MHz), CKE = 1 (SMBus inputs)
    SSP1CON1 = SSP1CON1_MASTER;
    PIR3bits.SSP1IF = 0;
}

// Minimum half SCL period of the bus clear, so it stays in spec at any FOSC. At 1 MHz the
// call and loop overhead dominates: ~9 kHz in the simulator, well under the SSD1315
// (tcycle >= 2.5 us) and AHT30 (<= 400 kHz) limits.
#define BUS_CLEAR_HALF_US 5

// Waits for a released SCL to read high, allowing a slave to stretch the clock.
// False if it is still low after the timeout: something holds SCL, which no master can fix.
static bool scl_release(void)
{
    uint8_t n = I2C1_TIMEOUT;

    TRISCbits.TRISC3 = 1;
    while (!PORTCbits.RC3) {
        if (--n == 0) {
            return false;
        }
    }
    __delay_us(BUS_CLEAR_HALF_US);
    return true;
}

static void scl_low(void)
{
    TRISCbits.TRISC3 = 0;
    __delay_us(BUS_CLEAR_HALF_US);
}

// Disabling the MSSP drops a stuck transaction on the PIC side only. A slave interrupted
// mid-byte can still be holding SDA low, which blocks every device on the bus, so the
// pins are taken over as GPIO for the standard I2C bus clear (NXP UM10204): clock SCL
// until the slave releases SDA (at most 9 pulses), then send a STOP.
// Open-drain by hand: LAT = 0 and TRIS = 1 releases a line, TRIS = 0 pulls it low.
// Never drive high: the bus is pulled up to 3.3 V on the modules.
void i2c1_reset(void)
{
    SSP1CON1bits.SSPEN = 0;   // MSSP off (SSPxCON1.SSPEN = 0: pins are PORT pins)
    RC3PPS = PPS_LAT;         // and route LATC3/LATC4 to the pins explicitly via PPS,
    RC4PPS = PPS_LAT;         // rather than rely on the disabled MSSP's output selection
    LATCbits.LATC3 = 0;
    LATCbits.LATC4 = 0;
    TRISCbits.TRISC4 = 1;     // SDA released

    if (scl_release()) {
        bool scl_ok = true;

        for (uint8_t i = 0; i < 9 && scl_ok && !PORTCbits.RC4; i++) {
            scl_low();
            scl_ok = scl_release();
        }
        if (scl_ok) {
            // STOP: SDA low while SCL is low, release SCL, then release SDA while SCL is
            // high. If SCL is still held at that point no STOP is possible; SDA is
            // released anyway.
            scl_low();
            TRISCbits.TRISC4 = 0;
            __delay_us(BUS_CLEAR_HALF_US);
            scl_release();
            TRISCbits.TRISC4 = 1;
            __delay_us(BUS_CLEAR_HALF_US);
        }
    }
    TRISCbits.TRISC3 = 1;     // both lines released whatever happened above
    TRISCbits.TRISC4 = 1;

    RC3PPS = PPS_SCL1;
    RC4PPS = PPS_SDA1;
    SSP1CON2 = 0x00;
    PIR3bits.BCL1IF = 0;
    SSP1CON1 = SSP1CON1_MASTER;   // also clears WCOL/SSPOV (R/W/HS: software must clear)
}

bool i2c1_start(void)
{
    PIR3bits.SSP1IF = 0;
    SSP1CON2bits.SEN = 1;
    return wait_done();
}

bool i2c1_stop(void)
{
    PIR3bits.SSP1IF = 0;
    SSP1CON2bits.PEN = 1;
    return wait_done();
}

bool i2c1_write(uint8_t data)
{
    PIR3bits.SSP1IF = 0;
    SSP1CON1bits.WCOL = 0;
    SSP1BUF = data;
    // WCOL is set at the write itself if the MSSP wasn't idle; nothing was sent then.
    if (SSP1CON1bits.WCOL || !wait_done()) {
        return false;
    }
    return !SSP1CON2bits.ACKSTAT;
}

bool i2c1_read(uint8_t *data, bool ack)
{
    PIR3bits.SSP1IF = 0;
    SSP1CON2bits.RCEN = 1;
    if (!wait_done()) {       // set after the 8th falling SCL edge, together with BF
        return false;
    }
    *data = SSP1BUF;

    PIR3bits.SSP1IF = 0;
    SSP1CON2bits.ACKDT = ack ? 0 : 1;
    SSP1CON2bits.ACKEN = 1;
    return wait_done();
}
