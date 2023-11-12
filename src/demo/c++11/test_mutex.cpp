#include <mutex>
#include <thread>
#include <string.h>
#include <atomic>
#include <vector>

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
int count[g_count_size];

std::atomic<int> atomic_count[g_count_size];

void works()
{
 //   g_mtx.lock();
    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]++;
    }

    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]--;
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

    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]--;
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

    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        count[i]--;
    }
    // TODO

}

void works_atomic()
{
    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        atomic_count[i]++;
    }

    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        atomic_count[i]--;
    }
}

void works_atomic_relaxed()
{
    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        atomic_count[i].fetch_add(1, std::memory_order_relaxed);
    }

    for(unsigned int i = 0; i < g_count_size; i++)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds{1000});
        atomic_count[i].fetch_sub(1, std::memory_order_relaxed);
    }
}

int TestMutex(int type)
{
    {
        timediff_t timediff(__FUNCTION__);

        unsigned int times = 10000;
        memset(count, 0, sizeof(count));

        for(unsigned int i = 0; i < g_count_size;i++)
        {
            atomic_count[i].store(0);
        }

        std::thread threads[times];
        for (unsigned int i = 0; i < times; i++)
        {
            if (type == 1)
            {
                threads[i] = std::thread(works);
            }
            else if (type == 2)
            {
                threads[i] = std::thread(works_mtx);
            }
            else if (type == 3)
            {
                threads[i] = std::thread(works_lock_guard);
            }
            else if (type == 4)
            {
                threads[i] = std::thread(works_atomic);
            }
            else if (type == 5)
            {
                threads[i] = std::thread(works_atomic_relaxed);
            } 
        }

        for (unsigned int i = 0; i < times; i++)
        {
            threads[i].join();
        }

        for (unsigned int i = 0; i < g_count_size; i++)
        {
            if (type == 1 || type == 2 || type == 3)
            {
                printf("count[%d]:%d\n", i, count[i]);
            }
            else if (type == 4)
            {
                printf("atomic_count[%d]:%d\n", i, atomic_count[i].load());
            }
            else if (type == 5)
            {
                printf("atomic_count relaxed[%d]:%d\n", i, atomic_count[i].load());
            }
        }
    }
    printf("%s,type:%d end\n\n", __FUNCTION__, type);

    return 0;
}

int TestNormalAddAndSub()
{
    {
        timediff_t timediff(__FUNCTION__);

        unsigned int left = 2;
        unsigned int total = 100000;
        unsigned int thread_num = 10000;
        unsigned int counts[g_count_size];
        for (unsigned int i = 0; i < g_count_size; i++)
        {
            counts[i] = 0;
        }

        while (left--)
        {
            for (unsigned int i = 0; i < g_count_size; i++)
            {
                counts[i] += total; // 每个格子+total
            }

            std::vector<std::thread> threads;
            threads.resize(thread_num);
            for (unsigned int i = 0; i < thread_num; i++)
            {
                threads[i] = std::thread([&]
                                         {
                for(unsigned int count_i = 0; count_i < g_count_size; count_i++)
                {// 每个格子，每个线程都 -1

                    std::this_thread::sleep_for(std::chrono::nanoseconds{1000});// +1
                    counts[count_i]++;

                    std::this_thread::sleep_for(std::chrono::nanoseconds{1000});// -2
                    counts[count_i]-=2;
                 } });
            }
            for (unsigned int i = 0; i < g_count_size; i++)
            {
                counts[i] -= (total - thread_num); // 每个格子-(total - thread_num)
            }
            for (unsigned int i = 0; i < thread_num; i++)
            {
                if (threads[i].joinable())
                {
                    threads[i].join();
                }
            }
        }

        for (unsigned int i = 0; i < g_count_size; i++)
        {
            printf("count[%d]:%d\n", i, counts[i]); // 每个格子都是0
        }
    }
    printf("%s end\n\n", __FUNCTION__);
    return 0;
}

