#include <memory>
#include <stdio.h>

struct Node {
    std::weak_ptr<Node> next;
    int value;

    virtual ~ Node()
    {
        printf("%s\n", __FUNCTION__);
    }
};

int test_weak_ptr_memory_leak() 
{
    std::shared_ptr<Node> node1(new Node{});
    std::shared_ptr<Node> node2(new Node{});
    node1->next = node2;
    node2->next = node1;
    return 0;
}

int main()
{
    test_weak_ptr_memory_leak();
    return 0;
}
/*
 [root@ecs-KwUzX demo]# make smart_ptr_weak_ptr 
 Make smart_ptr_weak_ptr begin......g++  smart_ptr_weak_ptr.cpp -o smart_ptr_weak_ptr  -g -Xlinker "-("  -Xlinker "-)" 
 ok.
 [root@ecs-KwUzX demo]# ./smart_ptr_weak_ptr // 调用析构函数，new出来的Node()，被释放
 ~Node
 ~Node

 // 检查内存泄露情况：没有泄露内存
 [root@ecs-KwUzX demo]# valgrind --leak-check=full --log-file=valgrind_smart_ptr_weak_ptr.log ./smart_ptr_weak_ptr

 [root@ecs-KwUzX demo]# cat valgrind_smart_ptr_weak_ptr.log 
 ==16133== Memcheck, a memory error detector
 ==16133== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
 ==16133== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
 ==16133== Command: ./smart_ptr_weak_ptr
 ==16133== Parent PID: 10418
 ==16133== 
 ==16133== 
 ==16133== HEAP SUMMARY:
 ==16133==     in use at exit: 0 bytes in 0 blocks
 ==16133==   total heap usage: 4 allocs, 4 frees, 96 bytes allocated
 ==16133== 
 ==16133== All heap blocks were freed -- no leaks are possible
 ==16133== 
 ==16133== For lists of detected and suppressed errors, rerun with: -s
 ==16133== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */
