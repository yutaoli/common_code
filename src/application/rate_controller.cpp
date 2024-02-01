#include "rate_controller.h"
#include <stdio.h>
#include <thread>

RateServerController::RateServerController(unsigned int rate_per_second)
{
    rate_per_second_ = (long long)rate_per_second * g_transfer_times;
    capcity_ = 0;//(long long)rate_per_second * g_transfer_times;
    water_ = 0;
    last_time_stamp_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}
RateServerController::~RateServerController()
{
}
bool RateServerController::CanAccess(bool busy_wait)
{
    // 不限速
    if(busy_wait == false) return true;

    // 限速
    std::chrono::microseconds now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    long long diff_mircosecond = (now - last_time_stamp_).count();

    long long water = water_ + 1 * g_transfer_times;
    long long eclapse_water = diff_mircosecond * rate_per_second_ / 1000000;
    if (water > eclapse_water)
    {
        water -= eclapse_water;
    }
    else
    {
        //printf("water <= eclapse_water,water_:%lld,eclapse_water:%lld,water:%lld\n", water_, eclapse_water,water);
        water = 0;
    }

    if (water <= capcity_)
    {
        water_ = water;
        last_time_stamp_ = now;
        /*printf("add water, water_:%lld,timestamp:%lld.%lld\n",
               water_, now.count() / 1000000, now.count() % 1000000);*/
        return true;
    }
    return false;
}
//////
RateClientController::RateClientController(const uint32_t rate, unsigned int max_delay_us)
    : counter_(0)
    , delay_interval_ns_((rate > 0) ? (1000UL * 1000UL * 1000UL / rate) : 0)
    , max_delay_us_(max_delay_us)
{
    time_begin_ = std::chrono::high_resolution_clock::now();
    last_update_time_clock_ = time_begin_;

    if (delay_interval_ns_ > 0)
        busy_wait_ = true;
    else
        busy_wait_ = false;
}

#if defined(WIN32)

// 当前时间
int64_t RateClientController::GetCurrTime()
{
    static LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER ticks;

    if (!ticksPerSec.QuadPart)
    {
        QueryPerformanceFrequency(&ticksPerSec);
        if (!ticksPerSec.QuadPart)
        {
            return -1;
        }
    }

    QueryPerformanceCounter(&ticks);

    double seconds = (double)ticks.QuadPart / (double)ticksPerSec.QuadPart;

    return (int64_t)seconds * (int64_t)1000000000ULL + ((ULONGLONG)(seconds * 1000000000ULL) % 1000000000ULL);
}

#else

// 当前时间
inline int64_t RateClientController::GetCurrTime()
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
    {
        return -1;
    }

    return (int64_t)ts.tv_sec * (int64_t)1000000000ULL + ts.tv_nsec;
}
#endif
// yield 单位(us)
inline void RateClientController::BusyWait(uint64_t nsec)
{
    uint64_t lBegin = GetCurrTime();

    do
    {
        std::this_thread::yield();
    } while ((GetCurrTime() - lBegin) < (uint64_t)nsec);
}

void RateClientController::Wait()
{
    if (delay_interval_ns_)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now(); 
        long long time_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(now - time_begin_).count();
        
        long long count = (++counter_);
        long long delay_ns = count * delay_interval_ns_ - time_diff;
        if (delay_ns > 0)
        {
            busy_wait_ = true;
            BusyWait(delay_ns);
        }
        else
        {
            if (std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_update_time_clock_).count() > max_delay_us_ * 1000)
            {
                busy_wait_ = false;

                printf("exceed,delay_ns:%lld,count:%lld,delay_interval_ns_:%u,time_diff:%lld,busy_wait_:%d,diff:%llu\n",
                   delay_ns, count, delay_interval_ns_, time_diff, busy_wait_,std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_update_time_clock_).count());
            }
        }
        /*printf("delay_ns:%lld,count:%lld,delay_interval_ns_:%u,time_diff:%lld,busy_wait_:%d,diff:%llu\n",
                   delay_ns, count, delay_interval_ns_, time_diff, busy_wait_,std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_update_time_clock_).count());*/
        last_update_time_clock_ = now;

        /*printf("delay_ns:%lld,count:%lld,delay_interval_ns_:%u,time_diff:%lld,busy_wait_:%d\n",
               delay_ns, count, delay_interval_ns_, time_diff, busy_wait_);*/
        // std::this_thread::sleep_for(std::chrono::nanoseconds(delay_ns));
    }
}
bool RateClientController::BusyWait()
{
    return busy_wait_;
}
//////
RateController::RateController(unsigned int rate_per_second, unsigned int max_delay_us)
    : rate_server_controller_(rate_per_second),
      rate_client_controller_(rate_per_second,max_delay_us)
{
}
bool RateController::CanAccess()
{
    // 为了提高性能，不要同时限速；故：client端限速，则server端不限速；当client端不限速，则server端限速；
    bool busy_wait = (rate_client_controller_.BusyWait() == false);
    bool can_access = rate_server_controller_.CanAccess(busy_wait);
    if (can_access == false)
        rate_client_controller_.Wait();

    return can_access;
}
void RateController::Wait()
{
    rate_client_controller_.Wait();
}
