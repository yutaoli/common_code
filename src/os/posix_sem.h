#ifndef POSIX_SEM_H
#define POSIX_SEM_H
#include <semaphore.h> 

class Sem
{
    public:
    int Create(int value = 0);
    int Release();
    int Wait(int timeout_ms);
    int Signal();
    const char *Errmsg();

    private:
    sem_t mutex_; 
    char errmsg_[1024];
};
#endif
/*
Sem sem;
int ret = Sem.Create();
if(ret != 0)
{
    printf("Sem.Create ret:%d,errmsg:%s",ret,Sem.Errmsg());
    return ret;
}

Sem.Wait(100);
Sem.Signal();

ret = Sem.Release();
if(ret != 0)
{
    printf("Sem.Release ret:%d,errmsg:%s",ret,Sem.Errmsg());
    return ret;
}

ret = Sem.Create(); 
。。。
ret = Sem.Release();
。。。
*/
