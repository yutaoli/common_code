#include <CppUTest/CommandLineTestRunner.h>

#include "cc_regex.h"

TEST_GROUP(cc_regex)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};

TEST(cc_regex, TestExtension)
{
    const std::string input = "hello.exe,,,a.jpg,this is a bird.css";
    const std::string re = "\\.([a-zA-Z0-9]+)$";

    int ret = 0;
    for (unsigned int i = 0; i < 2; i++)// 把CRegexSearchMatchAll和CppRegexSearchMatchAll都测试一遍
    {
        Matchs c_matchs;
        if (i == 0)
        {
            ret = CcRegex::CRegexSearchMatchAll(input, re, c_matchs);
        }
        else
        {
            ret = CcRegex::CppRegexSearchMatchAll(input, re, c_matchs);
        }
        CHECK(ret == 0);
        CHECK(c_matchs.size() == 1);
        CHECK(c_matchs[0].groups.size() == 2);
        CHECK(c_matchs[0].groups[0].str == ".css");
        CHECK(c_matchs[0].groups[1].str == "css");
    }
}

TEST(cc_regex, TestNoMatch)
{
     const std::string input = "12346789";
     const std::string re = "(\\w+):(\\w+);";

     int ret = 0;
    for (unsigned int i = 0; i < 2; i++)// 把CRegexSearchMatchAll和CppRegexSearchMatchAll都测试一遍
    {
        Matchs c_matchs;
        if (i == 0)
        {
            ret = CcRegex::CRegexSearchMatchAll(input, re, c_matchs);
        }
        else
        {
            ret = CcRegex::CppRegexSearchMatchAll(input, re, c_matchs);
        }
        CHECK(ret == 0);
        CHECK(c_matchs.size() == 0);
    }
   // printf("now :%s\n", cc_time::CcTime::NowString().c_str());
}

TEST(cc_regex, TestMatchOnce)
{
    const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
     const std::string re = "(\\w+):(\\w+);";

     int ret = 0;
    for (unsigned int i = 0; i < 2; i++)// 把CRegexSearchMatchAll和CppRegexSearchMatchAll都测试一遍
    {
        Matchs c_matchs;
        if (i == 0)
        {
            ret = CcRegex::CRegexSearchMatchAll(input, re, c_matchs);
        }
        else
        {
            ret = CcRegex::CppRegexSearchMatchAll(input, re, c_matchs);
        }
        CHECK(ret == 0);
        CHECK(c_matchs.size() == 1);

        CHECK(c_matchs[0].groups.size() == 3);
        CHECK(c_matchs[0].groups[0].str == "PQR:2;");
        CHECK(c_matchs[0].groups[1].str == "PQR");
        CHECK(c_matchs[0].groups[2].str == "2");
    }
   // printf("now :%s\n", cc_time::CcTime::NowString().c_str());
}

TEST(cc_regex, TestMatchMorethanonce)
{
    const std::string input = "hello@gmail-example.com;world@gmail.com;test@gmail.com;";
     const std::string re = "(\\w+([-+.]\\w+)*@\\w+([-+.]\\w+)*).com;";

     int ret = 0;
    for (unsigned int i = 0; i < 2; i++)// 把CRegexSearchMatchAll和CppRegexSearchMatchAll都测试一遍
    {
        Matchs c_matchs;
        if (i == 0)
        {
            ret = CcRegex::CRegexSearchMatchAll(input, re, c_matchs);
        }
        else
        {
            ret = CcRegex::CppRegexSearchMatchAll(input, re, c_matchs);
        }
        CHECK(ret == 0);
        CHECK(c_matchs.size() == 3);

        CHECK(c_matchs[0].groups.size() == 4);
        CHECK(c_matchs[0].groups[0].str == "hello@gmail-example.com;");
        CHECK(c_matchs[0].groups[1].str == "hello@gmail-example");
        CHECK(c_matchs[0].groups[2].str == "");
        CHECK(c_matchs[0].groups[3].str == "-example");

        CHECK(c_matchs[1].groups.size() == 4); 
        CHECK(c_matchs[1].groups[0].str == "world@gmail.com;");
        CHECK(c_matchs[1].groups[1].str == "world@gmail");
        CHECK(c_matchs[1].groups[2].str == "");
        CHECK(c_matchs[1].groups[3].str == "");

         CHECK(c_matchs[2].groups.size() == 4);
        CHECK(c_matchs[2].groups[0].str == "test@gmail.com;");
        CHECK(c_matchs[2].groups[1].str == "test@gmail");
        CHECK(c_matchs[2].groups[2].str == "");
        CHECK(c_matchs[2].groups[3].str == "");
    }
   // printf("now :%s\n", cc_time::CcTime::NowString().c_str());
}

