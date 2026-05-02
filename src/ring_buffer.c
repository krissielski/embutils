/**
 * ring_buffer.c — Circular buffer implementation
 */

#include "ring_buffer.h"

void rb_init(RingBuffer *rb, uint8_t *buf, size_t capacity)
{
    rb->buf      = buf;
    rb->capacity = capacity;
    rb->head     = 0;
    rb->tail     = 0;
    rb->count    = 0;
}

bool rb_push(RingBuffer *rb, uint8_t byte)
{
    if (rb_is_full(rb)) {
        return false;
    }
    rb->buf[rb->head] = byte;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return true;
}

bool rb_pop(RingBuffer *rb, uint8_t *out)
{
    if (rb_is_empty(rb)) {
        return false;
    }
    *out = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return true;
}

bool rb_peek(const RingBuffer *rb, uint8_t *out)
{
    if (rb_is_empty(rb)) {
        return false;
    }
    *out = rb->buf[rb->tail];
    return true;
}

size_t rb_count(const RingBuffer *rb)
{
    return rb->count;
}

bool rb_is_empty(const RingBuffer *rb)
{
    return rb->count == 0;
}

bool rb_is_full(const RingBuffer *rb)
{
    return rb->count == rb->capacity;
}

void rb_clear(RingBuffer *rb)
{
    rb->head  = 0;
    rb->tail  = 0;
    rb->count = 0;
}
