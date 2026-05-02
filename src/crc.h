/**
 * crc.h — CRC checksum functions
 *
 * CRC (Cyclic Redundancy Check) is used in embedded systems to validate
 * data integrity. Common uses:
 *   - Validating received UART/SPI/I2C packets
 *   - Verifying flash memory contents
 *   - Checking firmware images before execution
 *
 * This module provides:
 *   - CRC-8  (Dallas/Maxim, polynomial 0x31, refin=true)  — used in 1-Wire, some sensors
 *   - CRC-16 (CCITT, polynomial 0x1021)      — used in XMODEM, USB, HDLC
 */

#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * crc8 — Calculate CRC-8 (Dallas/Maxim, poly=0x31, init=0x00)
 *
 * @param data  Pointer to input data
 * @param len   Number of bytes to process
 * @return      8-bit CRC value
 */
uint8_t crc8(const uint8_t *data, size_t len);

/**
 * crc8_update — Update a running CRC-8 with additional bytes.
 * Useful for streaming data or multi-segment buffers.
 *
 * @param crc   Previous CRC value (start with 0x00)
 * @param data  Pointer to input data
 * @param len   Number of bytes to process
 * @return      Updated CRC-8 value
 */
uint8_t crc8_update(uint8_t crc, const uint8_t *data, size_t len);

/**
 * crc16 — Calculate CRC-16/CCITT-FALSE (poly=0x1021, init=0xFFFF)
 *
 * @param data  Pointer to input data
 * @param len   Number of bytes to process
 * @return      16-bit CRC value
 */
uint16_t crc16(const uint8_t *data, size_t len);

/**
 * crc16_update — Update a running CRC-16 with additional bytes.
 *
 * @param crc   Previous CRC value (start with 0xFFFF)
 * @param data  Pointer to input data
 * @param len   Number of bytes to process
 * @return      Updated CRC-16 value
 */
uint16_t crc16_update(uint16_t crc, const uint8_t *data, size_t len);

#endif /* CRC_H */
