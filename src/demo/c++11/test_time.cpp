#include <chrono>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <thread>
typedef struct timediff
{
    timediff(std::string prompt_)
    {
        prompt = prompt_;
        begin.tv_sec = 0;
        begin.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &begin);
    }

    ~timediff()
    {
        end.tv_sec = 0;
        end.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &end);

        if (end.tv_nsec > begin.tv_nsec)
        {
            printf("%s,timediff:sec:%lu,nsec:%lu\n", prompt.c_str(), end.tv_sec - begin.tv_sec, end.tv_nsec - begin.tv_nsec);
        }
        else
        {
            printf("%s,timediff:sec:%lu,nsec:%lu\n", prompt.c_str(), end.tv_sec - begin.tv_sec - 1, 1000000000UL + end.tv_nsec - begin.tv_nsec);
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
} timediff_t;

constexpr unsigned int kRunTime = 100000;
int TestClockGetTime()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        unsigned long long nanosec2 = ts.tv_sec * 1000000000L + ts.tv_nsec;
        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}
int TestChrono()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();
        auto nanosec = t0.time_since_epoch();

        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}

int TestChronoNoEpoch()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();
        // auto nanosec = t0.time_since_epoch();

        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}

int TestDivide()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        int a = rand() % 100;
        // auto nanosec = t0.time_since_epoch();

        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}

int TestChronoNoEpochDiff()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();

        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
        // std::this_thread::sleep_for(std::chrono::microseconds(100));
        std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();

        long long diff = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        //printf("diff:%lldns\n", diff); // diff:52619ns
        // auto nanosec = t0.time_since_epoch();

        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}

int TestClockGetTimeExpandVarialbe()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec * 1000000000L + ts.tv_nsec;
        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}
int TestChronoExpandVarialbe()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        std::chrono::high_resolution_clock::now().time_since_epoch().count();

        // printf("now nano:%llu\n", nanosec.count());
    }
    // std::cout << nanosec.count() << " nanoseconds since epoch\n";
    // std::cout << nanosec.count() / (1000000000.0 * 60.0 * 60.0) << " hours since epoch\n";

    return 0;
}

int TestGetTimeOfDay()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        struct timeval tv = {0, 0};
        struct timezone tz = {0, 0};
        gettimeofday(&tv, &tz);
        unsigned long long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}
int TestGetTimeOfDayExpandVarialbe()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        struct timeval tv = {0, 0};
        struct timezone tz = {0, 0};
        gettimeofday(&tv, &tz);
        tv.tv_sec * 1000 + tv.tv_usec / 1000;

        // printf("now nano:%llu\n", nanosec.count());
    }
    return 0;
}
int TestLocalTime()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        struct timespec ts = {0, 0};
        clock_gettime(CLOCK_REALTIME, &ts);
        struct tm stm = {0};
        localtime_r(&ts.tv_sec, &stm);
        // printf("%d-%02d-%02d %02d:%02d:%02d.%03d.%03d.%03d\n", stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec, ts.tv_nsec/1000000,(ts.tv_nsec/1000)%1000,ts.tv_nsec%1000);
    }
    return 0;
}

int TestClockExpandVarialbe()
{
    timediff_t timediff(__FUNCTION__);
    for (unsigned int i = 0; i < kRunTime; i++)
    {
        time(NULL);
    }
    return 0;
}

int main()
{
    TestChrono();
    TestClockGetTime();
    TestChronoExpandVarialbe();
    TestClockGetTimeExpandVarialbe();
    TestGetTimeOfDay();
    TestGetTimeOfDayExpandVarialbe();
    TestLocalTime();
    TestClockExpandVarialbe();

    TestChronoNoEpoch();
    TestDivide();
    TestChronoNoEpochDiff();
    return 0;
}
/*
TestChrono,timediff:sec:0,nsec:3170315
TestClockGetTime,timediff:sec:0,nsec:2945570
TestChronoExpandVarialbe,timediff:sec:0,nsec:3384861
TestClockGetTimeExpandVarialbe,timediff:sec:0,nsec:2829752
TestGetTimeOfDay,timediff:sec:0,nsec:3174705
TestGetTimeOfDayExpandVarialbe,timediff:sec:0,nsec:2923100
TestLocalTime,timediff:sec:0,nsec:31845969
TestClockExpandVarialbe,timediff:sec:0,nsec:3336211
TestChronoNoEpoch,timediff:sec:0,nsec:3153876
TestDivide,timediff:sec:0,nsec:1962550
TestChronoNoEpochDiff,timediff:sec:5,nsec:638835196
说明sleep_for()最小精度是50us
*/