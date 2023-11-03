#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
 
std::condition_variable cv;
std::mutex cv_m; // mutex的作用有三个:
                 // 1) 同步访问i
                 // 2) 同步访问std::cerr输出
                 // 3) 同步条件变量cv，收到notify_all的时候，只有获得锁才会被唤醒
int i = 0;
 
void waits()
{
    std::unique_lock<std::mutex> lk(cv_m);
    std::cerr << "Waiting... \n";
    cv.wait(lk, []{return i == 1;});
    std::cerr << "...finished waiting. i:"<<i<<std::endl;
}
 
void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(cv_m);
        std::cerr << "Notifying...\n";
    }
    cv.notify_all();
 
    std::this_thread::sleep_for(std::chrono::seconds(1));
 
    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;
        std::cerr << "Notifying again...\n";
    }
    cv.notify_all();
}
 
int main()
{
    std::thread t1(waits), t2(waits), t3(waits), t4(signals);
    t1.join(); 
    t2.join(); 
    t3.join();
    t4.join();
}
/*
Waiting... 
Waiting... 
Waiting... 
Notifying...
Notifying again...
...finished waiting. i:1
...finished waiting. i:1
...finished waiting. i:1

*/