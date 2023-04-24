#include "hash_table.h"
#include <CppUTest/CommandLineTestRunner.h>

using namespace std;

TEST_GROUP(hash_table)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(hash_table,test_ok)
{
    Hash h(7);

    h.insert(make_pair(3,4));
    h.insert(make_pair(3,5));
    h.insert(make_pair(3,6));
    list<pair<int,int> >::iterator kv_it;
    bool find = h.get(3, kv_it);
    CHECK(find == true);
    CHECK(kv_it->first == 3 && kv_it->second == 6);

    h.insert(make_pair(10,6));
    h.insert(make_pair(10,16));
    find = h.get(10, kv_it);
    CHECK(find == true);
    CHECK(kv_it->first == 10 && kv_it->second == 16);

    find = h.get(1,kv_it);
    CHECK(find == false);

    h.remove(10);
    find = h.get(10,kv_it);
    CHECK(find == false);

    //h.displayHash();
}
