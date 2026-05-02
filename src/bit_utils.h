/**
 * bit_utils.h — Bit manipulation helpers
 *
 * Bit manipulation is constant in embedded work: setting/clearing/toggling
 * hardware register bits, extracting fields from sensor data, packing
 * multi-byte values into byte arrays for transmission.
 *
 * Using these helpers instead of raw bit operations makes the code more
 * readable and less error-prone.
 */

#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>
#include <stdbool.h>

/* ---- Single-bit operations ---- */

/** Set bit N in value */
#define BIT_SET(val, n)     ((val) |=  (1U << (n)))

/** Clear bit N in value */
#define BIT_CLEAR(val, n)   ((val) &= ~(1U << (n)))

/** Toggle bit N in value */
#define BIT_TOGGLE(val, n)  ((val) ^=  (1U << (n)))

/** Test if bit N is set (returns non-zero if set) */
#define BIT_TEST(val, n)    ((val) &   (1U << (n)))

/* ---- Byte packing / unpacking ---- */

/**
 * pack_u16_be — Pack a uint16_t into a 2-byte buffer, big-endian.
 * Big-endian (MSB first) is the network byte order and common in
 * embedded protocols (e.g., Modbus, most sensor datasheets).
 *
 * @param buf   Output buffer (must be at least 2 bytes)
 * @param val   Value to pack
 */
void pack_u16_be(uint8_t *buf, uint16_t val);

/**
 * unpack_u16_be — Unpack a uint16_t from a 2-byte big-endian buffer.
 *
 * @param buf   Input buffer (must be at least 2 bytes)
 * @return      Unpacked value
 */
uint16_t unpack_u16_be(const uint8_t *buf);

/**
 * pack_u32_be — Pack a uint32_t into a 4-byte buffer, big-endian.
 */
void pack_u32_be(uint8_t *buf, uint32_t val);

/**
 * unpack_u32_be — Unpack a uint32_t from a 4-byte big-endian buffer.
 */
uint32_t unpack_u32_be(const uint8_t *buf);

/* ---- Bit field extraction ---- */

/**
 * extract_bits — Extract a bitfield from a 32-bit value.
 *
 * Example: extract_bits(0x1234, 4, 8) extracts bits [11:4]
 *
 * @param val    Source value
 * @param offset Bit offset from LSB
 * @param width  Number of bits to extract
 * @return       Extracted bitfield, right-aligned
 */
uint32_t extract_bits(uint32_t val, uint8_t offset, uint8_t width);

/**
 * count_set_bits — Count the number of 1-bits (popcount) in a byte.
 * Useful for parity checking and Hamming weight calculations.
 */
uint8_t count_set_bits(uint8_t val);

#endif /* BIT_UTILS_H */
