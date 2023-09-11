#include <CppUTest/CommandLineTestRunner.h>
#include "shm.h"

#pragma pack(push)
#pragma pack(4)
typedef struct Stat
{
	int case_count;
}Stat;
#pragma pack(pop)

#define kTestShmName "test_stat"
TEST_GROUP(Shm)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(Shm, TestWrite31Read)
{
Shm<Stat> write_stat;
int ret = write_stat.ShmWInit(kTestShmName);
CHECK(ret == 0);
Stat * write_ptr = write_stat.GetPtrW();
CHECK(write_ptr != NULL);
write_ptr->case_count = 31;

Shm<Stat> read_stat;
ret = read_stat.ShmRInit(kTestShmName);
CHECK(ret == 0);
const Stat * read_ptr = read_stat.GetPtrR();
CHECK(read_ptr != NULL);
CHECK(read_ptr->case_count == 31);

ret = write_stat.ShmFini(kTestShmName);
CHECK(ret == 0);
}