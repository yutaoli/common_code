extern "C" 
{
//#include <string.h>

}

#include "lru.h"
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(lru)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(lru,test_ok)
{
    LRUCache ca(4);
     
    ca.refer(1);
    ca.refer(2);
    ca.refer(3);
    ca.refer(1);
    ca.refer(4);
    ca.refer(5);

    list<int>::iterator it = ca.dq.begin();
    CHECK(*it == 5) ;

    it++;
    CHECK(*it == 4);

    it++;
    CHECK(*it == 1);

    it++;
    CHECK(*it == 3);
}
