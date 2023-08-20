#include <CppUTest/CommandLineTestRunner.h>
#include "stat_median.h"
#include <algorithm>

//#define PRINT_DEBUG 

TEST_GROUP(stat_median)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

unsigned int MedianOfX(const std::vector<unsigned int> &delay, int x)
{
    if(x > 100 || x <=0) return -1;
    if(delay.size()==0) return 0;

    unsigned int ranking = (double(x)/100 * delay.size());
    if(ranking == 0)
    {
        ranking = 1;
    }
    return delay[ranking - 1];
}

unsigned int Median(const std::vector<unsigned int> &delay)
{
    return MedianOfX(delay, 50);
}

unsigned int MedianOf90(const std::vector<unsigned int> &delay)
{
    return MedianOfX(delay, 90);
}

unsigned int MedianOf99(const std::vector<unsigned int> &delay)
{
    return MedianOfX(delay, 99);
}

// delta==1,测试匹配边界：刚好匹配到50分位的边界，50分位数排行第10，刚好最小的前10个都是1，然后是1个2
TEST(stat_median,test_fix_50)
{
    StatMedian stat_median;
    unsigned int max_size = 11;
    CHECK(stat_median.Init(1,max_size) == 0);

    //std::vector<unsigned int > delay;
    for(unsigned int i = 0; i < 10; i++)// 10个1
    {
        stat_median.Add(1);
    }
    stat_median.Add(2);// 1个2
    for(unsigned int i = 0; i < 9; i++)// 9个3
    {
        stat_median.Add(3);
    }

    stat_median.Sort();

    int median_of_50_ret = 0;
    int median_of_90_ret = 0;
    int median_of_99_ret = 0;
    unsigned int median_of_50 = 0;
    unsigned int median_of_90 = 0;
    unsigned int median_of_99 = 0;
    median_of_50_ret = stat_median.Median(median_of_50);
    median_of_90_ret = stat_median.MedianOf90(median_of_90);
    median_of_99_ret = stat_median.MedianOf99(median_of_99);
    CHECK(median_of_50_ret == 0);
    CHECK(median_of_90_ret == 0);
    CHECK(median_of_99_ret == 0);
    CHECK(median_of_50 == 1);
    CHECK(median_of_90 == 3);
    CHECK(median_of_99 == 3);
}

// delta==1,测试匹配边界：刚好不匹配到50分位的边界，50分位排行第11，10个1，8个2，所以命中2
TEST(stat_median,test_not_fix_50)
{
    StatMedian stat_median;
    unsigned int max_size = 11;
    CHECK(stat_median.Init(1,max_size) == 0);

    //std::vector<unsigned int > delay;
    for(unsigned int i = 0; i < 10; i++)// 10个1
    {
        stat_median.Add(1);
    }
    for(unsigned int i = 0; i < 8; i++)// 8个2
    {
        stat_median.Add(2);
    }

    for(unsigned int i = 0; i < 4; i++)// 4个3
    {
        stat_median.Add(3);
    }

    stat_median.Sort();

    int median_of_50_ret = 0;
    int median_of_90_ret = 0;
    int median_of_99_ret = 0;
    unsigned int median_of_50 = 0;
    unsigned int median_of_90 = 0;
    unsigned int median_of_99 = 0;
    median_of_50_ret = stat_median.Median(median_of_50);
    median_of_90_ret = stat_median.MedianOf90(median_of_90);
    median_of_99_ret = stat_median.MedianOf99(median_of_99);
    CHECK(median_of_50_ret == 0);
    CHECK(median_of_90_ret == 0);
    CHECK(median_of_99_ret == 0);
    CHECK(median_of_50 == 2);
    CHECK(median_of_90 == 3);
    CHECK(median_of_99 == 3);
}

