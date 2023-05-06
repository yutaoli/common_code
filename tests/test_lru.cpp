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
    lru ca(4);
     
    ca.set(1, 1);
    ca.set(2, 2);
    ca.set(3, 3);
    ca.set(4, 4);
    ca.set(5, 5);// 5 4 3 2

    std::list<std::pair<int, int> >::iterator it = ca.queue.begin();
    CHECK(it->first == 5 && it->second == 5) ;

    it++;
    CHECK(it->first == 4 && it->second == 4);

    it++;
    CHECK(it->first == 3 && it->second == 3);

    it++;
    CHECK(it->first == 2 && it->second == 2);

    int value = ca.get(1);
    CHECK(value == -1);

    value = ca.get(2);// 2 5 4 3
    CHECK(value == 2);

    it = ca.queue.begin();
    CHECK(it->first == 2 && it->second == 2) ;

    it++;
    CHECK(it->first == 5 && it->second == 5);

    it++;
    CHECK(it->first == 4 && it->second == 4);

    it++;
    CHECK(it->first == 3 && it->second == 3);

    ca.set(4, 8);// 4 2 5 3
    it = ca.queue.begin();
    CHECK(it->first == 4 && it->second == 8) ;

    it++;
    CHECK(it->first == 2 && it->second == 2);

    it++;
    CHECK(it->first == 5 && it->second == 5);

    it++;
    CHECK(it->first == 3 && it->second == 3);
}
