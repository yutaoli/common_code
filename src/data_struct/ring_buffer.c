#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>

//#include "mq_errno.h"
#include "ring_buffer.h"
//#include "log.h"
//#include "mq_util.h"

static int full(ring_buffer *queue)
{
    return (queue->tail + 1 == queue->head)
        || (queue->head == 0 && queue->tail == queue->size - 1);
    //return (queue->tail + 1) % queue->size == queue->head;
}

static int empty(ring_buffer *queue)
{
    return queue->tail == queue->head;
}

int ring_buffer_init(ring_buffer *queue, int size, unsigned char need_producer_lock, unsigned char need_consumer_lock)
{
    if(queue == NULL || size <=0) return -1;

    queue->head = 0;
    queue->tail = 0;
    queue->size = size + 1;// 满队时必须空一个位置。有n个元素入队，空间至少要n+1
    
    queue->items = (void **)malloc(sizeof(void *) * queue->size);
    if(queue->items == NULL) 
    {
        //ERROR_LOG("malloc queue items fail");
        printf("malloc queue items fail\n");
        return -2;
    }

    queue->need_producer_lock = need_producer_lock;
    if(need_producer_lock)
        pthread_spin_init(&queue->producer_lock, PTHREAD_PROCESS_PRIVATE);

    queue->need_consumer_lock = need_consumer_lock;
    if(need_consumer_lock)
        pthread_spin_init(&queue->consumer_lock, PTHREAD_PROCESS_PRIVATE);

    //DEBUG_LOG("%s,size:%d,need_producer_lock:%d,need_consumer_lock:%d",__FUNCTION__,size,need_producer_lock,need_consumer_lock);
    printf("%s,size:%d,need_producer_lock:%d,need_consumer_lock:%d\n",__FUNCTION__,size,need_producer_lock,need_consumer_lock);
    return 0;
}
int ring_buffer_reset(ring_buffer *queue)
{
    queue->head = 0;
    queue->tail = 0;
    return 0;
}
int ring_buffer_fini(ring_buffer *queue)
{
    if(queue->items != NULL) 
    {
        free(queue->items);
        queue->items = NULL;
    }

    if(queue->need_producer_lock)
        pthread_spin_destroy(&queue->producer_lock);

    if(queue->need_consumer_lock)
        pthread_spin_destroy(&queue->consumer_lock);
    return 0;
}

int ring_buffer_pop(ring_buffer *queue, void **item)
{
    if(item == NULL)
    {
        return -1;
    }

    *item = NULL;
    if(empty(queue))
    {
        return ERR_RING_BUFFER_EMPTY;
    }

    if(queue->need_consumer_lock) pthread_spin_lock(&queue->consumer_lock);

    *item = queue->items[queue->head];
    queue->head++;
    if(queue->head == queue->size)  queue->head= 0;

    if(queue->need_consumer_lock) pthread_spin_unlock(&queue->consumer_lock);
    return 0;
}
int ring_buffer_push(ring_buffer *queue, void *item)
{
    if(full(queue)) return ERR_RING_BUFFER_FULL;

    if(queue->need_producer_lock) pthread_spin_lock(&queue->producer_lock);

    queue->items[queue->tail] = item;
    queue->tail++;
    if(queue->tail == queue->size) queue->tail = 0;

    if(queue->need_producer_lock) pthread_spin_unlock(&queue->producer_lock);
    return 0;
}
int ring_buffer_size(ring_buffer *queue)
{
    if(queue->tail  >= queue->head) 
        return queue->tail - queue->head;
    else 
        return queue->tail + queue->size - queue->head;
}
