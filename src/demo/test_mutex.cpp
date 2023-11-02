#include <mutex>
#include <thread>
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

constexpr  unsigned int g_count_size = 10;
std::mutex g_mtx;
unsigned int count[g_count_size];

void works()
{
 //   g_mtx.lock();
    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]++;
    }
    // TODO

  //  g_mtx.unlock();
}

void works_mtx()
{
    g_mtx.lock();
    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]++;
    }
    // TODO

    g_mtx.unlock();
}

void works_lock_guard()
{
    std::lock_guard<std::mutex> guard(g_mtx);
    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]++;
    }
    // TODO

}

int TestMutex(int type)
{
    timediff_t timediff(__FUNCTION__);

    unsigned int times = 3000;
    memset(count, 0 , sizeof(count));

    std::thread threads[times];
    for(unsigned int i = 0; i < times; i++)
    {
        if(type == 1)
        {
         threads[i] = std::thread(works);
        }
        else if(type == 2)
        {
            threads[i] = std::thread(works_mtx);
        }
        else if(type == 3)
        {
            threads[i] = std::thread(works_lock_guard);
        }

    }

    for(unsigned int i = 0; i < times; i++)
    {
        threads[i].join();
    }

    for(unsigned int i = 0; i < g_count_size; i++)
    {
        printf("count[%d]:%u\n",i,count[i]);
    }


    return 0;
}

int main()
{
    TestMutex(1);// works
    //TestMutex(2);// works_mtx
    //TestMutex(3);// works_lock_guard
    return 0;
}
/*
   works function:
count[0]:9950
count[1]:9965
count[2]:9985
count[3]:9934
count[4]:9968
count[5]:9972
count[6]:9974
count[7]:9982
count[8]:9984
count[9]:9985
TestMutex,timediff:sec:0,nsec:266314602

works_mtx function:
count[0]:10000
count[1]:10000
count[2]:10000
count[3]:10000
count[4]:10000
count[5]:10000
count[6]:10000
count[7]:10000
count[8]:10000
count[9]:10000
TestMutex,timediff:sec:5,nsec:199959625

works_lock_guard function:
count[0]:10000
count[1]:10000
count[2]:10000
count[3]:10000
count[4]:10000
count[5]:10000
count[6]:10000
count[7]:10000
count[8]:10000
count[9]:10000
TestMutex,timediff:sec:5,nsec:192880154
   */