int TestAtomicFetchAddAndFetchSub()
{
    {
        timediff_t timediff(__FUNCTION__);

        unsigned int left = 2;
        unsigned int total = 100000;
        unsigned int thread_num = 10000;
        std::atomic<unsigned int> counts[g_count_size];
        for(unsigned int i = 0; i < g_count_size; i++)
        {
            counts[i].store(0);
        }

        while (left--)
        {
            for (unsigned int i = 0; i < g_count_size; i++)
            {
                counts[i].fetch_add(total);// 每个格子+total
            }

            std::vector<std::thread> threads;
            threads.resize(thread_num);
            for (unsigned int i = 0; i < thread_num; i++)
            {
                threads[i] = std::thread([&]
                                         {
                for(unsigned int count_i = 0; count_i < g_count_size; count_i++)
                {// 每个格子，每个线程都 -1

                    std::this_thread::sleep_for(std::chrono::nanoseconds{1000});// +1
                    counts[count_i]++;

                    std::this_thread::sleep_for(std::chrono::nanoseconds{1000});// -2
                    counts[count_i].fetch_sub(2);
                 } });
            }
            for (unsigned int i = 0; i < g_count_size; i++)
            {
                counts[i].fetch_sub(total - thread_num);// 每个格子-(total - thread_num)
            }
            for (unsigned int i = 0; i < thread_num; i++)
            {
                if (threads[i].joinable())
                {
                    threads[i].join();
                }
            }
        }

        for (unsigned int i = 0; i < g_count_size; i++)
        {
            printf("atomic_count[%d]:%d\n", i, counts[i].load());// 每个格子都是0
        }
    }
    printf("%s end\n\n", __FUNCTION__);
    return 0;
}

int main()
{
    TestNormalAddAndSub();
    TestAtomicFetchAddAndFetchSub();

    TestMutex(1);// works
    TestMutex(2);// works_mtx
    TestMutex(3);// works_lock_guard
    TestMutex(4);// works_atomic
    TestMutex(5);// works_atomic_relaxed
    return 0;
}
/*

count[0]:117
count[1]:119
count[2]:117
count[3]:116
count[4]:115
count[5]:65
count[6]:91
count[7]:87
count[8]:63
count[9]:91
TestNormalAddAndSub,timediff:sec:0,nsec:655893527
TestNormalAddAndSub end

atomic_count[0]:0
atomic_count[1]:0
atomic_count[2]:0
atomic_count[3]:0
atomic_count[4]:0
atomic_count[5]:0
atomic_count[6]:0
atomic_count[7]:0
atomic_count[8]:0
atomic_count[9]:0
TestAtomicFetchAddAndFetchSub,timediff:sec:0,nsec:626664921
TestAtomicFetchAddAndFetchSub end

count[0]:8
count[1]:-1
count[2]:18
count[3]:-2
count[4]:8
count[5]:-4
count[6]:0
count[7]:-14
count[8]:13
count[9]:-6
TestMutex,timediff:sec:0,nsec:324739287
TestMutex,type:1 end

count[0]:0
count[1]:0
count[2]:0
count[3]:0
count[4]:0
count[5]:0
count[6]:0
count[7]:0
count[8]:0
count[9]:0
TestMutex,timediff:sec:10,nsec:710315668
TestMutex,type:2 end

count[0]:0
count[1]:0
count[2]:0
count[3]:0
count[4]:0
count[5]:0
count[6]:0
count[7]:0
count[8]:0
count[9]:0
TestMutex,timediff:sec:10,nsec:714713305
TestMutex,type:3 end

atomic_count[0]:0
atomic_count[1]:0
atomic_count[2]:0
atomic_count[3]:0
atomic_count[4]:0
atomic_count[5]:0
atomic_count[6]:0
atomic_count[7]:0
atomic_count[8]:0
atomic_count[9]:0
TestMutex,timediff:sec:0,nsec:320834711
TestMutex,type:4 end

atomic_count relaxed[0]:0
atomic_count relaxed[1]:0
atomic_count relaxed[2]:0
atomic_count relaxed[3]:0
atomic_count relaxed[4]:0
atomic_count relaxed[5]:0
atomic_count relaxed[6]:0
atomic_count relaxed[7]:0
atomic_count relaxed[8]:0
atomic_count relaxed[9]:0
TestMutex,timediff:sec:0,nsec:319094726
TestMutex,type:5 end

   */
