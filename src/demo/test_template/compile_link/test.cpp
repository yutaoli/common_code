#include "max.hpp"
#include <stdio.h>

void test_max();

int main()
{
    int a = max(1, 2);
    test_max();
    printf("%s,a:%d\n",__FUNCTION__,a);
    return 0;
}