// delta==1,桶少，样本大，且桶量<<样本量
TEST(stat_median,test_bucket_small_sample_big)
{
    std::vector<unsigned int > delay;
    StatMedian stat_median;
    unsigned int max_size = 10;
    CHECK(stat_median.Init(1,max_size) == 0);

    for(unsigned int i = 1; i <= 10000; i++)
    {
        stat_median.Add(i);
    }

    stat_median.Sort();

    int median_of_50_ret = 0;
    int median_of_90_ret = 0;
    int median_of_99_ret = 0;
    unsigned int median_of_50 = 0;
    unsigned int median_of_90 = 0;
    unsigned int median_of_99 = 0;
    median_of_50_ret = stat_median.Median(median_of_50);
    median_of_90_ret = stat_median.MedianOf90(median_of_90);
    median_of_99_ret = stat_median.MedianOf99(median_of_99);

    CHECK(median_of_50_ret==0);
    CHECK(median_of_90_ret==0);
    CHECK(median_of_99_ret==0);
    CHECK(median_of_50 == 9);
    CHECK(median_of_90 == 9);
    CHECK(median_of_99 == 9);
}

// delta==1,桶大，样本少，且桶量>>样本量
TEST(stat_median,test_bucket_big_sample_small)
{
    StatMedian stat_median;
    unsigned int max_size = 1000000;
    CHECK(stat_median.Init(1,max_size) == 0);

    stat_median.Add(99);
    stat_median.Add(999);

    stat_median.Sort();

    int median_of_50_ret = 0;
    int median_of_90_ret = 0;
    int median_of_99_ret = 0;
    unsigned int median_of_50 = 0;
    unsigned int median_of_90 = 0;
    unsigned int median_of_99 = 0;
    median_of_50_ret = stat_median.Median(median_of_50);
    median_of_90_ret = stat_median.MedianOf90(median_of_90);
    median_of_99_ret = stat_median.MedianOf99(median_of_99);

    CHECK(median_of_50_ret==0);
    CHECK(median_of_90_ret==0);
    CHECK(median_of_99_ret==0);
    CHECK(median_of_50 == 99);
    CHECK(median_of_90 == 99);
    CHECK(median_of_99 == 99);
}


// delta==1,一个随机的case，量少：桶少，样本大，且桶量<<样本量
TEST(stat_median,test_one_random_case_bucket_small_sample_big)
{
    std::vector<unsigned int > delay;
    std::vector<unsigned int > delay_backup;
    StatMedian stat_median;
    unsigned int max_size = 3;
    CHECK(stat_median.Init(1,max_size) == 0);

    srand(time(NULL));
    for(unsigned int i = 1; i < 10000; i++)
    {
        unsigned int x = rand()% stat_median.Size();
        stat_median.Add(x);
        delay.push_back(x);
    }
    delay_backup = delay;

    std::sort(delay.begin(), delay.end());
    stat_median.Sort();

    int median_of_50_1_ret = 0;
    int median_of_90_1_ret = 0;
    int median_of_99_1_ret = 0;
    unsigned int median_of_50_1 = 0;
    unsigned int median_of_90_1 = 0;
    unsigned int median_of_99_1 = 0;
    median_of_50_1_ret = stat_median.Median(median_of_50_1);
    median_of_90_1_ret = stat_median.MedianOf90(median_of_90_1);
    median_of_99_1_ret = stat_median.MedianOf99(median_of_99_1);

    unsigned int median_of_50_2 = Median(delay);
    unsigned int median_of_90_2 = MedianOf90(delay);
    unsigned int median_of_99_2 = MedianOf99(delay);

#ifdef PRINT_DEBUG
    for(unsigned int i = 0; i < stat_median.Size(); i++)
    {
        printf("i:%d,count:%llu,sort_count:%llu\n",
                i,stat_median.Count(i),stat_median.SortCount(i));
    }
    printf("median_of_50_1:%u,median_of_90_1:%u,median_of_99_1:%u\n",
            median_of_50_1,median_of_90_1,median_of_99_1);
    printf("======\n");
    for(unsigned int i = 0; i < delay.size(); i++)
    {
        printf("%d,",delay[i]);
    }
    printf("\n");

    printf("median_of_50_2:%u,median_of_90_2:%u,median_of_99_2:%u\n",
            median_of_50_2,median_of_90_2,median_of_99_2);
#endif

    CHECK(median_of_50_1_ret==0);
    CHECK(median_of_90_1_ret==0);
    CHECK(median_of_99_1_ret==0);
    CHECK(median_of_50_1 == median_of_50_2);
    CHECK(median_of_90_1 == median_of_90_2);
    CHECK(median_of_99_1 == median_of_99_2);
}

