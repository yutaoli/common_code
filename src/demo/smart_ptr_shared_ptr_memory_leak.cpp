#include <memory>
#include <stdio.h>

struct Node {
    std::shared_ptr<Node> next;
    int value;
    
    virtual ~ Node()
    {
        printf("%s\n", __FUNCTION__);
    }
};

int test_shared_ptr_memory_leak() 
{
    std::shared_ptr<Node> node1(new Node{});
    std::shared_ptr<Node> node2(new Node{});
    node1->next = node2;
    node2->next = node1;
    return 0;
}

int main()
{
    test_shared_ptr_memory_leak();
    return 0;
}
/*
 [root@ecs-KwUzX demo]# make smart_ptr_shared_ptr_memory_leak 
 Make smart_ptr_shared_ptr_memory_leak begin......g++  smart_ptr_shared_ptr_memory_leak.cpp -o smart_ptr_shared_ptr_memory_leak  -g -Xlinker "-("  -Xlinker "-)" 
 ok.
 [root@ecs-KwUzX demo]# ./smart_ptr_shared_ptr_memory_leak 
 // 没有输出任何数据，即没有调用析构函数，new出来的Node()的内存，没有释放

 // 检查内存泄露情况：泄露了96B内存
 [root@ecs-KwUzX demo]# valgrind --leak-check=full --log-file=valgrind_smart_ptr_shared_ptr_memory_leak.log -s ./smart_ptr_shared_ptr_memory_leak

 [root@ecs-KwUzX demo]# cat  valgrind_smart_ptr_shared_ptr_memory_leak.log 
 ==16428== Memcheck, a memory error detector
 ==16428== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 ==16428== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
 ==16428== Command: ./smart_ptr_shared_ptr_memory_leak
 ==16428== Parent PID: 10418
 ==16428== 
 ==16428== 
 ==16428== HEAP SUMMARY:
 ==16428==     in use at exit: 96 bytes in 4 blocks
 ==16428==   total heap usage: 4 allocs, 0 frees, 96 bytes allocated
 ==16428== 
 ==16428== 96 (24 direct, 72 indirect) bytes in 1 blocks are definitely lost in loss record 4 of 4
 ==16428==    at 0x4C2A593: operator new(unsigned long) (vg_replace_malloc.c:344)
 ==16428==    by 0x4011D1: test_shared_ptr_memory_leak() (smart_ptr_shared_ptr_memory_leak.cpp:10)
 ==16428==    by 0x4012C1: main (smart_ptr_shared_ptr_memory_leak.cpp:19)
 ==16428== 
 ==16428== LEAK SUMMARY:
 ==16428==    definitely lost: 24 bytes in 1 blocks
 ==16428==    indirectly lost: 72 bytes in 3 blocks
 ==16428==      possibly lost: 0 bytes in 0 blocks
 ==16428==    still reachable: 0 bytes in 0 blocks
 ==16428==         suppressed: 0 bytes in 0 blocks
 ==16428== 
 ==16428== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

 */
