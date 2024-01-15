#include "rate_controller.h"
#include <stdio.h>
#include <thread>

RateServerController::RateServerController(unsigned int rate_per_second)
{
    rate_per_second_ = rate_per_second;
    capcity_ = rate_per_second;
    water_ = 0;
    last_time_stamp_ = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}
RateServerController::~RateServerController()
{
}
bool RateServerController::CanAccess()
{
    std::chrono::microseconds now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    int diff_second = (now - last_time_stamp_).count();
    if (diff_second >= 1000000)
    {
        if (water_ > diff_second * rate_per_second_)
        {
            water_ -= diff_second * rate_per_second_;
        }
        else
        {
            water_ = 0;
        }

        last_time_stamp_ = now;
    }
    if (water_ < capcity_)
    {
        water_++;
        return true;
    }
    return false;
}
//////
RateClientController::RateClientController(const uint32_t rate) : counter_(0),
                                                                  delay_interval_ns_((rate > 0) ? (1000UL * 1000UL * 1000UL / rate) : 0)
{
    time_begin_ = std::chrono::high_resolution_clock::now();
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
        long long time_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - time_begin_).count();
        long long count = (++counter_);
        long long delay_ns = count * delay_interval_ns_ - time_diff;
        if (delay_ns > 0)
            BusyWait(delay_ns);
        // std::this_thread::sleep_for(std::chrono::nanoseconds(delay_ns));
    }
}
//////
RateController::RateController(unsigned int rate_per_second)
    : rate_server_controller_(rate_per_second),
      rate_client_controller_(rate_per_second)
{
}
bool RateController::CanAccess()
{
    return rate_server_controller_.CanAccess();
}
void RateController::Wait()
{
    rate_client_controller_.Wait();
}
