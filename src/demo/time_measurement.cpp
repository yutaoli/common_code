#include <iostream>
#include <stdlib.h>
#include <x86intrin.h>

using namespace std;

void ge_mul_div(int count) 
{
    int ret = 0;
    uint64_t start, end;
    float sum = 0;
    start = __rdtsc();
    for (int n = 0; n < count; n++)
        sum += (n*0.01);
    end = __rdtsc();
    cout << "sum of i*0.01 " << count << " took " << end - start << " ticks. " << sum << endl;

    sum = 0;
    start = __rdtsc();
    for (int n = 0; n < count; n++)
        sum += (n / 100.0);
    end = __rdtsc();
    cout << "sum of i/100.0 " << count << " took " << end - start << " ticks. " << sum << endl;
}

int main()
{
    ge_mul_div(1000000);
}
