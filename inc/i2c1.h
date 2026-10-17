#ifndef I2C1_H
#define I2C1_H

#include <stdbool.h>
#include <stdint.h>

/** MSSP1 as a blocking I2C master on RC3 (SCL1) / RC4 (SDA1). SMBus input levels. */
void i2c1_init(void);

/** Each call returns false on a bus timeout; the caller should i2c1_reset() and retry. */
bool i2c1_start(void);
bool i2c1_stop(void);
/** Returns true if the slave ACKed the byte. */
bool i2c1_write(uint8_t data);
/** Reads one byte; ack = true sends ACK (more bytes follow), false sends NACK (last byte). */
bool i2c1_read(uint8_t *data, bool ack);

/** Disable/re-enable the MSSP to drop any stuck transaction. */
void i2c1_reset(void);

#endif