// delta==1,一个随机的case，量少：桶大，样本少，且桶量>>样本量
TEST(stat_median,test_one_random_case_bucket_big_sample_small)
{
    std::vector<unsigned int > delay;
    std::vector<unsigned int > delay_backup;
    StatMedian stat_median;
    unsigned int max_size = 1000000;
    CHECK(stat_median.Init(1,max_size) == 0);

    srand(time(NULL));
    for(unsigned int i = 1; i < 3; i++)
    {
        unsigned int x = rand()% stat_median.Size();
        stat_median.Add(x);
        delay.push_back(x);
    }
    delay_backup = delay;

    std::sort(delay.begin(), delay.end());
    stat_median.Sort();

    int median_of_50_1_ret = 0;
    int median_of_90_1_ret = 0;
    int median_of_99_1_ret = 0;
    unsigned int median_of_50_1 = 0;
    unsigned int median_of_90_1 = 0;
    unsigned int median_of_99_1 = 0;
    median_of_50_1_ret = stat_median.Median(median_of_50_1);
    median_of_90_1_ret = stat_median.MedianOf90(median_of_90_1);
    median_of_99_1_ret = stat_median.MedianOf99(median_of_99_1);

    unsigned int median_of_50_2 = Median(delay);
    unsigned int median_of_90_2 = MedianOf90(delay);
    unsigned int median_of_99_2 = MedianOf99(delay);

    /*for(unsigned int i = 0; i < stat_median.Size(); i++)
      {
      printf("i:%d,count:%llu,sort_count:%llu\n",
      i,stat_median.Count(i),stat_median.SortCount(i));
      }
      printf("median_of_50_1:%u,median_of_90_1:%u,median_of_99_1:%u\n",
      median_of_50_1,median_of_90_1,median_of_99_1);
      printf("======\n");
      for(unsigned int i = 0; i < delay.size(); i++)
      {
      printf("%d,",delay[i]);
      }
      printf("\n");

      printf("median_of_50_2:%u,median_of_90_2:%u,median_of_99_2:%u\n",
      median_of_50_2,median_of_90_2,median_of_99_2);*/
    CHECK(median_of_50_1_ret==0);
    CHECK(median_of_90_1_ret==0);
    CHECK(median_of_99_1_ret==0);
    CHECK(median_of_50_1 == median_of_50_2);
    CHECK(median_of_90_1 == median_of_90_2);
    CHECK(median_of_99_1 == median_of_99_2);
}

// delta==1,一个随机的case，并且量大
TEST(stat_median,test_one_big_random_case)
{
    std::vector<unsigned int > delay;
    std::vector<unsigned int > delay_backup;
    StatMedian stat_median;
    unsigned int max_size = 1000000;
    CHECK(stat_median.Init(1,max_size) == 0);

    srand(time(NULL));
    for(unsigned int i = 1; i < 10000000; i++)
    {
        unsigned int x = rand() * rand()  % stat_median.Size();
        stat_median.Add(x);
        delay.push_back(x);
    }
    delay_backup = delay;

    std::sort(delay.begin(), delay.end());
    stat_median.Sort();

    int median_of_50_1_ret = 0;
    int median_of_90_1_ret = 0;
    int median_of_99_1_ret = 0;
    unsigned int median_of_50_1 = 0;
    unsigned int median_of_90_1 = 0;
    unsigned int median_of_99_1 = 0;
    median_of_50_1_ret = stat_median.Median(median_of_50_1);
    median_of_90_1_ret = stat_median.MedianOf90(median_of_90_1);
    median_of_99_1_ret = stat_median.MedianOf99(median_of_99_1);

    unsigned int median_of_50_2 = Median(delay);
    unsigned int median_of_90_2 = MedianOf90(delay);
    unsigned int median_of_99_2 = MedianOf99(delay);

    /*for(unsigned int i = 0; i < stat_median.Size(); i++)
      {
      printf("i:%d,count:%llu,sort_count:%llu\n",
      i,stat_median.Count(i),stat_median.SortCount(i));
      }
      printf("median_of_50_1:%u,median_of_90_1:%u,median_of_99_1:%u\n",
      median_of_50_1,median_of_90_1,median_of_99_1);
      printf("======\n");
      for(unsigned int i = 0; i < delay.size(); i++)
      {
      printf("%d,",delay[i]);
      }
      printf("\n");

      printf("median_of_50_2:%u,median_of_90_2:%u,median_of_99_2:%u\n",
      median_of_50_2,median_of_90_2,median_of_99_2);*/
    CHECK(median_of_50_1_ret==0);
    CHECK(median_of_90_1_ret==0);
    CHECK(median_of_99_1_ret==0);
    CHECK(median_of_50_1 == median_of_50_2);
    CHECK(median_of_90_1 == median_of_90_2);
    CHECK(median_of_99_1 == median_of_99_2);
}

