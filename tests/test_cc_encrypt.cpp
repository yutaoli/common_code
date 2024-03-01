#include <CppUTest/CommandLineTestRunner.h>

#include "cc_encrypt.h"

TEST_GROUP(cc_encrypt)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};

TEST(cc_encrypt, TestOk)
{
    //CHECK(cc_encrypt::CcEncrypt::Sha1("hello2 world") == "22596363b3de40b06f981fb85d82312e8c0ed511");
    CHECK(cc_encrypt::CcEncrypt::Sha1("hello world") == "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
}
