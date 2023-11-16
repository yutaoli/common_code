#ifndef CC_TIME_H
#define CC_TIME_H

#include <string>
namespace cc_time
{
    class CcTime
    {
    public:
        // time point
        static unsigned long long NowNs();
        // time format
        static std::string NowString();
    };

    // timediff
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
}
#endif