#include <errno.h>
#include <inttypes.h>
#include "hare_mpsc_rb.h"

int hare_mpsc_rb_init(hare_mpsc_rb_t *ring_buffer, void *buffer, size_t length)
{
    const size_t capacity = length - HARE_RB_TRAILER_LENGTH;
    int result = -1;

    if (HARE_RB_IS_CAPACITY_VALID(capacity))
    {
        ring_buffer->buffer = (uint8_t *)buffer;
        ring_buffer->capacity = capacity;
        ring_buffer->descriptor = (hare_rb_descriptor_t *)(ring_buffer->buffer + ring_buffer->capacity);
        ring_buffer->max_message_length = HARE_RB_MAX_MESSAGE_LENGTH(ring_buffer->capacity);
        result = 0;
    }

    return result;
}

inline static int32_t hare_mpsc_rb_claim_capacity(hare_mpsc_rb_t *ring_buffer, const size_t record_length)
{
    const size_t required_capacity = HARE_ALIGN(record_length, HARE_RB_ALIGNMENT);
    const size_t mask = ring_buffer->capacity - 1;
    int64_t head = 0;
    int64_t tail = 0;
    size_t tail_index = 0;
    size_t padding = 0;

    HARE_GET_VOLATILE(head, ring_buffer->descriptor->head_cache_position);

    do
    {
        HARE_GET_VOLATILE(tail, ring_buffer->descriptor->tail_position);

        int32_t available_capacity;
        available_capacity = (int32_t)ring_buffer->capacity - (int32_t)(tail - head);

        if ((int32_t)required_capacity > available_capacity)
        {
            HARE_GET_VOLATILE(head, ring_buffer->descriptor->head_position);

            if (required_capacity > (ring_buffer->capacity - (int32_t)(tail - head)))
            {
                return -1;
            }

            HARE_PUT_ORDERED(ring_buffer->descriptor->head_cache_position, head);
        }

        padding = 0;
        tail_index = (size_t)(tail & mask);
        size_t to_buffer_end_length = ring_buffer->capacity - tail_index;

        if (required_capacity > to_buffer_end_length)
        {
            size_t head_index = (size_t)(head & mask);

            if (required_capacity > head_index)
            {
                HARE_GET_VOLATILE(head, ring_buffer->descriptor->head_position);
                head_index = (size_t)(head & mask);

                if (required_capacity > head_index)
                {
                    return -1;
                }

                HARE_PUT_ORDERED(ring_buffer->descriptor->head_cache_position, head);
            }

            padding = to_buffer_end_length;
        }
    }
    while (!hare_cas_int64(
        &(ring_buffer->descriptor->tail_position),
        tail,
        tail + (int32_t)required_capacity + (int32_t)padding));

    if (0 != padding)
    {
        hare_rb_record_descriptor_t *record_header =
            (hare_rb_record_descriptor_t *)(ring_buffer->buffer + tail_index);
        HARE_PUT_ORDERED(record_header->length, -(int32_t)padding);

        record_header->msg_type_id = HARE_RB_PADDING_MSG_TYPE_ID;
        HARE_PUT_ORDERED(record_header->length, (int32_t)padding);
        tail_index = 0;
    }

    return (int32_t)tail_index;
}

hare_rb_write_result_t hare_mpsc_rb_write(
    hare_mpsc_rb_t *ring_buffer, int32_t msg_type_id, const void *msg, size_t length)
{
    if (length > ring_buffer->max_message_length || HARE_RB_INVALID_MSG_TYPE_ID(msg_type_id))
    {
        return HARE_RB_ERROR;
    }

    const size_t record_length = length + HARE_RB_RECORD_HEADER_LENGTH;
    const int32_t record_index = hare_mpsc_rb_claim_capacity(ring_buffer, record_length);

    if (-1 != record_index)
    {
        hare_rb_record_descriptor_t *record_header =
            (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);
        HARE_PUT_ORDERED(record_header->length, -(int32_t)record_length);

        memcpy(ring_buffer->buffer + HARE_RB_MESSAGE_OFFSET(record_index), msg, length);
        record_header->msg_type_id = msg_type_id;
        HARE_PUT_ORDERED(record_header->length, (int32_t)record_length);

        return HARE_RB_SUCCESS;
    }

    return HARE_RB_FULL;
}

int32_t hare_mpsc_rb_try_claim(hare_mpsc_rb_t *ring_buffer, int32_t msg_type_id, size_t length)
{
    if (length > ring_buffer->max_message_length || HARE_RB_INVALID_MSG_TYPE_ID(msg_type_id))
    {
        return HARE_RB_ERROR;
    }

    const size_t record_length = length + HARE_RB_RECORD_HEADER_LENGTH;
    const int32_t record_index = hare_mpsc_rb_claim_capacity(ring_buffer, record_length);
    if (-1 != record_index)
    {
        hare_rb_record_descriptor_t *record_header =
            (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);
        HARE_PUT_ORDERED(record_header->length, -(int32_t)record_length);
        record_header->msg_type_id = msg_type_id;

        return HARE_RB_MESSAGE_OFFSET(record_index);
    }

    return HARE_RB_FULL;
}

int hare_mpsc_rb_commit(hare_mpsc_rb_t *ring_buffer, int32_t offset)
{
    const int32_t record_index = offset - HARE_RB_RECORD_HEADER_LENGTH;
    if (record_index < 0 || record_index > (int32_t)(ring_buffer->capacity - HARE_RB_RECORD_HEADER_LENGTH))
    {
        return -1;
    }

    hare_rb_record_descriptor_t *record_header = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);
    const int32_t length = record_header->length;
    if (length < 0)
    {
        HARE_PUT_ORDERED(record_header->length, -length);
        return 0;
    }

    return -1;
}

