#ifndef RANDOM_H
#define RANDOM_H

#include <vector>

int RandomInit();

// 等概率产生64位整数
long long Rand64();

// 从N个数[0,N)里随机取M个；N>=M
int NChooseM(unsigned int N, unsigned int M, std::vector<unsigned int> &out);

// 对ids，随机洗牌
int Shuffle(std::vector<unsigned int> &ids);

////// 线程安全版本//////
// 等概率产生64位整数
long long Rand64_r(unsigned int *seedp);

// 从N个数[0,N)里随机取M个；N>=M
int NChooseM_r(unsigned int N, unsigned int M, std::vector<unsigned int> &out, unsigned int *seedp);

// 对ids，随机洗牌
int Shuffle_r(std::vector<unsigned int> &ids, unsigned int *seedp);

#endif
/*
1、NChooseM:
RandomInit();

int N = 100;
int M = 2;
std::vector<unsigned int> out;
out.size(M);
NChooseM(N, M, out);

2、Shuffle:
RandomInit();

int N = 10;
std::vector<unsigned int> out;
out.resize(N);
for(unsigned int i =0; i < N; i++)
{
    out[i] = i;
}
Shuffle(out);

////// 线程安全版本//////
1、NChooseM_r
int N = 100;
int M = 2;
std::vector<unsigned int> out;
out.size(M);

unsigned int seed = time(NULL);// 只能调用一次
NChooseM_r(N, M, out, &seed);// 可以调用N次；

2、Shuffle_r:
int N = 10;
std::vector<unsigned int> out;
out.resize(N);
for(unsigned int i =0; i < N; i++)
{
    out[i] = i;
}
unsigned int seed = time(NULL);// 只能调用一次
Shuffle_r(out, &seed);// 可以调用N次；

*/