TEST(cc_regex, TestMatchMorethanonce2)
{
    const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
     const std::string re = "(\\w+):([[:digit:]])";

     int ret = 0;
    for (unsigned int i = 0; i < 2; i++)// 把CRegexSearchMatchAll和CppRegexSearchMatchAll都测试一遍
    {
        Matchs c_matchs;
        if (i == 0)
        {
            ret = CcRegex::CRegexSearchMatchAll(input, re, c_matchs);
        }
        else
        {
            ret = CcRegex::CppRegexSearchMatchAll(input, re, c_matchs);
        }
        CHECK(ret == 0);
        //printf("i:%d,c_matchs.size:%u\n",i,c_matchs.size());
        CHECK(c_matchs.size() == 3);

        CHECK(c_matchs[0].groups.size() == 3);
        CHECK(c_matchs[0].groups[0].str == "ABC:1");
        CHECK(c_matchs[0].groups[1].str == "ABC");
        CHECK(c_matchs[0].groups[2].str == "1");

        CHECK(c_matchs[1].groups.size() == 3);
        CHECK(c_matchs[1].groups[0].str == "PQR:2");
        CHECK(c_matchs[1].groups[1].str == "PQR");
        CHECK(c_matchs[1].groups[2].str == "2");

        CHECK(c_matchs[2].groups.size() == 3);
        CHECK(c_matchs[2].groups[0].str == "XYZ:3");
        CHECK(c_matchs[2].groups[1].str == "XYZ");
        CHECK(c_matchs[2].groups[2].str == "3");
    }
   // printf("now :%s\n", cc_time::CcTime::NowString().c_str());
}

TEST(cc_regex, TestMatchHttpReqline)
{
    const std::string input = "http://10.203.60.5:9122/heartbeat?param=4&b=3";
    const std::string re = "^([A-Za-z]+):\\/\\/([0-9A-Za-z\\.\\-]+)(:([0-9]+))?(\\/([^?#]*))?(\\?([^#]*))?$";

     int ret = 0;
    for (unsigned int i = 0; i < 2; i++)// 把CRegexSearchMatchAll和CppRegexSearchMatchAll都测试一遍
    {
        Matchs c_matchs;
        if (i == 0)
        {
            ret = CcRegex::CRegexSearchMatchAll(input, re, c_matchs);
        }
        else
        {
            ret = CcRegex::CppRegexSearchMatchAll(input, re, c_matchs);
        }
        //printf("ret:%d,i:%d,c_matchs.size:%u\n",ret,i,c_matchs.size());
        CHECK(ret == 0);
        CHECK(c_matchs.size() == 1);

        CHECK(c_matchs[0].groups.size() == 9);
        CHECK(c_matchs[0].groups[0].str == "http://10.203.60.5:9122/heartbeat?param=4&b=3");
        CHECK(c_matchs[0].groups[1].str == "http");
        CHECK(c_matchs[0].groups[2].str == "10.203.60.5");
        CHECK(c_matchs[0].groups[3].str == ":9122");
        CHECK(c_matchs[0].groups[4].str == "9122");
        CHECK(c_matchs[0].groups[5].str == "/heartbeat");
        CHECK(c_matchs[0].groups[6].str == "heartbeat");
        CHECK(c_matchs[0].groups[7].str == "?param=4&b=3");
        CHECK(c_matchs[0].groups[8].str == "param=4&b=3");
    }
   // printf("now :%s\n", cc_time::CcTime::NowString().c_str());
}