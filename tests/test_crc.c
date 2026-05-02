/**
 * test_crc.c — Unity tests for the CRC module
 *
 * Known-good reference values were generated with:
 *   Python: crcmod library, or online CRC calculators
 *
 * CRC-8  : poly=0x31, init=0x00, refin=false, refout=false, xorout=0x00
 * CRC-16 : poly=0x1021, init=0xFFFF, refin=false, refout=false, xorout=0x0000
 *          (CRC-16/CCITT-FALSE)
 */

#include "unity.h"
#include "../src/crc.h"

void setUp(void)    {}
void tearDown(void) {}

/* ---- CRC-8 ---- */

void test_crc8_empty_input(void)
{
    /* Zero-length input must return init value 0x00 */
    TEST_ASSERT_EQUAL_HEX8(0x00, crc8(NULL, 0));
}

void test_crc8_single_zero_byte(void)
{
    const uint8_t data[] = { 0x00 };
    /* CRC-8/Dallas of {0x00} = 0x00 */
    TEST_ASSERT_EQUAL_HEX8(0x00, crc8(data, 1));
}

void test_crc8_single_byte(void)
{
    const uint8_t data[] = { 0xAB };
    /* Reference value verified with Python CRC-8/Dallas implementation */
    TEST_ASSERT_EQUAL_HEX8(0x38, crc8(data, 1));
}

void test_crc8_known_sequence(void)
{
    /* "123456789" → CRC-8/Dallas (reflected, poly=0x31) = 0x07 */
    const uint8_t data[] = { '1','2','3','4','5','6','7','8','9' };
    TEST_ASSERT_EQUAL_HEX8(0x07, crc8(data, sizeof(data)));
}

void test_crc8_two_byte_sequence(void)
{
    const uint8_t data[] = { 0x01, 0x02 };
    /* Reference: crc8({0x01,0x02}) = 0x18 */
    TEST_ASSERT_EQUAL_HEX8(0x18, crc8(data, sizeof(data)));
}

/* ---- CRC-8 streaming (crc8_update) ---- */

void test_crc8_update_matches_single_call(void)
{
    const uint8_t part1[] = { '1','2','3','4' };
    const uint8_t part2[] = { '5','6','7','8','9' };
    const uint8_t full[]  = { '1','2','3','4','5','6','7','8','9' };

    uint8_t streaming = crc8_update(0x00, part1, sizeof(part1));
    streaming         = crc8_update(streaming, part2, sizeof(part2));

    TEST_ASSERT_EQUAL_HEX8(crc8(full, sizeof(full)), streaming);
}

void test_crc8_update_single_call_equivalent(void)
{
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    uint8_t result1 = crc8(data, sizeof(data));
    uint8_t result2 = crc8_update(0x00, data, sizeof(data));
    TEST_ASSERT_EQUAL_HEX8(result1, result2);
}

/* ---- CRC-16 ---- */

void test_crc16_empty_input(void)
{
    /* Zero-length input must return init value 0xFFFF */
    TEST_ASSERT_EQUAL_HEX16(0xFFFF, crc16(NULL, 0));
}

void test_crc16_known_sequence(void)
{
    /* "123456789" → CRC-16/CCITT-FALSE = 0x29B1 */
    const uint8_t data[] = { '1','2','3','4','5','6','7','8','9' };
    TEST_ASSERT_EQUAL_HEX16(0x29B1, crc16(data, sizeof(data)));
}

void test_crc16_single_byte(void)
{
    const uint8_t data[] = { 0x00 };
    /* CRC-16/CCITT-FALSE of {0x00} = 0xE1F0 */
    TEST_ASSERT_EQUAL_HEX16(0xE1F0, crc16(data, 1));
}

void test_crc16_two_identical_buffers_same_result(void)
{
    const uint8_t a[] = { 0x11, 0x22, 0x33, 0x44 };
    const uint8_t b[] = { 0x11, 0x22, 0x33, 0x44 };
    TEST_ASSERT_EQUAL_HEX16(crc16(a, sizeof(a)), crc16(b, sizeof(b)));
}

void test_crc16_different_data_different_result(void)
{
    const uint8_t a[] = { 0x01, 0x02 };
    const uint8_t b[] = { 0x01, 0x03 };
    TEST_ASSERT_NOT_EQUAL(crc16(a, sizeof(a)), crc16(b, sizeof(b)));
}

/* ---- CRC-16 streaming (crc16_update) ---- */

void test_crc16_update_matches_single_call(void)
{
    const uint8_t part1[] = { '1','2','3','4' };
    const uint8_t part2[] = { '5','6','7','8','9' };
    const uint8_t full[]  = { '1','2','3','4','5','6','7','8','9' };

    uint16_t streaming = crc16_update(0xFFFF, part1, sizeof(part1));
    streaming          = crc16_update(streaming, part2, sizeof(part2));

    TEST_ASSERT_EQUAL_HEX16(crc16(full, sizeof(full)), streaming);
}

void test_crc16_update_single_call_equivalent(void)
{
    const uint8_t data[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    uint16_t result1 = crc16(data, sizeof(data));
    uint16_t result2 = crc16_update(0xFFFF, data, sizeof(data));
    TEST_ASSERT_EQUAL_HEX16(result1, result2);
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_crc8_empty_input);
    RUN_TEST(test_crc8_single_zero_byte);
    RUN_TEST(test_crc8_single_byte);
    RUN_TEST(test_crc8_known_sequence);
    RUN_TEST(test_crc8_two_byte_sequence);
    RUN_TEST(test_crc8_update_matches_single_call);
    RUN_TEST(test_crc8_update_single_call_equivalent);

    RUN_TEST(test_crc16_empty_input);
    RUN_TEST(test_crc16_known_sequence);
    RUN_TEST(test_crc16_single_byte);
    RUN_TEST(test_crc16_two_identical_buffers_same_result);
    RUN_TEST(test_crc16_different_data_different_result);
    RUN_TEST(test_crc16_update_matches_single_call);
    RUN_TEST(test_crc16_update_single_call_equivalent);

    return UNITY_END();
}
