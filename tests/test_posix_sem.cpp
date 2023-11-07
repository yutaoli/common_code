#include <CppUTest/CommandLineTestRunner.h>
#include <thread>
#include <unistd.h>

#include "posix_sem.h"

TEST_GROUP(PosixSem)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};

TEST(PosixSem, TestWaitSignalTimeout)
{
    Sem sem;
    int ret = sem.Create();
    CHECK(ret == 0);

    int timeout_ms = 100;
    std::thread t([&]{
            sleep(1);
            int ret = sem.Signal();
            CHECK(ret == 0);
            });

    ret = sem.Wait(timeout_ms);
    CHECK(ret == ETIMEDOUT);
    //printf("msg:%s\n",sem.Errmsg());

    if(t.joinable())
    {
        t.join();
    }
}

TEST(PosixSem, TestWaitSignalNoTimeout)
{
    Sem sem;
    int ret = sem.Create();
    CHECK(ret == 0);

    int timeout_ms = 10000;
    std::thread t([&]{
            sleep(1);
            int ret = sem.Signal();
            CHECK(ret == 0);
            });

    ret = sem.Wait(timeout_ms);
    CHECK(ret == 0);

    if(t.joinable())
    {
        t.join();
    }
}

// 多次Create-Release
TEST(PosixSem, TestWaitSignalNoTimeoutCreateAndRelease)
{
    Sem sem;
     std::thread t;
    for(unsigned int i = 0; i < 5; i++)
    {
        int ret = sem.Create();
        CHECK(ret == 0);

        int timeout_ms = 10000;
        t = std::thread([&]
                        {
            sleep(1);
            int ret = sem.Signal();
            CHECK(ret == 0); });

        ret = sem.Wait(timeout_ms);
        CHECK(ret == 0);

        if (t.joinable())
        {
            t.join();
        }
        ret = sem.Release();
        CHECK(ret == 0);
    }
}
