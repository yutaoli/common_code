#include <CppUTest/CommandLineTestRunner.h>
#include <string>
#include "hash_table_open_hashing.hpp"

TEST_GROUP(hash_table_open_hashing)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(hash_table_open_hashing, test_normal)
{
    hash_table_open_hashing<int, int> intHash(0.1, 10);
    hash_table_open_hashing<std::string, std::string> strHash;
    intHash[1] = 11;
    intHash[2] = 12;
    // Add one case to demonstrate hash collision
    intHash[1025] = 1025;
    CHECK(intHash.get_capacity() == 20);

    CHECK(intHash[1] == 11);
    CHECK(intHash[2] == 12);
    CHECK(intHash[1025] == 1025);
    CHECK(intHash.get_size() == 3);

    strHash["abc"] = "cde";
    strHash["bcd"] = "fgh";
    // Add one case to demonstrate hash collision
    strHash["bca"] = "bca";

    CHECK(strHash["abc"] == "cde");
    CHECK(strHash["bcd"] == "fgh");
    CHECK(strHash["bca"] == "bca");
    CHECK(strHash.get_size() == 3);
}
