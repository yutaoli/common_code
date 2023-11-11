#include <signal.h>
#include <thread>
#include <vector>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <chrono>

void SignalCallback(int signo)
{
    if (signo == SIGINT)
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    return;
}

void ThreadWorkSignalIgnore(bool ignore)
{
    if (ignore)
    {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        int ret = pthread_sigmask(SIG_BLOCK, &set, NULL);
        if (ret != 0)
        {
            printf("pthread_sigmask ret:%d,errno:%d,errmsg:%s",
                   ret, errno, strerror(errno));
            return;
        }
    }
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // TODO
}

int ThreadSignalIgnore(bool ignore)
{
    signal(SIGINT, SignalCallback);

    unsigned int threads_size = 2;
    std::vector<std::thread> threads;
    threads.resize(threads_size);

    for (unsigned int i = 0; i < threads_size; i++)
    {
        threads[i] = std::thread(ThreadWorkSignalIgnore, ignore);
    }

    for (unsigned int i = 0; i < threads_size; i++)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }
    return 0;
}

int TestThreadSignalIgnore()
{
    return ThreadSignalIgnore(true);
}

int TestThreadSignalNotIgnore()
{
    return ThreadSignalIgnore(false);
}

int main()
{
    TestThreadSignalIgnore();
    //TestThreadSignalNotIgnore();// 多发几次ctrl-c会发现每个线程都有可能跳转到信号处理函数
    // TODO
    return 0;
}