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
    bool CanAccess();

private:
    unsigned int rate_per_second_;
    unsigned int capcity_;
    unsigned int water_;
    // std::chrono::time_point<std::chrono::high_resolution_clock> last_time_stamp_ns_;
    std::chrono::microseconds last_time_stamp_;
};

//////
class RateClientController
{
public:
    RateClientController(const uint32_t rate);

public:
    void Wait();

private:
    // 当前时间
    int64_t GetCurrTime();
    // yield 单位(us)
    void BusyWait(uint64_t nsec);

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> time_begin_;
    uint32_t counter_;
    uint32_t delay_interval_ns_;
};

//////
class RateController
{
public:
    RateController(unsigned int rate_per_second);

public:
    bool CanAccess();
    void Wait();
private:
    RateServerController rate_server_controller_;
    RateClientController rate_client_controller_;

};
#endif