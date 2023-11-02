#include <stdio.h>
#include <pthread.h> 
//#include <threads.h>
#include <stdint.h>
#include <string>
#include <map>
#include <string.h>

typedef struct timediff
{
    timediff(std::string prompt_)
    {
        prompt = prompt_;
        begin.tv_sec = 0;
        begin.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &begin);
    }

    ~ timediff()
    {
        end.tv_sec = 0;
        end.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &end);

        if(end.tv_nsec > begin.tv_nsec)
        {
            printf("%s,timediff:sec:%lu,nsec:%lu\n",prompt.c_str(),end.tv_sec-begin.tv_sec, end.tv_nsec- begin.tv_nsec);
        }
        else 
        {
            printf("%s,timediff:sec:%lu,nsec:%lu\n",prompt.c_str(),end.tv_sec-begin.tv_sec-1, 1000000000UL + end.tv_nsec- begin.tv_nsec);
        }
    }

    struct timespec begin;
    struct timespec end;
    std::string prompt;
    /*
       uint64_t getnowtime64()
       {
       struct timespec now={0,0};
       clock_gettime(CLOCK_REALTIME, &now);
       return now.tv_sec << 32 | (now.tv_nsec & 0xffffffff);
       }
       */
}timediff_t;


pthread_spinlock_t lock;
pthread_mutex_t sleeplock;

int sleeplock_count=0;
int spinlock_count = 0;
int non_spinlock_count = 0;			//non-atomic variable

std::map<int,int> int2int;

#define COUNT 1000
#define THREAD_COUNT 1000
void * sleeplock_func(void* input)
{
    for(int n = 0; n < COUNT; ++n) {
        //atomic_fetch_add(&atomic_count, 1); // atomic updation

        pthread_mutex_lock(&sleeplock);
        sleeplock_count++;
        pthread_mutex_unlock(&sleeplock);
    }
    return NULL;
}
void * spinlock_func(void* input)
{
    for(int n = 0; n < COUNT; ++n) {
        //atomic_fetch_add(&atomic_count, 1); // atomic updation

        pthread_spin_lock(&lock);
        spinlock_count++;
        pthread_spin_unlock(&lock);
    }
    return NULL;
}

void * non_spinlock_func(void* input)
{
    for(int n = 0; n < COUNT; ++n) {
        //atomic_fetch_add(&atomic_count, 1); // atomic updation

        ++non_spinlock_count; // non-atomic updation
    }
    return NULL;
}

