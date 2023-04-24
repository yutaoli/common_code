extern "C" 
{
#include <string.h>

#include "ring_buffer.h"
//#include "mq_errno.h"
}

#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(ring_buffer)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(ring_buffer, Test_sizeOnce_pushTwiceExceed)
{
    ring_buffer queue;
    int size = 1;
    int ret = ring_buffer_init(&queue, size, 1, 1);
    CHECK(ret == 0);

    int pub[2];
    ret = ring_buffer_push(&queue, &pub[0]);
    CHECK(ret == 0);

    ret = ring_buffer_push(&queue, &pub[1]);
    CHECK(ret == ERR_RING_BUFFER_FULL);

    ret = ring_buffer_fini(&queue);
    CHECK(ret == 0);
}
TEST(ring_buffer, TestEmpty_sizeOnceAnd_popOnce)
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
TEST(ring_buffer, Test_size1_pushOnceAnd_popTwice)
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
TEST(ring_buffer, Test_size2_push2_pop1_push1_pop3)
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
TEST(ring_buffer, Test_size3_push3_pop1_push1_pop4)
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
TEST(ring_buffer, Test_size3_push4)
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
TEST(ring_buffer, Test_size5_push6)
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
TEST(ring_buffer, Test_size3_push3_pop1_push1_pop2Count)
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
