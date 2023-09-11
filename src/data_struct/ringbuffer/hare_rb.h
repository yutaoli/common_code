#ifndef HARE_RB_H
#define HARE_RB_H

#include <string.h>
#include "hare_bitutil.h"
#include "hare_rb_atomic.h"

#ifndef HARE_CACHE_LINE_LENGTH
#define HARE_CACHE_LINE_LENGTH 64
#endif

#pragma pack(push)
#pragma pack(4)
typedef struct hare_rb_descriptor_stct
{
    uint8_t begin_pad[(2 * HARE_CACHE_LINE_LENGTH)];
    volatile int64_t tail_position;
    uint8_t tail_pad[(2 * HARE_CACHE_LINE_LENGTH) - sizeof(int64_t)];
    int64_t head_cache_position;
    uint8_t head_cache_pad[(2 * HARE_CACHE_LINE_LENGTH) - sizeof(int64_t)];
    volatile int64_t head_position;
    uint8_t head_pad[(2 * HARE_CACHE_LINE_LENGTH) - sizeof(int64_t)];
    volatile int64_t correlation_counter;
    uint8_t correlation_counter_pad[(2 * HARE_CACHE_LINE_LENGTH) - sizeof(int64_t)];
    int64_t consumer_heartbeat;
    uint8_t consumer_heartbeat_pad[(2 * HARE_CACHE_LINE_LENGTH) - sizeof(int64_t)];
}
hare_rb_descriptor_t;

typedef struct hare_rb_record_descriptor_stct
{
    volatile int32_t length;
    int32_t msg_type_id;
}
hare_rb_record_descriptor_t;
#pragma pack(pop)

#define HARE_RB_TRAILER_LENGTH (sizeof(hare_rb_descriptor_t))

#define HARE_RB_ALIGNMENT (2 * sizeof(int32_t))

#define HARE_RB_MESSAGE_OFFSET(index) (index + sizeof(hare_rb_record_descriptor_t))
#define HARE_RB_RECORD_HEADER_LENGTH (sizeof(hare_rb_record_descriptor_t))

#define HARE_RB_MAX_MESSAGE_LENGTH(capacity) (capacity / 8)
#define HARE_RB_INVALID_MSG_TYPE_ID(id) (id < -1)
#define HARE_RB_PADDING_MSG_TYPE_ID (-1)

typedef enum hare_rb_write_result_stct
{
    HARE_RB_SUCCESS = 0,
    HARE_RB_ERROR = -2,
    HARE_RB_FULL = -1
}
hare_rb_write_result_t;

typedef void (*hare_rb_handler_t)(int32_t, const void *, size_t, void *);

#define HARE_RB_IS_CAPACITY_VALID(capacity) HARE_IS_POWER_OF_TWO(capacity)

#endif //HARE_RB_H
