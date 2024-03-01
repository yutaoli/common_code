#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <sys/syscall.h>

unsigned int i = 0;
void works()
{
    i++;
}

unsigned long long GetThreadId(const std::thread::id &id)
{
    std::stringstream ss;
    ss << id;
    return atoll(ss.str().c_str());
}

void TestYield()
{
    printf("%s,pid:%d,tid:%d,thread_id:%llu\n", __FUNCTION__, getpid(), syscall(__NR_gettid), GetThreadId(std::this_thread::get_id()));
    while (1)
    {
        std::this_thread::yield();
    }
}

void TestSleep()
{
    printf("%s,pid:%d,tid:%d,thread_id:%llu\n", __FUNCTION__, getpid(), syscall(__NR_gettid), GetThreadId(std::this_thread::get_id()));
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(100));
    }
}

void TestThread()
{
    std::thread t;

    t = std::thread(works);
    if (t.joinable())
    {
        sleep(1);
        printf("get_id:%llu,join_able:%d\n", GetThreadId(t.get_id()), t.joinable());
        t.join();
        printf("after join,get_id:%llu,join_able:%d\n", GetThreadId(t.get_id()), t.joinable());
    }
    printf("%d\n", i);
    printf("get_id:%llu\n\n", GetThreadId(t.get_id()));

    t = std::thread(works);
    if (t.joinable())
    {
        printf("get_id:%llu\n", GetThreadId(t.get_id()));
        t.join();
        printf("after join,get_id:%llu\n", GetThreadId(t.get_id()));
    }
    printf("%d\n", i);
    printf("get_id:%llu\n", GetThreadId(t.get_id()));
}

int TestYieldAndSleepCPU()
{
    std::thread t1 = std::thread(TestYield);
    std::thread t2 = std::thread(TestSleep);
    if (t1.joinable())
    {
        t1.join();
    }
    if (t2.joinable())
    {
        t2.join();
    }

    return 0;
}
int main()
{
    //TestThread();
    TestYieldAndSleepCPU();
    return 0;
}
/*
get_id:139831073912576,join_able:1
after join,get_id:0,join_able:0
1
get_id:0

get_id:139831073912576
after join,get_id:0
2
get_id:0

TestYieldAndSleepCPU:
[liyutao@fsptMaster:/home/liyutao/data/software/common_code/src/demo/c++11]$ ./test_thread
TestSleep,pid:3106236,tid:3106238,thread_id:281466864398784
TestYield,pid:3106236,tid:3106237,thread_id:281466874950080

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
3106237 liyutao   20   0   91968   2944   1856 R  99.9   0.0   0:42.27 test_thread
3106236 liyutao   20   0   91968   2944   1856 S   0.0   0.0   0:00.00 test_thread
3106238 liyutao   20   0   91968   2944   1856 S   0.0   0.0   0:00.00 test_thread
*/
