#include <xc.h>

#include "../inc/aht30.h"
#include "../inc/board.h"
#include "../inc/i2c1.h"

#define AHT30_ADDR_W 0x70   // 0x38 << 1
#define AHT30_ADDR_R 0x71

// CRC8, poly 0x31, init 0xFF. The per-byte CRC step is linear over GF(2), so it splits
// into two 16-entry nibble tables: T(a) = HI[a >> 4] ^ LO[a & 0x0F]. That is 4x faster
// than the bit loop for 32 bytes of flash (a full 256-byte table is only 2x faster still).
static const uint8_t crc8_hi[16] = {
    0x00, 0x43, 0x86, 0xC5, 0x3D, 0x7E, 0xBB, 0xF8,
    0x7A, 0x39, 0xFC, 0xBF, 0x47, 0x04, 0xC1, 0x82
};
static const uint8_t crc8_lo[16] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E
};

// len must be >= 1.
static uint8_t crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;

    do {
        crc ^= *data++;
        crc = crc8_hi[crc >> 4] ^ crc8_lo[crc & 0x0F];
    } while (--len);
    return crc;
}

// Returns (x * k + 0x8000) >> 16 for a 20-bit x = x2:x1:x0 (x2 <= 0x0F) and k = kh:kl,
// built from 8x8 hardware multiplies instead of a 32x32 library multiply. The low
// partial product only contributes its carry; 0x80 at the second column is the
// +0x8000 rounding term. The accumulator stays within 16 bits for kh <= 4.
static uint16_t mulhi(uint8_t x2, uint8_t x1, uint8_t x0, uint8_t kh, uint8_t kl)
{
    uint16_t acc = (uint16_t)((uint16_t)x0 * kl) >> 8;

    acc += (uint16_t)x0 * kh;
    acc += (uint16_t)x1 * kl;
    acc += 0x80;
    acc = (acc >> 8) + (uint16_t)x1 * kh;
    acc += (uint16_t)x2 * kl;
    return acc + (uint16_t)((uint16_t)x2 * kh << 8);
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

    __delay_ms(100);        // datasheet: >= 80 ms; extra margin covers oscillator tolerance

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

    // RH x100 = S_RH * 10000 / 2^20 = S_RH * 625 / 2^16 (625 = 0x0271)
    // T  x100 = S_T  * 20000 / 2^20 - 5000 = S_T * 1250 / 2^16 - 5000 (1250 = 0x04E2)
    // Exact over all 20 bits and rounded to nearest: error <= 0.5 LSB (0.005 units).
    *rh_x100 = mulhi(buf[1] >> 4, (uint8_t)(buf[1] << 4) | (buf[2] >> 4),
                     (uint8_t)(buf[2] << 4) | (buf[3] >> 4), 0x02, 0x71);
    *temp_x100 = (int16_t)(mulhi(buf[3] & 0x0F, buf[4], buf[5], 0x04, 0xE2) - 5000);

    return AHT30_OK;
}
