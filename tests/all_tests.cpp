#include <unistd.h>
#include <CppUTest/CommandLineTestRunner.h>

extern "C"
{
//#include "mq_log.h"
}

int main(int argc, char **argv)
{
//    MqLogInit(argv[0]);
    return RUN_ALL_TESTS(argc, argv);
}
