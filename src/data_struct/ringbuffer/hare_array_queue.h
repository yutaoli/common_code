#ifndef HARE_ARRAY_QUEUE_H
#define HARE_ARRAY_QUEUE_H

typedef enum hare_queue_offer_result_s
{
    HARE_OFFER_SUCCESS = 0,
    HARE_OFFER_ERROR = -2,
    HARE_OFFER_FULL = -1
} hare_queue_offer_result_t;

typedef void (*hare_queue_drain_func_t)(void *clientd, void *item);

#endif //HARE_ARRAY_QUEUE_H
