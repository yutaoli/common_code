#include <stdio.h>
#include <string.h>

int add(int a,int b)
{
    int buf[128];
    buf[0]=0;
    buf[10]=10;
    buf[11]=11;
    buf[127]=127;

    int buf2[16];
    buf2[0]=0;
    buf2[15]=15;

    int c = a + b;

    return c;
}

int main(int argc, char **argv)
{
    int a = 1, b= 2;
    int c = add(a,b);
    printf("%d\n",c);

    return 0;
}

/*
[root@ecs-KwUzX ~]# gcc -S a.cpp -o a.s
把无关的代码干掉了，如下；

_Z3addii:
.LFB0:
        .cfi_startproc
        pushq   %rbp      ; 保存上一个栈帧的rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp      ; rbp=rsp
        .cfi_def_cfa_register 6
        subq    $480, %rsp      ; rsp往下走，拉出一个栈帧
        movl    %edi, -596(%rbp)
        movl    %esi, -600(%rbp)
        movl    $0, -528(%rbp)
        movl    $10, -488(%rbp)
        movl    $11, -484(%rbp)
        movl    $127, -20(%rbp)

        movl    $0, -592(%rbp)
        movl    $15, -532(%rbp)
        movl    -600(%rbp), %eax
        movl    -596(%rbp), %edx
        addl    %edx, %eax
        movl    %eax, -4(%rbp)
        movl    -4(%rbp), %eax      ; 把call的返回值，放到eax中
        leave      ; esp=ebp；ebp=pop（）；把该栈帧销毁，返回到上一个栈帧
        .cfi_def_cfa 7, 8
        ret        ; eip=pop（）；和call刚好反过来
        .cfi_endproc

main:
.LFB1:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp      ;即rbp=rsp
        .cfi_def_cfa_register 6
        subq    $32, %rsp
        movl    %edi, -20(%rbp)
        movq    %rsi, -32(%rbp)
        movl    $1, -4(%rbp)
        movl    $2, -8(%rbp)
        movl    -8(%rbp), %edx
        movl    -4(%rbp), %eax
        movl    %edx, %esi
        movl    %eax, %edi
        call    _Z3addii      ; call 地址x：push（eip），eip=地址x；把eip入栈；
        movl    %eax, -12(%rbp)
        movl    -12(%rbp), %eax
        movl    %eax, %esi
        movl    $.LC0, %edi
        movl    $0, %eax
        call    printf
        movl    $0, %eax
        leave
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc

*/
