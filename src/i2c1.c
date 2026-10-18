#include <xc.h>

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
    // Both pins are bidirectional: map the same pin to the PPS input and output.
    SSP1CLKPPS = 0x13;        // RC3 (PORT = C, PIN = 3)
    SSP1DATPPS = 0x14;        // RC4 (PORT = C, PIN = 4)
    RC3PPS = 0x0F;            // MSSP1 SCK/SCL
    RC4PPS = 0x10;            // MSSP1 SDO/SDA

    SSP1ADD = I2C1_ADD;
    SSP1CON2 = 0x00;
    SSP1STAT = 0xC0;          // SMP = 1 (no slew control, <= 1 MHz), CKE = 1 (SMBus inputs)
    SSP1CON1 = 0x28;          // SSPEN = 1, SSPM = 1000 (I2C master, clock from SSP1ADD)
    PIR3bits.SSP1IF = 0;
}

void i2c1_reset(void)
{
    SSP1CON1bits.SSPEN = 0;
    SSP1CON1bits.SSPEN = 1;
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
