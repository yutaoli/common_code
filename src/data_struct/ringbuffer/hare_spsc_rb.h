#ifndef HARE_SPSC_RB_H
#define HARE_SPSC_RB_H

#include "hare_rb.h"

#if !defined(_MSC_VER)
#include <sys/uio.h>
#else
struct iovec
{
    void  *iov_base;
    size_t iov_len;
};
#endif

typedef struct hare_spsc_rb_stct
{
    uint8_t *buffer;
    hare_rb_descriptor_t *descriptor;
    size_t capacity;
    size_t max_message_length;
}
hare_spsc_rb_t;

int hare_spsc_rb_init(hare_spsc_rb_t *ring_buffer, void *buffer, size_t length);

hare_rb_write_result_t hare_spsc_rb_write(
    hare_spsc_rb_t *ring_buffer, int32_t msg_type_id, const void *msg, size_t length);

hare_rb_write_result_t hare_spsc_rb_writev(
    hare_spsc_rb_t *ring_buffer, int32_t msg_type_id, const struct iovec* iov, int iovcnt);

int32_t hare_spsc_rb_try_claim(hare_spsc_rb_t *ring_buffer, int32_t msg_type_id, size_t length);

int hare_spsc_rb_commit(hare_spsc_rb_t *ring_buffer, int32_t offset);

int hare_spsc_rb_abort(hare_spsc_rb_t *ring_buffer, int32_t offset);

size_t hare_spsc_rb_read(
    hare_spsc_rb_t *ring_buffer, hare_rb_handler_t handler, void *clientd, size_t message_count_limit);

int64_t hare_spsc_rb_next_correlation_id(hare_spsc_rb_t *ring_buffer);

void hare_spsc_rb_consumer_heartbeat_time(hare_spsc_rb_t *ring_buffer, int64_t time_ms);

#endif //HARE_SPSC_RB_H
