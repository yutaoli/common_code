#include "max.hpp"
#include <stdio.h>

void test_max();

int main()
{
    int a = max(1, 2);
    test_max();
    printf("%s,a:%d\n",__FUNCTION__,a);
    return 0;
}

/*
1、对 test_max.cpp编译，生成汇编代码：
[root@ecs-KwUzX test]# g++ test_max.cpp -c -o test_max
[root@ecs-KwUzX test]# objdump -S test_max

test_max：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <_Z8test_maxv>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 10          	sub    $0x10,%rsp
   8:	be 04 00 00 00       	mov    $0x4,%esi
   d:	bf 03 00 00 00       	mov    $0x3,%edi
  12:	e8 00 00 00 00       	callq  17 <_Z8test_maxv+0x17>
  17:	89 45 fc             	mov    %eax,-0x4(%rbp)
  1a:	8b 45 fc             	mov    -0x4(%rbp),%eax
  1d:	89 c2                	mov    %eax,%edx
  1f:	be 00 00 00 00       	mov    $0x0,%esi
  24:	bf 00 00 00 00       	mov    $0x0,%edi
  29:	b8 00 00 00 00       	mov    $0x0,%eax
  2e:	e8 00 00 00 00       	callq  33 <_Z8test_maxv+0x33>
  33:	90                   	nop
  34:	c9                   	leaveq 
  35:	c3                   	retq   

Disassembly of section .text._Z3maxIiET_S0_S0_:

0000000000000000 <_Z3maxIiET_S0_S0_>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	89 7d fc             	mov    %edi,-0x4(%rbp)
   7:	89 75 f8             	mov    %esi,-0x8(%rbp)
   a:	8b 45 fc             	mov    -0x4(%rbp),%eax
   d:	3b 45 f8             	cmp    -0x8(%rbp),%eax
  10:	7d 05                	jge    17 <_Z3maxIiET_S0_S0_+0x17>
  12:	8b 45 f8             	mov    -0x8(%rbp),%eax
  15:	eb 03                	jmp    1a <_Z3maxIiET_S0_S0_+0x1a>
  17:	8b 45 fc             	mov    -0x4(%rbp),%eax
  1a:	5d                   	pop    %rbp
  1b:	c3                   	retq  
  
可以看到产生了max代码

2、对test.cpp编译，生成汇编代码：
[root@ecs-KwUzX test]# g++ test.cpp -c -o test
[root@ecs-KwUzX test]# objdump -S test

test：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 10          	sub    $0x10,%rsp
   8:	be 02 00 00 00       	mov    $0x2,%esi
   d:	bf 01 00 00 00       	mov    $0x1,%edi
  12:	e8 00 00 00 00       	callq  17 <main+0x17>
  17:	89 45 fc             	mov    %eax,-0x4(%rbp)
  1a:	e8 00 00 00 00       	callq  1f <main+0x1f>
  1f:	8b 45 fc             	mov    -0x4(%rbp),%eax
  22:	89 c2                	mov    %eax,%edx
  24:	be 00 00 00 00       	mov    $0x0,%esi
  29:	bf 00 00 00 00       	mov    $0x0,%edi
  2e:	b8 00 00 00 00       	mov    $0x0,%eax
  33:	e8 00 00 00 00       	callq  38 <main+0x38>
  38:	b8 00 00 00 00       	mov    $0x0,%eax
  3d:	c9                   	leaveq 
  3e:	c3                   	retq   

Disassembly of section .text._Z3maxIiET_S0_S0_:

0000000000000000 <_Z3maxIiET_S0_S0_>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	89 7d fc             	mov    %edi,-0x4(%rbp)
   7:	89 75 f8             	mov    %esi,-0x8(%rbp)
   a:	8b 45 fc             	mov    -0x4(%rbp),%eax
   d:	3b 45 f8             	cmp    -0x8(%rbp),%eax
  10:	7d 05                	jge    17 <_Z3maxIiET_S0_S0_+0x17>
  12:	8b 45 f8             	mov    -0x8(%rbp),%eax
  15:	eb 03                	jmp    1a <_Z3maxIiET_S0_S0_+0x1a>
  17:	8b 45 fc             	mov    -0x4(%rbp),%eax
  1a:	5d                   	pop    %rbp
  1b:	c3                   	retq   

可以看到产生了max代码

3、链接，生成可执行文件：
[root@ecs-KwUzX test]# g++ test test_max -o test.out
[root@ecs-KwUzX test]# ./test.out 
test_max,a:4
main,a:2

可以看到，并不会产生链接错误，实际上的max代码进行了去重
*/
