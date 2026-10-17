#ifndef AHT30_H
#define AHT30_H

#include <stdint.h>

typedef enum {
    AHT30_OK = 0,
    AHT30_ERR_BUS,      // I2C timeout or no ACK
    AHT30_ERR_BUSY,     // status bit 7 still set after the 80 ms conversion time
    AHT30_ERR_CRC       // CRC8 mismatch
} aht30_status_t;

/** Waits out the sensor's power-up time. Call after i2c1_init(). */
void aht30_init(void);

/**
 * Blocking measurement (~85 ms). Sample no faster than once per second to keep
 * self-heating under 0.1 C.
 *   temp_x100: degrees C x 100 (2512 = 25.12 C)
 *   rh_x100:   %RH x 100       (4560 = 45.60 %RH)
 */
aht30_status_t aht30_read(int16_t *temp_x100, uint16_t *rh_x100);

#endif
