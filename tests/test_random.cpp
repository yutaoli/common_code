#include <CppUTest/CommandLineTestRunner.h>
#include "random.h"

TEST_GROUP(Random)
{
    void setup()
    {

    } 
    void teardown()
    {

    }
};

TEST(Random, TestNChooseM_Error)
{
    unsigned int N = 100;
    unsigned int M = 0;
    std::vector<unsigned int> out;

    RandomInit();
    int ret = NChooseM(N, M, out);
    CHECK(ret == -2);

     ret = NChooseM(N, N+1, out);
    CHECK(ret == -1);
}

// 从N个数中随机选M个数
TEST(Random, TestNChooseM_100wCaseOk)
{
    unsigned int N = 100;
    unsigned int M = 9;
    std::vector<unsigned int> out;

    //printf("rand:%d,RAND_MAX:%d,Rand64:%lld\n",rand(),RAND_MAX,Rand64());

    RandomInit();

    std::vector<unsigned int> counts;
    counts.resize(N);
    for(unsigned int i = 0; i < N; i++)
    {
        counts[i] = 0;
    }

    int ret = 0;
    int left = 1000000;
    while(left--)
    {
        ret = NChooseM(N, M, out);
        CHECK(ret == 0);
        for (unsigned int i = 0; i < out.size(); i++)
        {
            counts[out[i]]++;
        }
    }

    /*for(unsigned int i = 0; i < N; i++)
    {
       printf("counts[%u]:%u\n",i,counts[i]);
    }
    printf("\n");*/

    CHECK(counts[0] != 0);
    double ratio = 0;
    for(unsigned int i = 1; i < N; i++)
    {
        CHECK(counts[i] != 0);
        if(counts[i] > counts[0])
        {
            ratio = double(counts[0])/counts[i];
        }
        else 
        {
            ratio = double(counts[i])/counts[0];
        }
        CHECK(ratio > 0.98);
        /*printf("ratio[%u]:%lf\n",i, ratio);
        if(ratio <= 0.98)
        {
            printf("===ratio[%u]:%lf\n",i, ratio);
        }*/
    }
}

TEST(Random, TestShuffle_100wCaseOk)
{
    RandomInit();

    unsigned int N = 10;
    std::vector<std::vector<unsigned int> > values;
    values.resize(N);
    for(unsigned int i = 0; i < N; i++)
    {
        values[i].resize(N);
    }

    std::vector<unsigned int> out;
    out.resize(N);

    int left = 1000000;
    while(left--)
    {
        for (unsigned int i = 0; i < N; i++)
        {
            out[i] = i;
        }

        Shuffle(out);
        for (unsigned int i = 0; i < N; i++)
        {
            values[out[i]][i]++;
        }
    }

    /*printf("\n");
    for (unsigned int i = 0; i < N; i++)
    {
        printf("%03u:", i);
        for (unsigned int j = 0; j < N; j++)
        {
            printf("%09u ", values[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    // 每一行都和第[0]个元素比；
    for(unsigned int i = 0; i < N; i++)
    {
        CHECK(values[i][0] != 0);
        double ratio = 0;

        //printf("%03u:", i);
        for(unsigned int j = 1; j < N; j++)
        {
            CHECK(values[i][j] != 0);
            if(values[i][0] > values[i][j])
            {
                ratio = double(values[i][j])/values[i][0];
            }
            else 
            {
                ratio = double(values[i][0])/values[i][j];
            }
            //printf("%09lf ", ratio);
            CHECK(ratio > 0.98);
        }
        //printf("\n");
    }

    // 每列比：values[0][0]与values[1][0],...values[n][0]比，即每个代表都和values[0][0]（第0行的代表比）
    //printf("\n");
    double ratio = 0;
    CHECK(values[0][0] != 0);
    for(unsigned int i = 1; i < N; i++)
    {
        CHECK(values[i][0] != 0);
        if(values[i][0] > values[0][0])
        {
            ratio = double(values[0][0])/values[i][0];
        }
        else 
        {
            ratio = double(values[i][0])/values[0][0];
        }
        //printf("%09lf ", ratio);
        CHECK(ratio > 0.98);
    }
    //printf("\n");
    /*
    000:000100065 000100055 000099965 000099823 000100032 000099648 000100900 000100142 000099943 000099427 
001:000099708 000099803 000100431 000099846 000099599 000099774 000099766 000100100 000100702 000100271 
002:000099863 000100021 000099592 000099939 000100040 000099738 000100106 000100089 000100120 000100492 
003:000100341 000099994 000100223 000100237 000100050 000099755 000099785 000100152 000099459 000100004 
004:000099935 000099216 000099586 000099696 000100252 000100366 000100590 000100137 000100333 000099889 
005:000099640 000100052 000100288 000100080 000099966 000100075 000099620 000100072 000100192 000100015 
006:000100019 000100225 000099501 000100050 000099972 000100063 000100024 000099878 000099956 000100312 
007:000100396 000100151 000100195 000099967 000099939 000100242 000099450 000099641 000100155 000099864 
008:000099894 000100102 000100169 000100693 000100039 000100311 000099926 000099880 000099573 000099413 
009:000100139 000100381 000100050 000099669 000100111 000100028 000099833 000099909 000099567 000100313 

000:00.999900 00.999001 00.997582 00.999670 00.995833 00.991724 00.999231 00.998781 00.993624 
001:00.999048 00.992801 00.998618 00.998907 00.999339 00.999419 00.996084 00.990129 00.994385 
002:00.998420 00.997286 00.999240 00.998231 00.998748 00.997573 00.997742 00.997433 00.993741 
003:00.996542 00.998824 00.998964 00.997100 00.994160 00.994459 00.998116 00.991210 00.996641 
004:00.992805 00.996508 00.997608 00.996838 00.995706 00.993488 00.997983 00.996033 00.999540 
005:00.995882 00.993539 00.995604 00.996739 00.995653 00.999799 00.995683 00.994491 00.996251 
006:00.997945 00.994821 00.999690 00.999530 00.999560 00.999950 00.998590 00.999370 00.997079 
007:00.997560 00.997998 00.995727 00.995448 00.998466 00.990577 00.992480 00.997600 00.994701 
008:00.997922 00.997255 00.992065 00.998551 00.995843 00.999680 00.999860 00.996787 00.995185 
009:00.997589 00.999111 00.995307 00.999720 00.998892 00.996944 00.997703 00.994288 00.998265 

00.996432 00.997981 00.997249 00.998701 00.995753 00.999540 00.996703 00.998291 00.999261 

    */
}
