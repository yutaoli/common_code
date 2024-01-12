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
   for (unsigned int i = 0; i < rate_per_second; i++)
   {
      bool can_access = rate_server_controller.CanAccess();
      CHECK(can_access == true);
   }

   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   for (unsigned int i = 0; i < rate_per_second; i++)
   {
      bool can_access = rate_server_controller.CanAccess();
      CHECK(can_access == false);
   }

   std::this_thread::sleep_for(std::chrono::seconds(1));
   for (unsigned int i = 0; i < rate_per_second; i++)
   {
      bool can_access = rate_server_controller.CanAccess();
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
   unsigned int req_count = 100000;
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
      if(rate_controller.CanAccess() == false) i = (i == 0 ? 0 : i - 1);

      if (i == req_count/10)// 模拟掉坑10s
      {
         //printf("chose:%d\n",1);
         std::this_thread::sleep_for(std::chrono::seconds(10));
      }
      std::this_thread::sleep_for(std::chrono::microseconds(rand()%200));// 模拟process延迟
      rate_controller.Wait();
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
chose:1
last_update_i:4000,now_i:6000,qps:2000
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6000,qps:0<--
last_update_i:6000,now_i:6003,qps:3<--
last_update_i:6003,now_i:10000,qps:3997<--没有连续多个陡增了，还是会有一个陡增，可能是和qps统计线程与server_controller的计时错开有关；
last_update_i:10000,now_i:12000,qps:2000
last_update_i:12000,now_i:14000,qps:2000
last_update_i:14000,now_i:16000,qps:2000
last_update_i:16000,now_i:17303,qps:1303<--还是会有一个陡减，可能是和两个controller计时的叠加造成的
last_update_i:17303,now_i:19303,qps:2000
last_update_i:19303,now_i:21303,qps:2000
last_update_i:21303,now_i:23303,qps:2000
last_update_i:23303,now_i:25303,qps:2000
last_update_i:25303,now_i:27303,qps:2000
last_update_i:27303,now_i:29303,qps:2000
   */
}