#include "rate_controller.h"
#include <stdio.h>

RateServerController::RateServerController(unsigned int rate_per_second)
{
    rate_per_second_ = rate_per_second;
    capcity_ = rate_per_second;
    water_ = 0;
    last_time_stamp_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}
RateServerController::~RateServerController()
{
}
bool RateServerController::CanAccess()
{
    std::chrono::seconds now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    int diff_second = (now - last_time_stamp_).count();
    if ( diff_second >= 1)
    {
        if(water_ > diff_second * rate_per_second_)
        {
            water_ -= diff_second * rate_per_second_;
        }
        else 
        {
            water_ = 0;
        }

        last_time_stamp_ = now;
    }
    if(water_ < capcity_)
    {
        water_++;
        return true;
    }
    return false;
}