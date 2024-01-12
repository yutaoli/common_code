#include <CppUTest/CommandLineTestRunner.h>
#include <thread>
#include <unistd.h>

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

TEST(TestRateController, TestOk)
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