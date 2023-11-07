#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "posix_sem.h"

int Sem::Create(int value /* value = 0*/)
{
    int ret = sem_init(&mutex_, 0, value);
    if(ret == -1)
    {
        ret = errno;
        snprintf(errmsg_, sizeof(errmsg_), "%s", strerror(errno));
        return ret;
    }

    return ret;
}
int Sem::Release()
{
    int ret = sem_destroy(&mutex_); 
    if(ret == -1)
    {
        ret = errno;
        snprintf(errmsg_, sizeof(errmsg_), "%s", strerror(errno));
        return ret;
    }

    return 0;
}
int Sem::Wait(int timeout_ms)
{
    struct timespec now;
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &now);
    timeout.tv_sec = now.tv_sec;

    unsigned long long tv_nsec = now.tv_nsec + timeout_ms * 1000000UL;
    constexpr unsigned long long e9=1000000000;
    if(tv_nsec >= e9)
    {
        timeout.tv_sec += tv_nsec / e9;
        tv_nsec %= e9;
    }
    timeout.tv_nsec  = tv_nsec;

	int ret = sem_timedwait(&mutex_, &timeout); 
    if(ret == -1)
    {
        ret = errno;
        if(ret == ETIMEDOUT)
        {
            snprintf(errmsg_, sizeof(errmsg_), "sem_timedwait timeout:%dms",timeout_ms);
        }
        else 
        {
            snprintf(errmsg_, sizeof(errmsg_), 
            "sem_timedwait timeout:%dms,errno:%d,msg:%s",timeout_ms,errno,strerror(errno));
        }
        return ret;
    }
    return 0;
}
int Sem::Signal()
{
     int ret = sem_post(&mutex_); 
    if(ret == -1)
    {
        ret = errno;
        snprintf(errmsg_, sizeof(errmsg_), "%s", strerror(errno));
        return ret;
    }
    return 0;
}

const char *Sem::Errmsg()
{
    return errmsg_;
}
