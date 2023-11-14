#include <CppUTest/CommandLineTestRunner.h>
#include <thread>
#include <unistd.h>

#include "string_util.h"

TEST_GROUP(StringUtil)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};

TEST(StringUtil, TestEmpty)
{
   std::string str;
   std::string delimiter;
   std::vector<std::string> out;
   int ret = StringUtil::Spilt(str,delimiter,out);
   CHECK(ret == 0);
   CHECK(out.size() == 0);
}

TEST(StringUtil, TestOk)
{
   std::string str="1,2,3";
   std::string delimiter=",";
   std::vector<std::string> out;
   int ret = StringUtil::Spilt(str,delimiter,out);
   CHECK(ret == 0);
   CHECK(out.size() == 3);
   CHECK(out[0] == "1");
   CHECK(out[1] == "2");
   CHECK(out[2] == "3");
}

TEST(StringUtil, TestOkEndOfDelimiter)
{
   std::string str="1,,2,,";
   std::string delimiter=",,";
   std::vector<std::string> out;
   int ret = StringUtil::Spilt(str,delimiter,out);
   CHECK(ret == 0);
   CHECK(out.size() == 3);
   CHECK(out[0] == "1");
   CHECK(out[1] == "2");
   CHECK(out[2] == "");
}

TEST(StringUtil, TestOkEmptyElementInTheMiddle)
{
   std::string str="1,,2,,,,,";
   std::string delimiter=",";
   std::vector<std::string> out;
   int ret = StringUtil::Spilt(str,delimiter,out);
   CHECK(ret == 0);
   CHECK(out.size() == 8);
   CHECK(out[0] == "1");
   CHECK(out[1] == "");
   CHECK(out[2] == "2");
   CHECK(out[3] == "");
   CHECK(out[4] == "");
   CHECK(out[5] == "");
   CHECK(out[6] == "");
   CHECK(out[7] == "");
}

TEST(StringUtil, TestNoDelimiter)
{
   std::string str="123";
   std::string delimiter=",";
   std::vector<std::string> out;
   int ret = StringUtil::Spilt(str,delimiter,out);
   CHECK(ret == 0);
   CHECK(out.size() == 1);
   CHECK(out[0] == "123");
}