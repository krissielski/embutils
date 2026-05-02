/**
 * ring_buffer.h — Circular buffer (ring buffer / FIFO queue)
 *
 * A ring buffer is one of the most fundamental data structures in embedded
 * systems. It is used everywhere: UART receive buffers, DMA ping-pong
 * buffers, inter-task communication queues, audio sample buffers.
 *
 * This implementation is:
 *   - Fixed size (no dynamic memory allocation)
 *   - Interrupt-safe for single-producer / single-consumer use
 *   - Portable C99 with no dependencies
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * RingBuffer — the buffer control structure.
 * Allocate one of these plus a backing byte array.
 *
 * Example:
 *   uint8_t storage[64];
 *   RingBuffer rb;
 *   rb_init(&rb, storage, sizeof(storage));
 */
typedef struct {
    uint8_t *buf;       /* pointer to backing storage */
    size_t   capacity;  /* total capacity in bytes    */
    size_t   head;      /* write index                */
    size_t   tail;      /* read index                 */
    size_t   count;     /* number of bytes stored     */
} RingBuffer;

/**
 * rb_init — Initialize a ring buffer with external storage.
 *
 * @param rb       Pointer to RingBuffer control struct
 * @param buf      Pointer to backing byte array
 * @param capacity Size of the backing array in bytes
 */
void rb_init(RingBuffer *rb, uint8_t *buf, size_t capacity);

/**
 * rb_push — Write one byte into the buffer.
 *
 * @return true on success, false if the buffer is full
 */
bool rb_push(RingBuffer *rb, uint8_t byte);

/**
 * rb_pop — Read one byte from the buffer.
 *
 * @param out  Pointer to store the retrieved byte
 * @return true on success, false if the buffer is empty
 */
bool rb_pop(RingBuffer *rb, uint8_t *out);

/**
 * rb_peek — Read the next byte WITHOUT removing it from the buffer.
 *
 * @param out  Pointer to store the peeked byte
 * @return true on success, false if the buffer is empty
 */
bool rb_peek(const RingBuffer *rb, uint8_t *out);

/**
 * rb_count — Return the number of bytes currently stored.
 */
size_t rb_count(const RingBuffer *rb);

/**
 * rb_is_empty — Return true if the buffer contains no bytes.
 */
bool rb_is_empty(const RingBuffer *rb);

/**
 * rb_is_full — Return true if the buffer is at capacity.
 */
bool rb_is_full(const RingBuffer *rb);

/**
 * rb_clear — Discard all data in the buffer.
 */
void rb_clear(RingBuffer *rb);

#endif /* RING_BUFFER_H */
