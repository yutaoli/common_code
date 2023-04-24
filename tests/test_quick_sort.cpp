extern "C" 
{
#include <string.h>

#include "quick_sort.h"
}

#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(quick_sort)
{
    void setup()
    {
    }
    void teardown()
    {
    }

    bool sorted(int *data, int n)
    {
        bool is_sorted = true;
        for(int i = 0; i < n-1; i++) 
        {
            if(data[i] > data[i+1])
            {
                is_sorted = false;
                break;
            }
        }
        return is_sorted;
    }
};

TEST(quick_sort, test_normal)
{
    int data[] = {8, 7, 2, 1, 0, 9, 6};

    int n = sizeof(data) / sizeof(data[0]);

    // perform quicksort on data
    quick_sort(data, 0, n - 1);

    CHECK(sorted(data, n) == true);
}

TEST(quick_sort, test_two_element)
{
    int data[] = {8, 7};

    int n = sizeof(data) / sizeof(data[0]);

    // perform quicksort on data
    quick_sort(data, 0, n - 1);

    CHECK(sorted(data, n) == true);
}
