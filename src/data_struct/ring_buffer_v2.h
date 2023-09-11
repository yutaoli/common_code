#ifndef RINGBUFFER_V2_H
#define RINGBUFFER_V2_H

#define ERR_RING_BUFFER_V2_FULL -100
#define ERR_RING_BUFFER_V2_EMPTY -101

#include "hare_spsc_rb.h"

template <typename T>
class RingBufferSpScV2
{
public:
RingBufferSpScV2()
{
    buf_ = NULL;
    buf_len_ = 0;
    pop_msg_type_id_ = 0;
    push_msg_type_id_ = 0;
    error_ = false;
    init_ = false;
}
virtual ~ RingBufferSpScV2()
{
    Fini();
}

public:
//size:为分多少Byte内存，只调用一次
int Init(unsigned int size)
{
    if(init_) return 0;

    init_ = true;

    buf_len_ = size;
    buf_ = (char *)malloc(size);
    if(buf_ == NULL) return -1;

    memset(buf_, 0, buf_len_);
    return hare_spsc_rb_init(&ring_buffer_, buf_, buf_len_);
}

// 可以调用N次
int Reset()
{
    if(buf_ != NULL)
    {
        memset(buf_, 0, buf_len_);
    }
    return 0;
}

int Pop(T &item)
{
    if(buf_ == NULL) return -1;

    size_t message_count_limit = 1;
    size_t read_count = hare_spsc_rb_read(&ring_buffer_, OnRecv, this, message_count_limit);
    if(read_count == 0)
    {
        return ERR_RING_BUFFER_V2_EMPTY;
    }

    if(error_) return -2;

    // pop_msg_type_id_ = 
    memcpy((void*) & item, (void*)&data_, sizeof(data_));
    return 0;
}
int Push(const T &item)
{
    if(buf_ == NULL) return -1;

    push_msg_type_id_++;
    hare_rb_write_result_t ret = hare_spsc_rb_write(&ring_buffer_, push_msg_type_id_, &item, sizeof(item));
    if(ret == HARE_RB_SUCCESS) return 0;
    else if(ret == HARE_RB_FULL) return ERR_RING_BUFFER_V2_FULL;

    return -2;
}
//int Size();
private:
int Fini()
{
    if(buf_ != NULL)
    {
        free(buf_);
        buf_ = NULL;
    }
    buf_len_ = 0;
    return 0;
}
static void OnRecv(int32_t msg_type_id, const void *buf, size_t size, void *arg)
{
    RingBufferSpScV2 *ring_buffer = (RingBufferSpScV2*)arg;
    bool &error = ring_buffer->error_;

    error = false;
    if(sizeof(ring_buffer->data_) != size) 
    {
        error = true;
        return;
    }

    ring_buffer->pop_msg_type_id_ = msg_type_id;
    memcpy((void *)&(ring_buffer->data_),buf, size);
}
private:
char *buf_;
unsigned int buf_len_;
hare_spsc_rb_t ring_buffer_;
T data_;
int32_t pop_msg_type_id_;
int32_t push_msg_type_id_;

bool error_;
bool init_;
};
/*
RingBufferSpScV2<T> queDelay;
queDelay.Init((1<<11) + HARE_RB_TRAILER_LENGTH);

T data;
ret = queDelay.Push(data);
if(ret == ERR_RING_BUFFER_V2_FULL) ...

queDelay.Pop(data);
if(ret == ERR_RING_BUFFER_V2_EMPTY) ...

queDelay.Reset();

queDelay.Fini();
*/
#endif