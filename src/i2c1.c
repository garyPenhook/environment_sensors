#include <xc.h>

#include "../inc/i2c1.h"

// Busy-wait budget for one MSSP operation. A byte takes ~72 us at 125 kHz; this
// is far longer, so it only expires if the bus is stuck.
#define I2C1_TIMEOUT 3000U

// SCL = FOSC / (4 * (SSP1ADD + 1)) = 1 MHz / (4 * 2) = 125 kHz (AHT30: 10-400 kHz).
#define I2C1_ADD 1U

// MSSP is idle when no start/restart/stop/receive/ack is pending and no byte is
// being transmitted (SSP1STAT.R_nW ORed with SEN/RSEN/PEN/RCEN/ACKEN).
static bool wait_idle(void)
{
    uint16_t n = I2C1_TIMEOUT;

    while ((SSP1CON2 & 0x1F) || SSP1STATbits.R_nW) {
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
    if (!wait_idle()) {
        return false;
    }
    SSP1CON2bits.SEN = 1;
    return wait_idle();
}

bool i2c1_stop(void)
{
    if (!wait_idle()) {
        return false;
    }
    SSP1CON2bits.PEN = 1;
    return wait_idle();
}

bool i2c1_write(uint8_t data)
{
    if (!wait_idle()) {
        return false;
    }
    SSP1CON1bits.WCOL = 0;
    SSP1BUF = data;
    if (!wait_idle() || SSP1CON1bits.WCOL) {
        return false;
    }
    return !SSP1CON2bits.ACKSTAT;
}

bool i2c1_read(uint8_t *data, bool ack)
{
    uint16_t n = I2C1_TIMEOUT;

    if (!wait_idle()) {
        return false;
    }
    SSP1CON2bits.RCEN = 1;
    while (!SSP1STATbits.BF) {
        if (--n == 0) {
            return false;
        }
    }
    *data = SSP1BUF;

    if (!wait_idle()) {
        return false;
    }
    SSP1CON2bits.ACKDT = ack ? 0 : 1;
    SSP1CON2bits.ACKEN = 1;
    return wait_idle();
}
