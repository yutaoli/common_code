#ifndef STAT_MEDIAN_H
#define STAT_MEDIAN_H

#include <vector>

class StatMedian
{
public:
StatMedian();
virtual ~ StatMedian();

public:
// 最大的数不超过max_size，如1000,000，故用下标作为桶处理
// 内部的正常的delay取值[0, max_size)
// delta:每隔delta算一个计时单元
int Init(unsigned int delta, unsigned int max_size);
void Add(unsigned int delay_us);

void Sort();
 int Median(unsigned int &result);
 int MedianOf90(unsigned int &result);
 int MedianOf99(unsigned int &result);

 public:
unsigned long long Count(unsigned int index){return data_[index];}
unsigned int Size(){return max_size_ / delta_ + (max_size_ % delta_ != 0); }
private:
 int MedianOfX(int x,unsigned int &result);
  unsigned long long SampleCount();

private:
std::vector<unsigned long long> data_;
unsigned int delta_;
unsigned int max_size_;

unsigned long long sample_count_;
};
/*
StatMedian stat_median;
stat_median.Init(1, 1000000);

stat_median.Add(1);
stat_median.Add(21);
stat_median.Add(10);
...

if(stat_median.Size()>0)
{
    stat_median.Sort();
printf("media:%u,median_of_90%:%u,median_of_99%:%u\n",
stat_median.Median(), stat_median.MedianOf90(), stat_median.MedianOf99());
}
*/
#endif
