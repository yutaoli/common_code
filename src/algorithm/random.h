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

*/