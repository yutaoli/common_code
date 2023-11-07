// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <errno.h>
#include <string>

sem_t mutex; 
//int timeout_ms = 2 * 1000;

typedef struct timediff
{
    timediff(std::string prompt_)
    {   
        prompt = prompt_;
        begin.tv_sec = 0;
        begin.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &begin);
    }   

    ~ timediff()
    {   
        end.tv_sec = 0;
        end.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &end);

        if(end.tv_nsec > begin.tv_nsec)
        {   
            printf("%s,timediff:sec:%lu,nsec:%lu\n",prompt.c_str(),end.tv_sec-begin.tv_sec, end.tv_nsec- begin.tv_nsec);
        }   
        else 
        {   
            printf("%s,timediff:sec:%lu,nsec:%lu\n",prompt.c_str(),end.tv_sec-begin.tv_sec-1, 1000000000UL + end.tv_nsec- begin.tv_nsec);
        }   
        printf("\n");
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
}timediff_t;

struct thread_wait_timeout_arg
{
int timeout_ms;
int retcode;
};

void* thread_wait_timeout(void* arg_) 
{ 
	printf("\n%s Entered & Wait..\n",__FUNCTION__); 
	//wait 

    thread_wait_timeout_arg *arg = (thread_wait_timeout_arg*)arg_;
    struct timespec now;
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &now);
    timeout.tv_sec = now.tv_sec;

    unsigned long long tv_nsec = now.tv_nsec + arg->timeout_ms * 1000000UL;
    constexpr unsigned long long e9=1000000000;
    printf("now.tv_nsec:%llu,arg->timeout_ms:%d,tv_nsec:%llu\n",
            now.tv_nsec,arg->timeout_ms,tv_nsec);
    if(tv_nsec >= e9)
    {
        timeout.tv_sec += tv_nsec / e9;
        tv_nsec %= e9;
    }
    timeout.tv_nsec  = tv_nsec;

    int ret = 0;
    {
        timediff_t timediff_sem_timedwait("sem_timedwait");
        ret = sem_timedwait(&mutex, &timeout); 
    }
    if(ret == -1)
    {
        if(errno == ETIMEDOUT)
        {
            printf("%s,sem_timedwait timeout:%dms\n",__FUNCTION__,arg->timeout_ms);
            arg->retcode =  -101;
        }
        else 
        {
            printf("%s,sem_timedwait timeout ret:%d,errno:%d\n",__FUNCTION__,ret,errno);
            arg->retcode = -1;
        }
    }
    
	printf("\n%s Wait return ..\n",__FUNCTION__); 
}

void* thread_wait(void* arg) 
{ 
	printf("\n%s Entered & Wait..\n",__FUNCTION__); 
	//wait 
	sem_wait(&mutex); 
	printf("\n%s Wait return ..\n",__FUNCTION__); 
} 

void* thread_signal(void* arg) 
{ 
	printf("\n%s Entered..\n",__FUNCTION__); 

	//critical section 
	sleep(4); 
	
	//signal 
	printf("\n%s,Just Signal...\n",__FUNCTION__); 
	sem_post(&mutex); 
} 

int TestWaitSignal()
{
    unsigned int value = 0;
    sem_init(&mutex, 0, value); // value==0, equal lock
    pthread_t t1,t2; 
	pthread_create(&t1,NULL,thread_wait,NULL); 
	pthread_create(&t2,NULL,thread_signal,NULL); 
	pthread_join(t1,NULL); 
	pthread_join(t2,NULL); 
	sem_destroy(&mutex); 

    return 0;
}

int TestWaitSignalNoWait()
{
    unsigned int value = 1;
    sem_init(&mutex, 0, value); // value==0, equal lock
    pthread_t t1,t2,t3; 
	pthread_create(&t1,NULL,thread_wait,NULL); 
	pthread_create(&t2,NULL,thread_wait,NULL); 
	pthread_create(&t3,NULL,thread_signal,NULL); 
	pthread_join(t1,NULL); 
	pthread_join(t2,NULL); 
	pthread_join(t3,NULL); 
	sem_destroy(&mutex); 

    return 0;
}

int TestWaitSignalTimeout()
{
    unsigned int value = 0;
    sem_init(&mutex, 0, value); // value==0, equal lock
    pthread_t t1,t2; 

    thread_wait_timeout_arg arg;
    arg.retcode = 0;
    arg.timeout_ms = 10 * 1000;
	pthread_create(&t1,NULL,thread_wait_timeout,&arg); 
	pthread_create(&t2,NULL,thread_signal,NULL); 
	pthread_join(t1,NULL); 
	pthread_join(t2,NULL); 
	sem_destroy(&mutex); 

    return 0;
}

int main() 
{ 
    //TestWaitSignal();
    //TestWaitSignalNoWait();
    TestWaitSignalTimeout();
	return 0; 
} 
