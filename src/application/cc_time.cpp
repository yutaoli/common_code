#include "cc_time.h"

namespace cc_time
{
     unsigned long long CcTime::NowNs()
    {
#ifdef _MSC_VER
        return std::chrono::high_resolution_clock::now().time_since_epoch().count();
#else
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return ts.tv_sec * 1000000000LL + ts.tv_nsec;
#endif
    }

     std::string CcTime::NowString()
    {
        struct timespec ts = {0, 0};
        clock_gettime(CLOCK_REALTIME, &ts);
        struct tm stm = {0};
        localtime_r(&ts.tv_sec, &stm);
        char buf[256] = {0};
        snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d.%03d.%03d.%03d", stm.tm_year + 1900, stm.tm_mon + 1, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec, ts.tv_nsec / 1000000, (ts.tv_nsec / 1000) % 1000, ts.tv_nsec % 1000);
        return buf;
    }
}