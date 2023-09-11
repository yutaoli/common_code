extern "C" 
{
#include <string.h>
//#include "mq_errno.h"
}

#include <CppUTest/CommandLineTestRunner.h>
#include "ring_buffer_v2.h"

TEST_GROUP(RingBufferSpScV2)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

struct ST_ORDER_DELAY
{
  int Put(ST_ORDER_DELAY& data)
  {
    nNode = data.nNode;
    nCase = data.nCase;
    nAcct = data.nAcct;
    nTsReq = data.nTsReq;
    nTsAns = data.nTsAns;
    nTsConfirm = data.nTsConfirm;
    nTsMatch = data.nTsMatch;
    nDelayTime = data.nDelayTime;
    nTsReqGtu = data.nTsReqGtu;
    nTsAnsGtu = data.nTsAnsGtu;
    nDelayTimeGtu = data.nDelayTimeGtu;
    nByteReq = data.nByteReq;
    nByteAns = data.nByteAns;
    strOrderId = data.strOrderId;
    strOrderBsn = data.strOrderBsn;
    strMsgId = data.strMsgId;
    strFuncId = data.strFuncId;
    nState = data.nState;
    return 0;
  }
  int Get(ST_ORDER_DELAY& data)
  {
    nNode = data.nNode;
    nCase = data.nCase;
    nAcct = data.nAcct;
    nTsReq = data.nTsReq;
    nTsAns = data.nTsAns;
    nTsConfirm = data.nTsConfirm;
    nTsMatch = data.nTsMatch;
    nDelayTime = data.nDelayTime;
    nTsReqGtu = data.nTsReqGtu;
    nTsAnsGtu = data.nTsAnsGtu;
    nDelayTimeGtu = data.nDelayTimeGtu;
    nByteReq = data.nByteReq;
    nByteAns = data.nByteAns;
    strOrderId = data.strOrderId;
    strOrderBsn = data.strOrderBsn;
    strMsgId = data.strMsgId;
    strFuncId = data.strFuncId;
    nState = data.nState;
    return 0;
  }
  size_t nNode;
  size_t nCase;
  size_t nAcct;
  unsigned long long nTsReq;
  unsigned long long nTsAns;
  unsigned long long nTsConfirm;
  unsigned long long nTsMatch;
  unsigned int nDelayTime;
  unsigned long long nTsReqGtu;
  unsigned long long nTsAnsGtu;
  unsigned int nDelayTimeGtu;
  unsigned long long nByteReq;
  unsigned long long nByteAns;
  std::string strOrderId;
  std::string strOrderBsn;
  std::string strMsgId;
  std::string strFuncId;
  unsigned int nState;
};

TEST(RingBufferSpScV2, Test_sizeOnce_pushTwiceExceed)
{
    //printf("in Test_sizeOnce_pushTwiceExceed\n");
    /*printf("HARE_RB_RECORD_HEADER_LENGTH:%ld,HARE_RB_TRAILER_LENGTH:%ld,ST_ORDER_DELAY:%ld",
    HARE_RB_RECORD_HEADER_LENGTH,HARE_RB_TRAILER_LENGTH,sizeof(ST_ORDER_DELAY));*/
    
    RingBufferSpScV2<ST_ORDER_DELAY> queue;
    int size = ((1<<11) + HARE_RB_TRAILER_LENGTH);// (1<<10)B
    int ret = queue.Init(size);
    CHECK(ret == 0);

    //int pub[2];
    ST_ORDER_DELAY test_struct;
    test_struct.nNode = 1;

    for(int i = 0; i < 20; i++)
    {
        test_struct.nCase = i;
        ret = queue.Push(test_struct);
        if(i >= 13)
        {
            CHECK(ret == ERR_RING_BUFFER_V2_FULL);
        }
        //printf("i:%d,ncase:%lu,ret:%d\n",i,test_struct.nCase,ret);
        //CHECK(ret == 0);
    }

    ST_ORDER_DELAY rsp_struct;
    for(int i = 0; i < 30; i++)
    {
        ret = queue.Pop(rsp_struct);
         if(i >= 13)
        {
            CHECK(ret == ERR_RING_BUFFER_V2_EMPTY);
        }
        //printf("i:%d,ncase:%lu,ret:%d\n",i,rsp_struct.nCase,ret);
    }

    // 还有剩余，然后Reset
    for(int i = 0; i < 20; i++)
    {
        test_struct.nCase = i;
        ret = queue.Push(test_struct);
        //printf("i:%d,ncase:%lu,ret:%d\n",i,test_struct.nCase,ret);
        //CHECK(ret == 0);
    }
     ret = queue.Reset();
     CHECK(ret == 0);
    for(int i = 0; i < 30; i++)
    {
        ret = queue.Pop(rsp_struct);
        //printf("i:%d,ncase:%lu,ret:%d\n",i,rsp_struct.nCase,ret);
    }
}

