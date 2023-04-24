// /usr/src/kernels/3.10.0-1127.19.1.el7.x86_64/include/linux/kfifo.h 
#ifndef RINGBUFFER_H
#define RINGBUFFER_H 

#include <pthread.h>

#define ERR_RING_BUFFER_FULL -100
#define ERR_RING_BUFFER_EMPTY -101

typedef struct ring_buffer
{
// [head, tail)
// init:head = 0;tail = 0
// empty:head==tail
// full:(tail+1)%size == head
// 空队时head等于tail,满队时必须空一个位置。有n个元素入队，空间至少要n+1
int size;
int head;
int tail;
void **items;
unsigned char need_producer_lock;// 0 or 1
unsigned char need_consumer_lock;// 0 or 1

pthread_spinlock_t producer_lock;
pthread_spinlock_t consumer_lock;
}ring_buffer;

//size:只能容纳size个item
int ring_buffer_init(ring_buffer *queue, int size, unsigned char need_producer_lock, unsigned char need_consumer_lock);
int ring_buffer_reset(ring_buffer *queue);
int ring_buffer_fini(ring_buffer *queue);

// in:queue,item
// out:ret,item
//  sucess:ret==0,top item store in *item
//  fail:ret!=0,*item undefine
int ring_buffer_pop(ring_buffer *queue, void **item);
int ring_buffer_push(ring_buffer *queue, void *item);
int ring_buffer_size(ring_buffer *queue);

#endif
