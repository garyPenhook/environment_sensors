#include <xc.h>

#include "../inc/aht30.h"
#include "../inc/board.h"
#include "../inc/i2c1.h"

#define AHT30_ADDR_W 0x70   // 0x38 << 1
#define AHT30_ADDR_R 0x71

static uint8_t crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;

    while (len--) {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; i++) {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

void aht30_init(void)
{
    __delay_ms(10);         // datasheet: wait >= 5 ms after VDD before talking to the sensor
}

aht30_status_t aht30_read(int16_t *temp_x100, uint16_t *rh_x100)
{
    uint8_t buf[7];

    // Trigger a measurement: 0x70 0xAC 0x33 0x00
    if (!i2c1_start() || !i2c1_write(AHT30_ADDR_W) || !i2c1_write(0xAC) ||
        !i2c1_write(0x33) || !i2c1_write(0x00) || !i2c1_stop()) {
        i2c1_reset();
        return AHT30_ERR_BUS;
    }

    __delay_ms(80);

    // Read Status, H[19:12], H[11:4], H[3:0]|T[19:16], T[15:8], T[7:0], CRC
    if (!i2c1_start() || !i2c1_write(AHT30_ADDR_R)) {
        i2c1_reset();
        return AHT30_ERR_BUS;
    }
    for (uint8_t i = 0; i < 7; i++) {
        if (!i2c1_read(&buf[i], i < 6)) {
            i2c1_reset();
            return AHT30_ERR_BUS;
        }
    }
    if (!i2c1_stop()) {
        i2c1_reset();
        return AHT30_ERR_BUS;
    }

    if (buf[0] & 0x80) {
        return AHT30_ERR_BUSY;
    }
    if (crc8(buf, 6) != buf[6]) {
        return AHT30_ERR_CRC;
    }

    uint32_t raw_h = ((uint32_t)buf[1] << 12) | ((uint32_t)buf[2] << 4) | (buf[3] >> 4);
    uint32_t raw_t = ((uint32_t)(buf[3] & 0x0F) << 16) | ((uint32_t)buf[4] << 8) | buf[5];

    // RH = raw/2^20 * 100, T = raw/2^20 * 200 - 50. Drop the low 4 bits so the
    // products stay inside 32 bits.
    *rh_x100 = (uint16_t)(((raw_h >> 4) * 10000UL) >> 16);
    *temp_x100 = (int16_t)((int32_t)(((raw_t >> 4) * 20000UL) >> 16) - 5000);

    return AHT30_OK;
}
