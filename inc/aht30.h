#ifndef AHT30_H
#define AHT30_H

#include <stdint.h>

typedef enum {
    AHT30_OK = 0,
    AHT30_ERR_BUS,      // I2C timeout or no ACK
    AHT30_ERR_BUSY,     // status bit 7 still set: fetched before the conversion finished
    AHT30_ERR_CRC       // CRC8 mismatch
} aht30_status_t;

/** Waits out the sensor's power-up time. Call after i2c1_init(). */
void aht30_init(void);

/**
 * Non-blocking measurement in two steps: aht30_start() triggers a conversion and
 * returns at once; call aht30_fetch() no sooner than 80 ms later (datasheet minimum).
 * Sample no faster than once per second to keep self-heating under 0.1 C.
 */
aht30_status_t aht30_start(void);

/**
 * Reads and checks the result of the last aht30_start(). Both outputs are rounded to
 * the nearest 0.01 and are only written when AHT30_OK is returned.
 *   temp_x100: degrees C x 100 (2512 = 25.12 C)
 *   rh_x100:   %RH x 100       (4560 = 45.60 %RH)
 */
aht30_status_t aht30_fetch(int16_t *temp_x100, uint16_t *rh_x100);

#endif