// delta==1, N个随机的case，并且量大
TEST(stat_median,test_N_big_random_case)
{
    int N = 100;
    while(N--)
    {
        std::vector<unsigned int > delay;
        std::vector<unsigned int > delay_backup;
        StatMedian stat_median;
        unsigned int max_size = 100000;
        CHECK(stat_median.Init(1,max_size) == 0);

        srand(time(NULL));
        for(unsigned int i = 1; i < 100000; i++)
        {
            unsigned int x = rand() * rand() % stat_median.Size();
            stat_median.Add(x);
            delay.push_back(x);
        }
        delay_backup = delay;

        std::sort(delay.begin(), delay.end());
        stat_median.Sort();

        int median_of_50_1_ret = 0;
        int median_of_90_1_ret = 0;
        int median_of_99_1_ret = 0;
        unsigned int median_of_50_1 = 0;
        unsigned int median_of_90_1 = 0;
        unsigned int median_of_99_1 = 0;
        median_of_50_1_ret = stat_median.Median(median_of_50_1);
        median_of_90_1_ret = stat_median.MedianOf90(median_of_90_1);
        median_of_99_1_ret = stat_median.MedianOf99(median_of_99_1);

        unsigned int median_of_50_2 = Median(delay);
        unsigned int median_of_90_2 = MedianOf90(delay);
        unsigned int median_of_99_2 = MedianOf99(delay);

        /*for(unsigned int i = 0; i < stat_median.Size(); i++)
          {
          printf("i:%d,count:%llu,sort_count:%llu\n",
          i,stat_median.Count(i),stat_median.SortCount(i));
          }
          printf("median_of_50_1:%u,median_of_90_1:%u,median_of_99_1:%u\n",
          median_of_50_1,median_of_90_1,median_of_99_1);
          printf("======\n");
          for(unsigned int i = 0; i < delay.size(); i++)
          {
          printf("%d,",delay[i]);
          }
          printf("\n");

          printf("median_of_50_2:%u,median_of_90_2:%u,median_of_99_2:%u\n",
          median_of_50_2,median_of_90_2,median_of_99_2);*/
        CHECK(median_of_50_1_ret==0);
        CHECK(median_of_90_1_ret==0);
        CHECK(median_of_99_1_ret==0);
        /*printf("median_of_50_1:%u,median_of_50_2:%u,delay1:%u\n",
          median_of_50_1,median_of_50_2,delay[0]);*/
        CHECK(median_of_50_1 == median_of_50_2);
        CHECK(median_of_90_1 == median_of_90_2);
        CHECK(median_of_99_1 == median_of_99_2);
    }
}

// 正常delta=2的情况
TEST(stat_median,test_delta_queal_2_normal_case)
{
    StatMedian stat_median;
    unsigned int max_size = 1000000;
    unsigned int delta = 2;
    CHECK(stat_median.Init(delta,max_size) == 0);

    for(unsigned int i = 1; i <= 8; i++)
    {
        stat_median.Add(i);
    }

    stat_median.Sort();

    int median_of_50_ret = 0;
    int median_of_90_ret = 0;
    int median_of_99_ret = 0;
    unsigned int median_of_50 = 0;
    unsigned int median_of_90 = 0;
    unsigned int median_of_99 = 0;
    median_of_50_ret = stat_median.Median(median_of_50);
    median_of_90_ret = stat_median.MedianOf90(median_of_90);
    median_of_99_ret = stat_median.MedianOf99(median_of_99);

    CHECK(median_of_50_ret==0);
    CHECK(median_of_90_ret==0);
    CHECK(median_of_99_ret==0);
    CHECK(median_of_50 == 4);
    CHECK(median_of_90 == 6);
    CHECK(median_of_99 == 6);
}
