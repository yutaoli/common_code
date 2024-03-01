#include <CppUTest/CommandLineTestRunner.h>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <atomic>
#include "rate_controller.h"

TEST_GROUP(TestRateController)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};
///RateServerController///
TEST(TestRateController, TestServerControllerOk)
{
   unsigned int rate_per_second = 10;
   RateServerController rate_server_controller(rate_per_second);
   bool busy_wait = true;
   for (unsigned int i = 0; i < rate_per_second; i++)
   {
      bool can_access = rate_server_controller.CanAccess(busy_wait);
      CHECK(can_access == true);
   }

   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   for (unsigned int i = 0; i < rate_per_second; i++)
   {
      bool can_access = rate_server_controller.CanAccess(busy_wait);
      CHECK(can_access == false);
   }

   std::this_thread::sleep_for(std::chrono::seconds(1));
   for (unsigned int i = 0; i < rate_per_second; i++)
   {
      bool can_access = rate_server_controller.CanAccess(busy_wait);
      CHECK(can_access == true);
   }
}
///RateClientController///
TEST(TestRateController, TestClientControllerOkRateLessThanProcessDelay)
{
   unsigned int rate_per_second = 20000;// 50us
   RateClientController rate_client_controller(rate_per_second);

   std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();
   unsigned int req_count = 100000;
   srand(time(NULL));
   for(unsigned int i = 0; i < req_count; i++)
   {
      std::this_thread::sleep_for(std::chrono::microseconds(rand()%500));// 模拟process延迟
      rate_client_controller.Wait();
   }

   std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
   long long diff_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
   double req_count_ll = req_count * 1000000000UL;
   double qps = req_count_ll / diff_ns;
   
   unsigned int qps_u = ceil(qps);
   /*printf("qps:%.2f,diff_ns:%lld,req_count_ll:%.2f,rate_per_second:%u,qps_u:%u\n",
      qps,diff_ns,req_count_ll,rate_per_second,qps_u);*/
   CHECK(qps_u < rate_per_second);
}

