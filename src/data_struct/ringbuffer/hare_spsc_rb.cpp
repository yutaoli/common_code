#include "hare_spsc_rb.h"

int hare_spsc_rb_init(hare_spsc_rb_t *ring_buffer, void *buffer, size_t length)
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

hare_rb_write_result_t hare_spsc_rb_write(
    hare_spsc_rb_t *ring_buffer, int32_t msg_type_id, const void *msg, size_t length)
{
    struct iovec vec[1];
    vec[0].iov_len = length;
    vec[0].iov_base = (void *)msg;

    return hare_spsc_rb_writev(ring_buffer, msg_type_id, vec, 1);
}

inline static int32_t hare_spsc_rb_claim_capacity(hare_spsc_rb_t *ring_buffer, const size_t record_length)
{
    const size_t aligned_record_length = HARE_ALIGN(record_length, HARE_RB_ALIGNMENT);
    const size_t required_capacity = aligned_record_length + HARE_RB_RECORD_HEADER_LENGTH;
    const size_t mask = ring_buffer->capacity - 1;

    int64_t head = ring_buffer->descriptor->head_cache_position;
    int64_t tail = ring_buffer->descriptor->tail_position;
    const int32_t available_capacity = (int32_t)ring_buffer->capacity - (int32_t)(tail - head);

    size_t padding = 0;
    size_t record_index = (size_t)tail & mask;
    const size_t to_buffer_end_length = ring_buffer->capacity - record_index;
    hare_rb_record_descriptor_t *record_header, *next_header = NULL;

    if ((int32_t)required_capacity > available_capacity)
    {
        HARE_GET_VOLATILE(head, ring_buffer->descriptor->head_position);

        if (required_capacity > (ring_buffer->capacity - (size_t)(tail - head)))
        {
            return -1;
        }

        ring_buffer->descriptor->head_cache_position = head;
    }

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

            ring_buffer->descriptor->head_cache_position = head;
        }

        padding = to_buffer_end_length;
    }

    HARE_PUT_ORDERED(ring_buffer->descriptor->tail_position, tail + aligned_record_length + padding);

    if (0 != padding)
    {
        record_header = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);
        next_header = (hare_rb_record_descriptor_t *)ring_buffer->buffer;

        next_header->length = 0;
        next_header->msg_type_id = 0;
        HARE_PUT_ORDERED(record_header->length, -(int32_t)padding);
        record_header->msg_type_id = HARE_RB_PADDING_MSG_TYPE_ID;
        HARE_PUT_ORDERED(record_header->length, (int32_t)padding);
        record_index = 0;
    }

    next_header = (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index + aligned_record_length);

    next_header->length = 0;
    next_header->msg_type_id = 0;

    return (int32_t)record_index;
}

hare_rb_write_result_t hare_spsc_rb_writev(
    hare_spsc_rb_t *ring_buffer, int32_t msg_type_id, const struct iovec *iov, int iovcnt)
{
    size_t length = 0;
    for (int i = 0; i < iovcnt; i++)
    {
        length += iov[i].iov_len;
    }

    if (length > ring_buffer->max_message_length || HARE_RB_INVALID_MSG_TYPE_ID(msg_type_id))
    {
        return HARE_RB_ERROR;
    }

    const size_t record_length = length + HARE_RB_RECORD_HEADER_LENGTH;
    const int32_t record_index = hare_spsc_rb_claim_capacity(ring_buffer, record_length);
    if (-1 != record_index)
    {
        hare_rb_record_descriptor_t *record_header =
            (hare_rb_record_descriptor_t *)(ring_buffer->buffer + record_index);
        HARE_PUT_ORDERED(record_header->length, -(int32_t)record_length);

        size_t current_vector_offset = 0;
        for (int i = 0; i < iovcnt; i++)
        {
            uint8_t *offset = ring_buffer->buffer + HARE_RB_MESSAGE_OFFSET(record_index) + current_vector_offset;
            memcpy(offset, iov[i].iov_base, iov[i].iov_len);
            current_vector_offset += iov[i].iov_len;
        }

        record_header->msg_type_id = msg_type_id;
        HARE_PUT_ORDERED(record_header->length, (int32_t)record_length);

        return HARE_RB_SUCCESS;
    }

    return HARE_RB_FULL;
}

int32_t hare_spsc_rb_try_claim(hare_spsc_rb_t *ring_buffer, int32_t msg_type_id, size_t length)
{
    if (length > ring_buffer->max_message_length || HARE_RB_INVALID_MSG_TYPE_ID(msg_type_id))
    {
        return HARE_RB_ERROR;
    }

    const size_t record_length = length + HARE_RB_RECORD_HEADER_LENGTH;
    const int32_t record_index = hare_spsc_rb_claim_capacity(ring_buffer, record_length);
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

int hare_spsc_rb_commit(hare_spsc_rb_t *ring_buffer, int32_t offset)
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

int hare_spsc_rb_abort(hare_spsc_rb_t *ring_buffer, int32_t offset)
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

size_t hare_spsc_rb_read(
    hare_spsc_rb_t *ring_buffer, hare_rb_handler_t handler, void *clientd, size_t message_count_limit)
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
        HARE_PUT_ORDERED(ring_buffer->descriptor->head_position, head + bytes_read);
    }

    return messages_read;
}

int64_t hare_spsc_rb_next_correlation_id(hare_spsc_rb_t *ring_buffer)
{
    int64_t result;
    HARE_GET_AND_ADD_INT64(result, ring_buffer->descriptor->correlation_counter, 1);
    return result;
}

void hare_spsc_rb_consumer_heartbeat_time(hare_spsc_rb_t *ring_buffer, int64_t time_ms)
{
    HARE_PUT_ORDERED(ring_buffer->descriptor->consumer_heartbeat, time_ms);
}
