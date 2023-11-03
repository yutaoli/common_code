#include <thread>
#include <stdio.h>
#include <unistd.h>

unsigned int i = 0;
void works()
{
    i++;
}

int main()
{
    std::thread t;
    
    t = std::thread(works);
    if(t.joinable())
    {
        sleep(1);
        printf("get_id:%llu,join_able:%d\n",t.get_id(),t.joinable());
        t.join();
        printf("after join,get_id:%llu,join_able:%d\n",t.get_id(),t.joinable());
    }
    printf("%d\n",i);
    printf("get_id:%llu\n\n",t.get_id());
     
    t = std::thread(works);
    if(t.joinable())
    {
        printf("get_id:%llu\n",t.get_id());
        t.join();
        printf("after join,get_id:%llu\n",t.get_id());
    }
    printf("%d\n",i);
    printf("get_id:%llu\n",t.get_id());

    return 0;
}
/*
get_id:139831073912576,join_able:1
after join,get_id:0,join_able:0
1
get_id:0

get_id:139831073912576
after join,get_id:0
2
get_id:0

*/
