#ifndef RATE_CONTROLLER_H
#define RATE_CONTROLLER_H

#include <chrono>

// 漏桶算法
class RateServerController
{
public:
    RateServerController(unsigned int rate_per_second);
    virtual ~RateServerController();

public:
    bool CanAccess(bool busy_wait);

private:
    long long rate_per_second_;
    long long capcity_;
    long long water_;
    // std::chrono::time_point<std::chrono::high_resolution_clock> last_time_stamp_ns_;
    std::chrono::microseconds last_time_stamp_;

    static constexpr long long g_transfer_times = 1000000;
};

//////
class RateClientController
{
public:
    RateClientController(const uint32_t rate, unsigned int max_delay_us = 200 * 1000);

public:
    void Wait();
    bool BusyWait();

private:
    // 当前时间
    int64_t GetCurrTime();
    // yield 单位(us)
    void BusyWait(uint64_t nsec);

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> time_begin_;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time_clock_;
    uint32_t counter_;
    uint32_t delay_interval_ns_;// 0 为不等（不限速）；>0为限速
    bool busy_wait_;// true：限速；false：不限速；

    unsigned int max_delay_us_;
};

//////
class RateController
{
public:
    RateController(unsigned int rate_per_second, unsigned int max_delay_us = 200 * 1000);

public:
    bool CanAccess();
    void Wait();
private:
    RateServerController rate_server_controller_;
    RateClientController rate_client_controller_;

};
#endif