TEST(RingBufferSpScV2, TestReset)
{
    //printf("in TestReset\n");
    RingBufferSpScV2<ST_ORDER_DELAY> queue;
    int size = ((1<<11) + HARE_RB_TRAILER_LENGTH);// (1<<10)B
    int ret = queue.Init(size);
    CHECK(ret == 0);

    //int pub[2];
    ST_ORDER_DELAY test_struct;
    test_struct.nNode = 1;
    for(int i = 0; i < 5; i++)
    {
        test_struct.nCase = i;
        ret = queue.Push(test_struct);
       // printf("i:%d,ncase:%lu,ret:%d\n",i,test_struct.nCase,ret);
        CHECK(ret == 0);
    }
    // 还有剩余，然后Reset，模拟第二次再加数据，再消费
     ret = queue.Reset();
     CHECK(ret == 0);

    int N = 2;
     for(int i = 0; i < N; i++)
    {
        test_struct.nCase = i;
        ret = queue.Push(test_struct);
        //printf("i:%d,ncase:%lu,ret:%d\n",i,test_struct.nCase,ret);
        CHECK(ret == 0);
    }

     ST_ORDER_DELAY rsp_struct;
    for(int i = 0; i < N + 10; i++)
    {
        ret = queue.Pop(rsp_struct);
        if(i >= N)
        {
            CHECK(ret == ERR_RING_BUFFER_V2_EMPTY);
        }
        //printf("i:%d,ncase:%lu,ret:%d\n",i,rsp_struct.nCase,ret);
    }
/*
    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == ERR_RING_BUFFER_FULL);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);*/
    
}
#if 0
TEST(RingBufferSpScV2, TestEmpty_sizeOnceAnd_popOnce)
{
    ring_buffer queue;
    int size = 1;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int *pub = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pub);
    CHECK(ret == ERR_RING_BUFFER_EMPTY);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(RingBufferSpScV2, Test_size1_pushOnceAnd_popTwice)
{
    ring_buffer queue;
    int size = 1;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[2];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    int *pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);
    CHECK(pubout != NULL);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == ERR_RING_BUFFER_EMPTY);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(RingBufferSpScV2, Test_size2_push2_pop1_push1_pop3)
{
    ring_buffer queue;
    int size = 2;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[3];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == 0);

    int *pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);
    CHECK(pubout != NULL);

    ret = ring_buffer_push(&queue, &pub[2]);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == ERR_RING_BUFFER_EMPTY);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(RingBufferSpScV2, Test_size3_push3_pop1_push1_pop4)
{
    ring_buffer queue;
    int size = 3;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[4];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[2]);
    CHECK(ret == 0);

    int *pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);
    CHECK(pubout != NULL);

    ret = ring_buffer_push(&queue, &pub[3]);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == ERR_RING_BUFFER_EMPTY);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(RingBufferSpScV2, Test_size3_push4)
{
    ring_buffer queue;
    int size = 3;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[4];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[2]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[3]);
    CHECK(ret == ERR_RING_BUFFER_FULL);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(RingBufferSpScV2, Test_size5_push6)
{
    ring_buffer queue;
    int size = 5;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[6];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[2]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[3]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[4]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[5]);
    CHECK(ret == ERR_RING_BUFFER_FULL);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(RingBufferSpScV2, Test_size3_push3_pop1_push1_pop2Count)
{
    ring_buffer queue;
    int size = 3;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[4];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 1);

    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == 0);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 2);

    ret = ring_buffer_push(&queue, &pub[2]);
    CHECK(ret == 0);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 3);

    int *pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);
    CHECK(pubout != NULL);
    CHECK(pubout == &pub[0]);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 2);

    ret = ring_buffer_push(&queue, &pub[3]);
    CHECK(ret == 0);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 3);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);
    CHECK(pubout == &pub[1]);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 2);

    pubout = NULL;
    ret = ring_buffer_pop(&queue, (void **)&pubout);
    CHECK(ret == 0);
    CHECK(pubout == &pub[2]);

    ret = ring_buffer_size(&queue);
    CHECK( ret == 1);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
#endif