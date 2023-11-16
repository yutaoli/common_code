#include <CppUTest/CommandLineTestRunner.h>

#include "cc_time.h"

TEST_GROUP(CcTime)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};

TEST(CcTime, TeskNowns)
{
    unsigned long long now_ns = cc_time::CcTime::NowNs();
    CHECK(now_ns / 1000000000LL == time(NULL));
}

TEST(CcTime, TestNowString)
{
   // printf("now :%s\n", cc_time::CcTime::NowString().c_str());
}