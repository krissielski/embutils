/**
 * test_bit_utils.c — Unity tests for the bit_utils module
 */

#include "unity.h"
#include "../src/bit_utils.h"

void setUp(void)    {}
void tearDown(void) {}

/* ---- BIT_SET ---- */

void test_bit_set_single_bit(void)
{
    uint8_t val = 0x00;
    BIT_SET(val, 3);
    TEST_ASSERT_EQUAL_HEX8(0x08, val);
}

void test_bit_set_already_set(void)
{
    uint8_t val = 0xFF;
    BIT_SET(val, 5);
    TEST_ASSERT_EQUAL_HEX8(0xFF, val);  /* no change */
}

void test_bit_set_bit0(void)
{
    uint8_t val = 0x00;
    BIT_SET(val, 0);
    TEST_ASSERT_EQUAL_HEX8(0x01, val);
}

void test_bit_set_bit7(void)
{
    uint8_t val = 0x00;
    BIT_SET(val, 7);
    TEST_ASSERT_EQUAL_HEX8(0x80, val);
}

/* ---- BIT_CLEAR ---- */

void test_bit_clear_single_bit(void)
{
    uint8_t val = 0xFF;
    BIT_CLEAR(val, 3);
    TEST_ASSERT_EQUAL_HEX8(0xF7, val);
}

void test_bit_clear_already_clear(void)
{
    uint8_t val = 0x00;
    BIT_CLEAR(val, 4);
    TEST_ASSERT_EQUAL_HEX8(0x00, val);  /* no change */
}

/* ---- BIT_TOGGLE ---- */

void test_bit_toggle_sets_when_clear(void)
{
    uint8_t val = 0x00;
    BIT_TOGGLE(val, 2);
    TEST_ASSERT_EQUAL_HEX8(0x04, val);
}

void test_bit_toggle_clears_when_set(void)
{
    uint8_t val = 0xFF;
    BIT_TOGGLE(val, 2);
    TEST_ASSERT_EQUAL_HEX8(0xFB, val);
}

void test_bit_toggle_twice_restores_original(void)
{
    uint8_t val = 0xA5;
    BIT_TOGGLE(val, 6);
    BIT_TOGGLE(val, 6);
    TEST_ASSERT_EQUAL_HEX8(0xA5, val);
}

/* ---- BIT_TEST ---- */

void test_bit_test_set_bit_returns_nonzero(void)
{
    uint8_t val = 0x10;
    TEST_ASSERT_TRUE(BIT_TEST(val, 4));
}

void test_bit_test_clear_bit_returns_zero(void)
{
    uint8_t val = 0x10;
    TEST_ASSERT_FALSE(BIT_TEST(val, 3));
}

/* ---- pack_u16_be / unpack_u16_be ---- */

void test_pack_u16_be_known_value(void)
{
    uint8_t buf[2] = {0};
    pack_u16_be(buf, 0x1234);
    TEST_ASSERT_EQUAL_HEX8(0x12, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0x34, buf[1]);
}

void test_pack_u16_be_zero(void)
{
    uint8_t buf[2] = { 0xFF, 0xFF };
    pack_u16_be(buf, 0x0000);
    TEST_ASSERT_EQUAL_HEX8(0x00, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, buf[1]);
}

void test_pack_u16_be_max(void)
{
    uint8_t buf[2] = {0};
    pack_u16_be(buf, 0xFFFF);
    TEST_ASSERT_EQUAL_HEX8(0xFF, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, buf[1]);
}

void test_unpack_u16_be_known_value(void)
{
    const uint8_t buf[] = { 0x12, 0x34 };
    TEST_ASSERT_EQUAL_HEX16(0x1234, unpack_u16_be(buf));
}

void test_pack_unpack_u16_be_roundtrip(void)
{
    uint16_t original = 0xABCD;
    uint8_t  buf[2];
    pack_u16_be(buf, original);
    TEST_ASSERT_EQUAL_HEX16(original, unpack_u16_be(buf));
}

/* ---- pack_u32_be / unpack_u32_be ---- */

void test_pack_u32_be_known_value(void)
{
    uint8_t buf[4] = {0};
    pack_u32_be(buf, 0x12345678);
    TEST_ASSERT_EQUAL_HEX8(0x12, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0x34, buf[1]);
    TEST_ASSERT_EQUAL_HEX8(0x56, buf[2]);
    TEST_ASSERT_EQUAL_HEX8(0x78, buf[3]);
}

