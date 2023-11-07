#include "hare_mpsc_array_queue.h"

#include <stdlib.h>

int hare_mpsc_array_queue_init(hare_mpsc_array_queue_t *queue, size_t length)
{
    length = (size_t)hare_find_next_power_of_two((int32_t)length);

    queue->buffer = (void **)calloc(1, sizeof(void *) * length);
    if (NULL == queue->buffer)
    {
        return -1;
    }

    for (size_t i = 0; i < length; i++)
    {
        queue->buffer[i] = NULL;
    }

    queue->capacity = length;
    queue->mask = length - 1;
    queue->producer.head_cache = 0;
    queue->producer.shared_head_cache = 0;
    HARE_PUT_ORDERED(queue->producer.tail, (uint64_t)0);
    HARE_PUT_ORDERED(queue->consumer.head, (uint64_t)0);

    return 0;
}

int hare_mpsc_array_queue_close(hare_mpsc_array_queue_t *queue)
{
    free((void *)queue->buffer);
    return 0;
}

extern hare_queue_offer_result_t hare_mpsc_array_queue_offer(
    hare_mpsc_array_queue_t *queue, void *element);

extern size_t hare_mpsc_array_queue_drain(
    hare_mpsc_array_queue_t *queue, hare_queue_drain_func_t func, void *clientd, size_t limit);

extern size_t hare_mpsc_array_queue_drain_all(
    hare_mpsc_array_queue_t *queue, hare_queue_drain_func_t func, void *clientd);

extern size_t hare_mpsc_array_queue_size(hare_mpsc_array_queue_t *queue);
