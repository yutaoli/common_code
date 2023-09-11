#ifndef HARE_SPSC_ARRAY_QUEUE_H
#define HARE_SPSC_ARRAY_QUEUE_H

#include "hare_bitutil.h"
#include "hare_rb_atomic.h"
#include "hare_array_queue.h"

typedef struct hare_spsc_array_queue_stct
{
    int8_t padding[HARE_CACHE_LINE_LENGTH - sizeof(uint64_t)];
    struct
    {
        volatile uint64_t tail;
        uint64_t head_cache;
        int8_t padding[HARE_CACHE_LINE_LENGTH - (2 * sizeof(uint64_t))];
    } producer;

    struct
    {
        volatile uint64_t head;
        int8_t padding[HARE_CACHE_LINE_LENGTH - sizeof(uint64_t)];
    } consumer;

    size_t capacity;
    size_t mask;
    volatile void **buffer;
}
hare_spsc_array_queue_t;

int hare_spsc_array_queue_init(hare_spsc_array_queue_t *queue, size_t length);

int hare_spsc_array_queue_close(hare_spsc_array_queue_t *queue);

inline hare_queue_offer_result_t hare_spsc_array_queue_offer(
    hare_spsc_array_queue_t *queue, void *element)
{
    if (NULL == element)
    {
        return HARE_OFFER_ERROR;
    }

    uint64_t current_head = queue->producer.head_cache;
    uint64_t buffer_limit = current_head + queue->capacity;
    uint64_t current_tail = queue->producer.tail;

    if (current_tail >= buffer_limit)
    {
        HARE_GET_VOLATILE(current_head, queue->consumer.head);
        buffer_limit = current_head + queue->capacity;

        if (current_tail >= buffer_limit)
        {
            return HARE_OFFER_FULL;
        }

        queue->producer.head_cache = current_head;
    }

    const size_t index = (size_t)(current_tail & queue->mask);

    HARE_PUT_ORDERED(queue->buffer[index], element);
    HARE_PUT_ORDERED(queue->producer.tail, current_tail + 1);

    return HARE_OFFER_SUCCESS;
}

inline void *hare_spsc_array_queue_poll(hare_spsc_array_queue_t *queue)
{
    const uint64_t current_head = queue->consumer.head;
    const size_t index = (size_t)(current_head & queue->mask);

    volatile void *item;
    HARE_GET_VOLATILE(item, queue->buffer[index]);

    if (NULL != item)
    {
        HARE_PUT_ORDERED(queue->buffer[index], NULL);
        HARE_PUT_ORDERED(queue->consumer.head, current_head + 1);
    }

    return (void *)item;
}

inline size_t hare_spsc_array_queue_drain(
    hare_spsc_array_queue_t *queue, hare_queue_drain_func_t func, void *clientd, size_t limit)
{
    uint64_t current_head = queue->consumer.head;
    uint64_t next_sequence = current_head;
    const uint64_t limit_sequence = next_sequence + limit;

    while (next_sequence < limit_sequence)
    {
        const size_t index = (size_t)(next_sequence & queue->mask);
        volatile void *item;
        HARE_GET_VOLATILE(item, queue->buffer[index]);

        if (NULL == item)
        {
            break;
        }

        HARE_PUT_ORDERED(queue->buffer[index], NULL);
        next_sequence++;
        HARE_PUT_ORDERED(queue->consumer.head, next_sequence);
        func(clientd, (void *)item);
    }

    return next_sequence - current_head;
}

inline size_t hare_spsc_array_queue_drain_all(
    hare_spsc_array_queue_t *queue, hare_queue_drain_func_t func, void *clientd)
{
    uint64_t current_head = queue->consumer.head;
    uint64_t current_tail;
    HARE_GET_VOLATILE(current_tail, queue->producer.tail);

    return hare_spsc_array_queue_drain(queue, func, clientd, current_tail - current_head);
}

inline size_t hare_spsc_array_queue_size(hare_spsc_array_queue_t *queue)
{
    uint64_t current_head_before;
    uint64_t current_tail;
    uint64_t current_head_after;

    HARE_GET_VOLATILE(current_head_after, queue->consumer.head);

    do
    {
        current_head_before = current_head_after;
        HARE_GET_VOLATILE(current_tail, queue->producer.tail);
        HARE_GET_VOLATILE(current_head_after, queue->consumer.head);
    }
    while (current_head_after != current_head_before);

    size_t size = (size_t)(current_tail - current_head_after);
    if ((int64_t)size < 0)
    {
        return 0;
    }
    else if (size > queue->capacity)
    {
        return queue->capacity;
    }

    return size;
}

#endif //HARE_SPSC_ARRAY_QUEUE_H