void test_unpack_u32_be_known_value(void)
{
    const uint8_t buf[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    TEST_ASSERT_EQUAL_HEX32(0xDEADBEEF, unpack_u32_be(buf));
}

void test_pack_unpack_u32_be_roundtrip(void)
{
    uint32_t original = 0xCAFEBABE;
    uint8_t  buf[4];
    pack_u32_be(buf, original);
    TEST_ASSERT_EQUAL_HEX32(original, unpack_u32_be(buf));
}

/* ---- extract_bits ---- */

void test_extract_bits_full_byte(void)
{
    /* Extract all 8 bits from offset 0 */
    TEST_ASSERT_EQUAL_HEX32(0xAB, extract_bits(0xAB, 0, 8));
}

void test_extract_bits_nibble(void)
{
    /* Extract lower nibble of 0xAB → 0x0B */
    TEST_ASSERT_EQUAL_HEX32(0x0B, extract_bits(0xAB, 0, 4));
}

void test_extract_bits_upper_nibble(void)
{
    /* Extract upper nibble of 0xAB → 0x0A */
    TEST_ASSERT_EQUAL_HEX32(0x0A, extract_bits(0xAB, 4, 4));
}

void test_extract_bits_single_bit(void)
{
    TEST_ASSERT_EQUAL_UINT32(1, extract_bits(0x08, 3, 1));
    TEST_ASSERT_EQUAL_UINT32(0, extract_bits(0x08, 2, 1));
}

void test_extract_bits_mid_field(void)
{
    /* 0x1234 = 0001 0010 0011 0100
       Extract bits [11:4] → 0x23 */
    TEST_ASSERT_EQUAL_HEX32(0x23, extract_bits(0x1234, 4, 8));
}

/* ---- count_set_bits ---- */

void test_count_set_bits_zero(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, count_set_bits(0x00));
}

void test_count_set_bits_all_ones(void)
{
    TEST_ASSERT_EQUAL_UINT8(8, count_set_bits(0xFF));
}

void test_count_set_bits_alternating(void)
{
    TEST_ASSERT_EQUAL_UINT8(4, count_set_bits(0xAA));  /* 10101010 */
    TEST_ASSERT_EQUAL_UINT8(4, count_set_bits(0x55));  /* 01010101 */
}

void test_count_set_bits_single_bit(void)
{
    TEST_ASSERT_EQUAL_UINT8(1, count_set_bits(0x01));
    TEST_ASSERT_EQUAL_UINT8(1, count_set_bits(0x80));
}

void test_count_set_bits_known_value(void)
{
    /* 0xB7 = 1011 0111 → 6 bits set */
    TEST_ASSERT_EQUAL_UINT8(6, count_set_bits(0xB7));
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_bit_set_single_bit);
    RUN_TEST(test_bit_set_already_set);
    RUN_TEST(test_bit_set_bit0);
    RUN_TEST(test_bit_set_bit7);

    RUN_TEST(test_bit_clear_single_bit);
    RUN_TEST(test_bit_clear_already_clear);

    RUN_TEST(test_bit_toggle_sets_when_clear);
    RUN_TEST(test_bit_toggle_clears_when_set);
    RUN_TEST(test_bit_toggle_twice_restores_original);

    RUN_TEST(test_bit_test_set_bit_returns_nonzero);
    RUN_TEST(test_bit_test_clear_bit_returns_zero);

    RUN_TEST(test_pack_u16_be_known_value);
    RUN_TEST(test_pack_u16_be_zero);
    RUN_TEST(test_pack_u16_be_max);
    RUN_TEST(test_unpack_u16_be_known_value);
    RUN_TEST(test_pack_unpack_u16_be_roundtrip);

    RUN_TEST(test_pack_u32_be_known_value);
    RUN_TEST(test_unpack_u32_be_known_value);
    RUN_TEST(test_pack_unpack_u32_be_roundtrip);

    RUN_TEST(test_extract_bits_full_byte);
    RUN_TEST(test_extract_bits_nibble);
    RUN_TEST(test_extract_bits_upper_nibble);
    RUN_TEST(test_extract_bits_single_bit);
    RUN_TEST(test_extract_bits_mid_field);

    RUN_TEST(test_count_set_bits_zero);
    RUN_TEST(test_count_set_bits_all_ones);
    RUN_TEST(test_count_set_bits_alternating);
    RUN_TEST(test_count_set_bits_single_bit);
    RUN_TEST(test_count_set_bits_known_value);

    return UNITY_END();
}
