#include "random.h"
#include <stdlib.h>
#include <time.h>

int RandomInit()
{
    srand(time(NULL));
    return 0;
}

long long Rand64()
{
    return (long long)(rand()) * RAND_MAX + rand();
}

int NChooseM(unsigned int N, unsigned int M, std::vector<unsigned int> &out)
{
    if(N<M) return -1;
    if(M==0) return -2;

    out.clear();  
    for(unsigned int i = 0; i < N && M > 0; i++)
    {
        if(Rand64() % (N - i) < M)
        {
            out.push_back(i);
            M--;
        }
    }
    return 0;
}

int Shuffle(std::vector<unsigned int> &ids)
{
    if(ids.empty() || ids.size() == 1) return 0;

    for(unsigned int i = ids.size() -1; i > 0; i--)
    {
        unsigned int j = rand() % (i + 1);// [0, i+1)

        unsigned int temp = ids[i];
        ids[i] = ids[j];
        ids[j] = temp;
    }
    return 0;
}

long long Rand64_r(unsigned int *seedp)
{
    long long high = rand_r(seedp);
    long long low = rand_r(seedp);
    return high * RAND_MAX + low;
}

int NChooseM_r(unsigned int N, unsigned int M, std::vector<unsigned int> &out, unsigned int *seedp)
{
    if(N<M) return -1;
    if(M==0) return -2;

    out.clear();  
    for(unsigned int i = 0; i < N && M > 0; i++)
    {
        if(Rand64_r(seedp) % (N - i) < M)
        {
            out.push_back(i);
            M--;
        }
    }
    return 0;
}

// 对ids，随机洗牌
int Shuffle_r(std::vector<unsigned int> &ids, unsigned int *seedp)
{
    if(ids.empty() || ids.size() == 1) return 0;

    for(unsigned int i = ids.size() -1; i > 0; i--)
    {
        unsigned int j = rand_r(seedp) % (i + 1);// [0, i+1)

        unsigned int temp = ids[i];
        ids[i] = ids[j];
        ids[j] = temp;
    }
    return 0;
}