TEST(TestRateController, TestClientControllerOkRateGreaterThanProcessDelay)
{
   unsigned int rate_per_second = 2000;// 500us
   RateClientController rate_client_controller(rate_per_second);

   std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();
   unsigned int req_count = 10000;
   srand(time(NULL));
   for(unsigned int i = 0; i < req_count; i++)
   {
      std::this_thread::sleep_for(std::chrono::microseconds(rand()%100));// 模拟process延迟
      rate_client_controller.Wait();
   }

   std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
   long long diff_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
   double req_count_ll = req_count * 1000000000UL;
   double qps = req_count_ll / diff_ns;
   unsigned int qps_u = ceil(qps);
   /*printf("qps:%.2f,diff_ns:%lld,req_count_ll:%.2f,rate_per_second:%u,qps_u:%u\n",
      qps,diff_ns,req_count_ll,rate_per_second,qps_u);*/
   CHECK(qps_u == rate_per_second);
}
TEST(TestRateController, TestClientControllerConnectionFailLongTime)
{
   unsigned int rate_per_second = 2000;// 500us
   RateClientController rate_client_controller(rate_per_second);

   std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();
   unsigned int req_count = 60000;
   srand(time(NULL));
   unsigned int i = 0;

   // TimerThread
   std::thread t = std::thread([&]{ 
      unsigned int last_update_i = i;
      unsigned int now_i = 0;
      unsigned int qps = 0;
      while( i < req_count)
      {
         std::this_thread::sleep_for(std::chrono::seconds(1));
         now_i = i;
         qps = (now_i - last_update_i);// qps
         //printf("last_update_i:%u,now_i:%u,qps:%u\n",last_update_i,now_i,qps);
         
         last_update_i = now_i;
      } 
   });

   // 主线程
   for(; i < req_count; i++)
   {
      if (i == req_count/10)// 模拟掉坑10s
      {
         //printf("chose:%d\n",1);
         std::this_thread::sleep_for(std::chrono::seconds(10));
      }
      std::this_thread::sleep_for(std::chrono::microseconds(rand()%200));// 模拟process延迟
      rate_client_controller.Wait();
   }

   /*std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
   long long diff_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
   double req_count_ll = req_count * 1000000000UL;
   double qps = req_count_ll / diff_ns;
   unsigned int qps_u = ceil(qps);
   printf("qps:%.2f,diff_ns:%lld,req_count_ll:%.2f,rate_per_second:%u,qps_u:%u\n",
      qps,diff_ns,req_count_ll,rate_per_second,qps_u);
   CHECK((unsigned int)(qps) == req_count/rate_per_second);*/

   if (t.joinable())
      t.join();

   /*
   last_update_i:0,now_i:2000,qps:2000
last_update_i:2000,now_i:4000,qps:2000
last_update_i:4000,now_i:6000,qps:2000
last_update_i:6000,now_i:8000,qps:2000
chose:1
last_update_i:8000,now_i:9498,qps:1498<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:9498,qps:0<--
last_update_i:9498,now_i:11351,qps:1853<--
last_update_i:11351,now_i:18679,qps:7328<--陡增
last_update_i:18679,now_i:26086,qps:7407<--陡增
last_update_i:26086,now_i:33447,qps:7361<--陡增
last_update_i:33447,now_i:38002,qps:4555<--陡增
last_update_i:38002,now_i:40002,qps:2000
last_update_i:40002,now_i:42002,qps:2000
last_update_i:42002,now_i:44002,qps:2000
last_update_i:44002,now_i:46003,qps:2001
last_update_i:46003,now_i:48003,qps:2000
last_update_i:48003,now_i:50003,qps:2000
last_update_i:50003,now_i:52003,qps:2000
last_update_i:52003,now_i:54003,qps:2000

   */
}
///RateController///
TEST(TestRateController, TestControllerConnectionFailLongTime)
{
   unsigned int rate_per_second = 2000;// 500us
   RateController rate_controller(rate_per_second);

   std::chrono::time_point<std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();
   int req_count = 4000;
   srand(time(NULL));
   int i = 0;
   int finish_i = 0;

   // TimerThread
   std::thread t = std::thread([&]{ 
       int last_update_i = -1;
       int now_i = 0;
       unsigned int qps = 0;
      while( finish_i < req_count)
      {
         std::this_thread::sleep_for(std::chrono::seconds(1));
         now_i = finish_i;// (last_update_i, finish_i]
         qps = (now_i - last_update_i);// qps
        printf("last_update_i:%d,now_i:%d,qps:%u\n",last_update_i,now_i,qps);
         
         last_update_i = now_i;
      } });

   // 主线程
   for (; i < req_count; i++)
   {
      if (rate_controller.CanAccess() == false)
      {
         i = (i == 0 ? 0 : i - 1);
         continue;
      }

      //if (i == rate_per_second * 5 || i == rate_per_second * 20) // 模拟第5s掉坑10s，第15s掉坑10s
      if (i == rate_per_second * 5) // 模拟第5s掉坑10s
      {
         printf("chose:%d\n", 1);
         std::this_thread::sleep_for(std::chrono::seconds(10));
      }
      std::this_thread::sleep_for(std::chrono::microseconds(rand() % 2)); // 模拟process延迟
      finish_i = i;
      //printf("req_count:%d,finish_i:%d\n",req_count,i);
      rate_controller.Wait();
   }
   finish_i = i;

   /*std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
   long long diff_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
   double req_count_ll = req_count * 1000000000UL;
   double qps = req_count_ll / diff_ns;
   unsigned int qps_u = ceil(qps);
   printf("qps:%.2f,diff_ns:%lld,req_count_ll:%.2f,rate_per_second:%u,qps_u:%u\n",
      qps,diff_ns,req_count_ll,rate_per_second,qps_u);
   CHECK((unsigned int)(qps) == req_count/rate_per_second);*/

   if (t.joinable())
      t.join();

   /*
   last_update_i:-1,now_i:10001,qps:10002
last_update_i:10001,now_i:20001,qps:10000
last_update_i:20001,now_i:30001,qps:10000
last_update_i:30001,now_i:40003,qps:10002
chose:1<---开始掉坑
last_update_i:40003,now_i:49999,qps:9996<---掉坑
last_update_i:49999,now_i:49999,qps:0<---掉坑
last_update_i:49999,now_i:49999,qps:0<---掉坑
last_update_i:49999,now_i:49999,qps:0
last_update_i:49999,now_i:49999,qps:0
last_update_i:49999,now_i:49999,qps:0
last_update_i:49999,now_i:49999,qps:0
last_update_i:49999,now_i:49999,qps:0
last_update_i:49999,now_i:49999,qps:0
last_update_i:49999,now_i:49999,qps:0
delay_ns:-9999993305,count:50001,delay_interval_ns_:100000,time_diff:15000093305,busy_wait_:0,diff:10000139729
add water, water_:0,timestamp:1706753695.50400
add water, water_:0,timestamp:1706753695.50500
add water, water_:0,timestamp:1706753695.50600
add water, water_:0,timestamp:1706753695.50700
add water, water_:0,timestamp:1706753695.50819
add water, water_:0,timestamp:1706753695.50919
add water, water_:0,timestamp:1706753695.51019
add water, water_:0,timestamp:1706753695.51119
add water, water_:0,timestamp:1706753695.51219
add water, water_:0,timestamp:1706753695.51319
last_update_i:49999,now_i:50010,qps:11
add water, water_:0,timestamp:1706753695.51419
add water, water_:0,timestamp:1706753695.51519
add water, water_:0,timestamp:1706753695.51619
add water, water_:0,timestamp:1706753695.51719
add water, water_:0,timestamp:1706753695.51819
add water, water_:0,timestamp:1706753695.51919
。。。。。。
（中间类似的省略）
。。。。。。
add water, water_:0,timestamp:1706753695.76982
add water, water_:0,timestamp:1706753695.77082
add water, water_:0,timestamp:1706753695.77182
add water, water_:0,timestamp:1706753695.77282
add water, water_:0,timestamp:1706753695.77382
add water, water_:0,timestamp:1706753695.77482
add water, water_:0,timestamp:1706753695.77582
add water, water_:0,timestamp:1706753695.77682
last_update_i:50010,now_i:60008,qps:9998<---开始恢复，无陡增；
last_update_i:60008,now_i:70009,qps:10001<---开始恢复，无陡增；
last_update_i:70009,now_i:80009,qps:10000
last_update_i:80009,now_i:90010,qps:10001
last_update_i:90010,now_i:100011,qps:10001
。。。。。。
（中间类似的省略）
。。。。。。
last_update_i:400027,now_i:410028,qps:10001
last_update_i:410028,now_i:420028,qps:10000
last_update_i:420028,now_i:430029,qps:10001
   */
}