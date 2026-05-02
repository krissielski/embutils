/**
 * bit_utils.c — Bit manipulation implementations
 */

#include "bit_utils.h"

void pack_u16_be(uint8_t *buf, uint16_t val)
{
    buf[0] = (uint8_t)(val >> 8);
    buf[1] = (uint8_t)(val & 0xFF);
}

uint16_t unpack_u16_be(const uint8_t *buf)
{
    return (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);
}

void pack_u32_be(uint8_t *buf, uint32_t val)
{
    buf[0] = (uint8_t)((val >> 24) & 0xFF);
    buf[1] = (uint8_t)((val >> 16) & 0xFF);
    buf[2] = (uint8_t)((val >>  8) & 0xFF);
    buf[3] = (uint8_t)( val        & 0xFF);
}

uint32_t unpack_u32_be(const uint8_t *buf)
{
    return ((uint32_t)buf[0] << 24)
         | ((uint32_t)buf[1] << 16)
         | ((uint32_t)buf[2] <<  8)
         |  (uint32_t)buf[3];
}

uint32_t extract_bits(uint32_t val, uint8_t offset, uint8_t width)
{
    uint32_t mask = (width == 32) ? 0xFFFFFFFFU : ((1U << width) - 1U);
    return (val >> offset) & mask;
}

uint8_t count_set_bits(uint8_t val)
{
    uint8_t count = 0;
    while (val) {
        count += val & 1;
        val >>= 1;
    }
    return count;
}
