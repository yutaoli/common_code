extern "C" 
{
#include <string.h>

#include "binary_search.h"
//#include "mq_errno.h"
}

#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(binary_search)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(binary_search, TestFind)
{
    int arr[] = { 2, 3, 4, 10, 40 };
    int x = 10;
    int n = sizeof(arr) / sizeof(arr[0]);
    int result = binarySearch(arr, 0, n - 1, x);
    CHECK(result == 3);
}

TEST(binary_search, TestNotFind)
{
    int arr[] = { 2, 3, 4, 10, 40 };
    int x = 9;
    int n = sizeof(arr) / sizeof(arr[0]);
    int result = binarySearch(arr, 0, n - 1, x);
    CHECK(result == -1);
}
