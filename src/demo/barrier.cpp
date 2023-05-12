#include <stdio.h>
#include <stdint.h>

int x,y,r;
void test_barrier()
{
    x = r;
    y = 1;
    /*
     *g++ -S -O2 barrier.cpp
       发现变成了y=1,x=r，即乱序了
       movl    $1, y(%rip)
       movl    r(%rip), %eax
       movl    %eax, x(%rip)
       */
}

void test_barrier_add_barrier()
{
    x = r;
    //barrier();
    __asm__ __volatile__("" : : : "memory");

    y = 1;
    /*
     * g++ -S -O2 barrier.cpp
      正常：
       movl    r(%rip), %eax
       movl    %eax, x(%rip)
       movl    $1, y(%rip)
     */
}

void test_barrier_add_barrier_last()
{
    x = r;
    y = 1;

    __asm__ __volatile__("" : : : "memory");
    /*
     * g++ -S -O2 barrier.cpp
     * 乱序：
       movl    $1, y(%rip)
       movl    r(%rip), %eax
       movl    %eax, x(%rip)
       */
}

int main()
{
    test_barrier();
    return 0;
}

/*
 *
 *
[root@ecs-KwUzX demo]# g++ -S barrier.cpp

vi barrier.s
截取出test_barrier的汇编：

_Z12test_barrierv:
.LFB0:
.cfi_startproc
pushq   %rbp
.cfi_def_cfa_offset 16
.cfi_offset 6, -16 
movq    %rsp, %rbp
.cfi_def_cfa_register 6


#核心是下面三条语句，并没有乱序
movl    r(%rip), %eax
movl    %eax, x(%rip)
movl    $1, y(%rip)

nop 
popq    %rbp
.cfi_def_cfa 7, 8
ret 
.cfi_endproc

[root@ecs-KwUzX demo]# g++ -S -O2 barrier.cpp
vi barrier.s
截取出test_barrier的汇编：

_Z12test_barrierv:
.LFB12:
.cfi_startproc

#发现变成了y=1,x=r，即乱序了
movl    $1, y(%rip)
movl    r(%rip), %eax
movl    %eax, x(%rip)
ret 
.cfi_endproc


[root@ecs-KwUzX demo]# g++ -S -O2 barrier.cpp 

vi barrier.s
截取出test_barrier, test_barrier_add_barrier的汇编：

_Z12test_barrierv:
.LFB12:
.cfi_startproc
movl    $1, y(%rip)
movl    r(%rip), %eax
movl    %eax, x(%rip)
ret 
.cfi_endproc
.LFE12:
.size   _Z12test_barrierv, .-_Z12test_barrierv
.p2align 4
.globl  _Z24test_barrier_add_barrierv
.type   _Z24test_barrier_add_barrierv, @function
_Z24test_barrier_add_barrierv:
.LFB13:
.cfi_startproc
movl    r(%rip), %eax
movl    %eax, x(%rip)
movl    $1, y(%rip)
ret 
.cfi_endproc
*/