int hare_mpsc_rb_abort(hare_mpsc_rb_t *ring_buffer, int32_t offset)
{
    const int32_t record_index = offset - HARE_RB_RECORD_HEADER_LENGTH;
    if (record_index < 0 || record_index > (int32_t)(ring_buffer->capacity - HARE_RB_RECORD_HEADER_LENGTH))
    {
        return -1;
    }

    hare_rb_record_descriptor_t *record_header = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);
    const int32_t length = record_header->length;
    if (length < 0)
    {
        record_header->msg_type_id = HARE_RB_PADDING_MSG_TYPE_ID;
        HARE_PUT_ORDERED(record_header->length, -length);
        return 0;
    }

    return -1;
}

size_t hare_mpsc_rb_read(
    hare_mpsc_rb_t *ring_buffer, hare_rb_handler_t handler, void *clientd, size_t message_count_limit)
{
    const int64_t head = ring_buffer->descriptor->head_position;
    const size_t head_index = (size_t)(head & (ring_buffer->capacity - 1));
    const size_t contiguous_block_length = ring_buffer->capacity - head_index;
    size_t messages_read = 0;
    size_t bytes_read = 0;

    while ((bytes_read < contiguous_block_length) && (messages_read < message_count_limit))
    {
        const size_t record_index = head_index + bytes_read;
        hare_rb_record_descriptor_t *header = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);

        int32_t record_length;
        HARE_GET_VOLATILE(record_length, header->length);

        if (record_length <= 0)
        {
            break;
        }

        bytes_read += HARE_ALIGN(record_length, HARE_RB_ALIGNMENT);
        int32_t msg_type_id = header->msg_type_id;

        if (HARE_RB_PADDING_MSG_TYPE_ID == msg_type_id)
        {
            continue;
        }

        ++messages_read;
        handler(
            msg_type_id,
            ring_buffer->buffer + HARE_RB_MESSAGE_OFFSET(record_index),
            record_length - HARE_RB_RECORD_HEADER_LENGTH,
            clientd);
    }

    if (0 != bytes_read)
    {
        memset(ring_buffer->buffer + head_index, 0, bytes_read);
        HARE_PUT_ORDERED(ring_buffer->descriptor->head_position, head + bytes_read);
    }

    return messages_read;
}

int64_t hare_mpsc_rb_next_correlation_id(hare_mpsc_rb_t *ring_buffer)
{
    int64_t result;
    HARE_GET_AND_ADD_INT64(result, ring_buffer->descriptor->correlation_counter, 1);
    return result;
}

void hare_mpsc_rb_consumer_heartbeat_time(hare_mpsc_rb_t *ring_buffer, int64_t now_ms)
{
    HARE_PUT_ORDERED(ring_buffer->descriptor->consumer_heartbeat, now_ms);
}

int64_t hare_mpsc_rb_consumer_heartbeat_time_value(hare_mpsc_rb_t *ring_buffer)
{
    int64_t value;
    HARE_GET_VOLATILE(value, ring_buffer->descriptor->consumer_heartbeat);
    return value;
}

inline static bool scan_back_to_confirm_still_zeroed(const uint8_t *buffer, size_t from, size_t limit)
{
    size_t i = from - HARE_RB_ALIGNMENT;
    bool all_zeroes = true;

    while (i >= limit)
    {
        const hare_rb_record_descriptor_t *record = (hare_rb_record_descriptor_t *)(buffer + i);
        int32_t length;
        HARE_GET_VOLATILE(length, record->length);
        if (0 != length)
        {
            all_zeroes = false;
            break;
        }

        i -= HARE_RB_ALIGNMENT;
    }

    return all_zeroes;
}

bool hare_mpsc_rb_unblock(hare_mpsc_rb_t *ring_buffer)
{
    int64_t head;
    HARE_GET_VOLATILE(head, ring_buffer->descriptor->head_position);
    int64_t tail;
    HARE_GET_VOLATILE(tail, ring_buffer->descriptor->tail_position);

    if (head == tail)
    {
        return false;
    }

    bool unblocked = false;
    const size_t mask = ring_buffer->capacity - 1;
    size_t consumer_index = (size_t)(head & mask);
    size_t producer_index = (size_t)(tail & mask);

    int32_t length;
    hare_rb_record_descriptor_t *record = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + consumer_index);

    HARE_GET_VOLATILE(length, record->length);
    if (length < 0)
    {
        record->msg_type_id = HARE_RB_PADDING_MSG_TYPE_ID;
        HARE_PUT_ORDERED(record->length, -length);
        unblocked = true;
    }
    else if (0 == length)
    {
        const size_t limit = producer_index > consumer_index ? producer_index : ring_buffer->capacity;
        size_t i = consumer_index + HARE_RB_ALIGNMENT;

        do
        {
            record = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + i);
            HARE_GET_VOLATILE(length, record->length);
            if (0 != length)
            {
                if (scan_back_to_confirm_still_zeroed(ring_buffer->buffer, i, consumer_index))
                {
                    record = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + consumer_index);
                    record->msg_type_id = HARE_RB_PADDING_MSG_TYPE_ID;
                    HARE_PUT_ORDERED(record->length, (int32_t)(i - consumer_index));
                    unblocked = true;
                }

                break;
            }

            i += HARE_RB_ALIGNMENT;
        }
        while (i < limit);
    }

    return unblocked;
}

extern int64_t hare_mpsc_rb_consumer_position(hare_mpsc_rb_t *ring_buffer);

extern int64_t hare_mpsc_rb_producer_position(hare_mpsc_rb_t *ring_buffer);
