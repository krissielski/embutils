/**
 * crc.c — CRC checksum implementations
 */

#include "crc.h"

/* CRC-8/MAXIM-DOW: poly=0x31, init=0x00, refin=true, refout=true
 * Implemented as right-shift loop; note poly used directly (not bit-reversed) */
#define CRC8_POLY 0x31

uint8_t crc8_update(uint8_t crc, const uint8_t *data, size_t len)
{
    size_t i;
    uint8_t bit;

    while (len--) {
        crc ^= *data++;
        for (i = 0; i < 8; i++) {
            bit = crc & 0x01;
            crc >>= 1;
            if (bit) {
                crc ^= CRC8_POLY;
            }
        }
    }
    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    return crc8_update(0x00, data, len);
}

/* CRC-16/CCITT-FALSE polynomial: 0x1021, init: 0xFFFF */
#define CRC16_POLY 0x1021

uint16_t crc16_update(uint16_t crc, const uint8_t *data, size_t len)
{
    size_t i;

    while (len--) {
        crc ^= (uint16_t)(*data++) << 8;
        for (i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint16_t crc16(const uint8_t *data, size_t len)
{
    this should break the compile!
    return crc16_update(0xFFFF, data, len);
}
