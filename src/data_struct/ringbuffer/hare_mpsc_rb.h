#ifndef HARE_MPSC_RB_H
#define HARE_MPSC_RB_H

#include "hare_rb.h"

typedef struct hare_mpsc_rb_stct
{
    uint8_t *buffer;
    hare_rb_descriptor_t *descriptor;
    size_t capacity;
    size_t max_message_length;
}
hare_mpsc_rb_t;

int hare_mpsc_rb_init(hare_mpsc_rb_t *ring_buffer, void *buffer, size_t length);

hare_rb_write_result_t hare_mpsc_rb_write(
    hare_mpsc_rb_t *ring_buffer,
    int32_t msg_type_id,
    const void *msg,
    size_t length);

int32_t hare_mpsc_rb_try_claim(hare_mpsc_rb_t *ring_buffer, int32_t msg_type_id, size_t length);

int hare_mpsc_rb_commit(hare_mpsc_rb_t *ring_buffer, int32_t offset);

int hare_mpsc_rb_abort(hare_mpsc_rb_t *ring_buffer, int32_t offset);

size_t hare_mpsc_rb_read(
    hare_mpsc_rb_t *ring_buffer,
    hare_rb_handler_t handler,
    void *clientd,
    size_t message_count_limit);

int64_t hare_mpsc_rb_next_correlation_id(hare_mpsc_rb_t *ring_buffer);

void hare_mpsc_rb_consumer_heartbeat_time(hare_mpsc_rb_t *ring_buffer, int64_t now_ms);
int64_t hare_mpsc_rb_consumer_heartbeat_time_value(hare_mpsc_rb_t *ring_buffer);

bool hare_mpsc_rb_unblock(hare_mpsc_rb_t *ring_buffer);

inline int64_t hare_mpsc_rb_consumer_position(hare_mpsc_rb_t *ring_buffer)
{
    int64_t position;
    HARE_GET_VOLATILE(position, ring_buffer->descriptor->head_position);
    return position;
}

inline int64_t hare_mpsc_rb_producer_position(hare_mpsc_rb_t *ring_buffer)
{
    int64_t position;
    HARE_GET_VOLATILE(position, ring_buffer->descriptor->tail_position);
    return position;
}

#endif //HARE_MPSC_RB_H
