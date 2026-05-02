/**
 * test_ring_buffer.c — Unity tests for the ring buffer module
 */

#include "unity.h"
#include "../src/ring_buffer.h"

/* Unity requires these even if empty */
void setUp(void)    {}
void tearDown(void) {}

/* ---- Initialization ---- */

void test_rb_init_starts_empty(void)
{
    RingBuffer rb;
    uint8_t buf[8];
    rb_init(&rb, buf, sizeof(buf));

    TEST_ASSERT_EQUAL(0, rb_count(&rb));
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
    TEST_ASSERT_FALSE(rb_is_full(&rb));
}

/* ---- Push and Pop ---- */

void test_rb_push_single_byte(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    rb_init(&rb, buf, sizeof(buf));

    TEST_ASSERT_TRUE(rb_push(&rb, 0xAB));
    TEST_ASSERT_EQUAL(1, rb_count(&rb));
    TEST_ASSERT_FALSE(rb_is_empty(&rb));
}

void test_rb_pop_single_byte(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    uint8_t out = 0;
    rb_init(&rb, buf, sizeof(buf));

    rb_push(&rb, 0xAB);
    TEST_ASSERT_TRUE(rb_pop(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0xFF, out);      //Was 0xAB
    TEST_ASSERT_EQUAL(0, rb_count(&rb));
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
}

void test_rb_fifo_ordering(void)
{
    RingBuffer rb;
    uint8_t buf[8];
    uint8_t out = 0;
    rb_init(&rb, buf, sizeof(buf));

    /* Push three bytes */
    rb_push(&rb, 0x01);
    rb_push(&rb, 0x02);
    rb_push(&rb, 0x03);

    /* They must come out in the same order */
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x01, out);
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x02, out);
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x03, out);
}

/* ---- Full buffer ---- */

void test_rb_push_fails_when_full(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    rb_init(&rb, buf, sizeof(buf));

    /* Fill it up */
    TEST_ASSERT_TRUE(rb_push(&rb, 0x01));
    TEST_ASSERT_TRUE(rb_push(&rb, 0x02));
    TEST_ASSERT_TRUE(rb_push(&rb, 0x03));
    TEST_ASSERT_TRUE(rb_push(&rb, 0x04));
    TEST_ASSERT_TRUE(rb_is_full(&rb));

    /* One more should fail */
    TEST_ASSERT_FALSE(rb_push(&rb, 0x05));
    TEST_ASSERT_EQUAL(4, rb_count(&rb));  /* count unchanged */
}

/* ---- Empty buffer ---- */

void test_rb_pop_fails_when_empty(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    uint8_t out = 0xDD;  /* sentinel value */
    rb_init(&rb, buf, sizeof(buf));

    TEST_ASSERT_FALSE(rb_pop(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0xDD, out);  /* out should be unchanged */
}

/* ---- Peek ---- */

void test_rb_peek_does_not_remove(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    uint8_t out = 0;
    rb_init(&rb, buf, sizeof(buf));

    rb_push(&rb, 0x55);
    TEST_ASSERT_TRUE(rb_peek(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0x55, out);
    TEST_ASSERT_EQUAL(1, rb_count(&rb));  /* still 1 byte */

    /* peek again - same result */
    TEST_ASSERT_TRUE(rb_peek(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0x55, out);
}

void test_rb_peek_fails_when_empty(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    uint8_t out = 0xDD;
    rb_init(&rb, buf, sizeof(buf));

    TEST_ASSERT_FALSE(rb_peek(&rb, &out));
    TEST_ASSERT_EQUAL_HEX8(0xDD, out);  /* unchanged */
}

/* ---- Wrap-around (the key ring buffer property) ---- */

void test_rb_wraps_around_correctly(void)
{
    RingBuffer rb;
    uint8_t buf[4];
    uint8_t out = 0;
    rb_init(&rb, buf, sizeof(buf));

    /* Fill then drain halfway — this moves tail forward */
    rb_push(&rb, 0xAA);
    rb_push(&rb, 0xBB);
    rb_pop(&rb, &out);   /* consume 0xAA, tail is now at index 1 */
    rb_pop(&rb, &out);   /* consume 0xBB, tail is now at index 2 */

    /* Now fill past the end of the array — head wraps around */
    rb_push(&rb, 0x11);
    rb_push(&rb, 0x22);
    rb_push(&rb, 0x33);
    rb_push(&rb, 0x44);
    TEST_ASSERT_TRUE(rb_is_full(&rb));

    /* Read them all back in order */
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x11, out);
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x22, out);
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x33, out);
    rb_pop(&rb, &out); TEST_ASSERT_EQUAL_HEX8(0x44, out);
    TEST_ASSERT_TRUE(rb_is_empty(&rb));
}

/* ---- Clear ---- */

void test_rb_clear_empties_buffer(void)
{
    RingBuffer rb;
    uint8_t buf[8];
    rb_init(&rb, buf, sizeof(buf));

    rb_push(&rb, 0x01);
    rb_push(&rb, 0x02);
    rb_push(&rb, 0x03);
    TEST_ASSERT_EQUAL(3, rb_count(&rb));

    rb_clear(&rb);
    TEST_ASSERT_EQUAL(0, rb_count(&rb));
    TEST_ASSERT_TRUE(rb_is_empty(&rb));

    /* Should be usable again after clear */
    TEST_ASSERT_TRUE(rb_push(&rb, 0xFF));
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_rb_init_starts_empty);
    RUN_TEST(test_rb_push_single_byte);
    RUN_TEST(test_rb_pop_single_byte);
    RUN_TEST(test_rb_fifo_ordering);
    RUN_TEST(test_rb_push_fails_when_full);
    RUN_TEST(test_rb_pop_fails_when_empty);
    RUN_TEST(test_rb_peek_does_not_remove);
    RUN_TEST(test_rb_peek_fails_when_empty);
    RUN_TEST(test_rb_wraps_around_correctly);
    RUN_TEST(test_rb_clear_empties_buffer);

    return UNITY_END();
}
