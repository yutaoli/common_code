#include "stat_median.h"
#include <algorithm>
#include <stdio.h>

StatMedian::StatMedian()
{
    delta_ = 1;
    max_size_ = 0;
    sample_count_ = 0;
}
StatMedian::~ StatMedian()
{
}

int StatMedian::Init(unsigned int delta, unsigned int max_size)
{
    if(delta > max_size -1) return -1;
    if(max_size == 0) return -2;
    if(delta == 0) return -3;

    delta_ = delta;
    max_size_ = max_size;
    
    sample_count_ = 0;

    data_.resize(Size());
    for(unsigned int i = 0; i < data_.size();i++)
    {
        data_[i] = 0;
    }
    return 0;
}
void StatMedian::Add(unsigned int delay_us)
{
    unsigned int index = delay_us;
    if(index >= max_size_)
    {
        index = max_size_ - 1;
    }

    index /= delta_;
    data_[index]++;
    
    sample_count_++;
}

void StatMedian::Sort()
{
    sort_data_.resize(data_.size());
    sort_data_[0] = data_[0];
    for(unsigned int i = 1; i < data_.size(); i++)
    {
        sort_data_[i] = sort_data_[i - 1] + data_[i];
    }
}
 int StatMedian::Median(unsigned int &result)
{
    return MedianOfX(50, result);
}
 int StatMedian::MedianOf90(unsigned int &result)
{
    return MedianOfX(90, result);
}
 int StatMedian::MedianOf99(unsigned int &result)
{
   return MedianOfX(99, result);
}
 int StatMedian::MedianOfX(int x, unsigned int &result)
{
    if(x > 100 || x <=0) return -1;
    if(SampleCount() == 0) return -2;

    double ratio = double(x)/100;
    unsigned int ranking = ratio * SampleCount();
    if(ranking == 0) 
    {
        ranking = 1;
    }

    std::vector<unsigned long long>::const_iterator v_it;

    // ranking == a[lower_bound], return lower_bound;
    // ranking != a[lower_bound], return upper_bound;
    v_it= std::lower_bound(sort_data_.begin(), sort_data_.end(), ranking);
    if(v_it == sort_data_.end()) return -2;

    unsigned int index = v_it - sort_data_.begin();
    if(sort_data_[index] != ranking)
    {
        v_it = std::upper_bound(sort_data_.begin(), sort_data_.end(), ranking);
        if(v_it == sort_data_.end())
        {
            //printf("fatal error,v_it == sort_data_.end(),ratio:%lf,size:%u,ranking:%u",ratio,SampleCount(),ranking);
            return -3;
        }
        index = v_it - sort_data_.begin();
    }

    result = index * delta_;
    /*printf("x:%d,ranking:%u,index:%u,result:%u\n",
    x,ranking,index,result);*/
    return 0;
}
unsigned int StatMedian::SampleCount()
{
    return sample_count_;
}