void * map_int2int_func(void* input)
{
    for(int n = 0; n < COUNT * THREAD_COUNT; ++n) {
        //atomic_fetch_add(&atomic_count, 1); // atomic updation

        // pthread_spin_lock(&lock);
        int2int[n] = n;
        // pthread_spin_unlock(&lock);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&sleeplock, NULL);
    pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);

    int ret = 0;
    {
        timediff_t timdiff("sleeplock");
        pthread_t  sleeplock_threads[THREAD_COUNT];
        for(int i =0; i < THREAD_COUNT; i++)
        {
            ret = pthread_create(&sleeplock_threads[i], NULL,sleeplock_func, NULL);
            if(ret != 0)
            {
                printf("pthread_create ret:%d,errno:%d,msg:%s\n",ret,errno, strerror(errno));
                continue;
            }
        }

        for(int i = 0; i < THREAD_COUNT; i++)
        {
            ret = pthread_join(sleeplock_threads[i], NULL);
            if(ret != 0)
            {
                printf("pthread_join ret:%d\n",ret);
                continue;
            }
        }
    }

    {
        timediff_t timdiff("spinlock");
        pthread_t  spinlock_threads[THREAD_COUNT];
        for(int i =0; i < THREAD_COUNT; i++)
        {
            ret = pthread_create(&spinlock_threads[i], NULL,spinlock_func, NULL);
            if(ret != 0)
            {
                printf("pthread_create ret:%d,errno:%d,msg:%s\n",ret,errno, strerror(errno));
                continue;
            }
        }

        for(int i = 0; i < THREAD_COUNT; i++)
        {
            ret = pthread_join(spinlock_threads[i], NULL);
            if(ret != 0)
            {
                printf("pthread_join ret:%d\n",ret);
                continue;
            }
        }

        //printf("after spinlock:sec:%lu,nsec:%lu\n",now64 >> 32, now64 & 0xffffffff);
    }

    {
        timediff_t timdiff("non_spinlock");
        pthread_t  non_spinlock_threads[THREAD_COUNT];
        for(int i =0; i < THREAD_COUNT; i++)
        {
            ret = pthread_create(&non_spinlock_threads[i], NULL,non_spinlock_func, NULL);
            if(ret != 0)
            {
                printf("pthread_create ret:%d,errno:%d,msg:%s\n",ret,errno, strerror(errno));
                continue;
            }
        }

        for(int i = 0; i < THREAD_COUNT; i++)
        {
            ret = pthread_join(non_spinlock_threads[i], NULL);
            if(ret != 0)
            {
                printf("pthread_join ret:%d\n",ret);
                continue;
            }
        }
    }

    {
        timediff_t timdiff("map int2int");
        /*
           thrd_t map_int2int_threads[THREAD_COUNT];
           for(int i =0; i < THREAD_COUNT; i++)
           thrd_create(&map_int2int_threads[i], map_int2int_func, NULL);

           for(int i = 0; i < THREAD_COUNT; i++)
           thrd_join(map_int2int_threads[i], NULL);
           */

        pthread_t  map_int2int_threads;
        ret = pthread_create(&map_int2int_threads, NULL,map_int2int_func, NULL);
        if(ret != 0)
        {
            printf("pthread_create ret:%d,errno:%d,msg:%s\n",ret,errno, strerror(errno));
        }

        ret = pthread_join(map_int2int_threads, NULL);
        if(ret != 0)
        {
            printf("pthread_join ret:%d\n",ret);
        }
    }

    int a = 0;
    {
        timediff_t timdiff("1 cmd & a outsite");

        a = 1;
    }

    {
        timediff_t timdiff("1 cmd & a insite");

        int a = 0;
        a = 1;
    }

    {
        timediff_t timdiff("2 cmd");

        int i = 0;
        i++;
    }

    {
        timediff_t timdiff("100 cmd");

        int a = 0;
        for(int i = 0; i < 100; i++)
        {
            a++;
        }
    }

    {
        timediff_t timdiff("1000 cmd");

        int a = 0;
        for(int i = 0; i < 1000; i++)
        {
            a++;
        }
    }


    {
        timediff_t timdiff("10000 cmd & a outsite");

        for(int i = 0; i < 10000; i++)
        {
            a++;
        }
    }

    {
        timediff_t timdiff("10000 cmd & a inside");

        int a = 0;
        for(int i = 0; i < 10000; i++)
        {
            a++;
        }
    }

    {
        timediff_t timdiff("10000 cmd && 2 init");

        int a = 0;
        int b = 0;
        for(int i = 0; i < 10000; i++)
        {
            a++;
        }
    }

    {
        timediff_t timdiff("10000 cmd && 3 init");

        int a = 0;
        int b = 0;
        int c = 0;
        for(int i = 0; i < 10000; i++)
        {
            a++;
        }
    }

    {
        timediff_t timdiff("10000 cmd && 4 init");

        int a = 0;
        int b = 0;
        int c = 0;
        int d = 0;
        for(int i = 0; i < 10000; i++)
        {
            a++;
        }
    }


    printf("The sleeplock counter is: %u\n", sleeplock_count);
    printf("The splinlock counter is: %u\n", spinlock_count);
    printf("The non-spinlock counter is: %d\n", non_spinlock_count);
    return 0;
}
