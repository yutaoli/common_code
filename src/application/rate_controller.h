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
    std::chrono::seconds last_time_stamp_;
};

class RateClientController
{
public:
};

class RateController
{
public:
    bool CanAccess();
    void Wait();
};
#endif