#include "max.hpp"
#include <stdio.h>

void test_max()
{
    int a = max(3, 4);
    printf("%s,a:%d\n",__FUNCTION__,a);
}
