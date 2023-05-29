// 模板实现文件：mytemplate.cpp

#include "mytemplate.h"
#include <iostream>

    template <typename T>
void MyTemplateFunction(T value)
{
    std::cout << "Template value: " << value << std::endl;
}

// 显式实例化模板函数，以便在需要的地方进行编译
template void MyTemplateFunction<int>(int);
template void MyTemplateFunction<double>(double);
/*
 产生编译结果如下：
   [root@ecs-KwUzX separate_compilation]# objdump -S mytemplate 

   mytemplate：     文件格式 elf64-x86-64


   Disassembly of section .text:

   0000000000000000 <_Z41__static_initialization_and_destruction_0ii>:
0:   55                      push   %rbp
1:    48 89 e5                mov    %rsp,%rbp
4: 48 83 ec 10             sub    $0x10,%rsp
8:  89 7d fc                mov    %edi,-0x4(%rbp)
b:   89 75 f8                mov    %esi,-0x8(%rbp)
e:    83 7d fc 01             cmpl   $0x1,-0x4(%rbp)
12: 75 27                   jne    3b <_Z41__static_initialization_and_destruction_0ii+0x3b>
14:   81 7d f8 ff ff 00 00    cmpl   $0xffff,-0x8(%rbp)
1b: 75 1e                   jne    3b <_Z41__static_initialization_and_destruction_0ii+0x3b>
1d:   bf 00 00 00 00          mov    $0x0,%edi
22: e8 00 00 00 00          callq  27 <_Z41__static_initialization_and_destruction_0ii+0x27>
27:   ba 00 00 00 00          mov    $0x0,%edx
2c: be 00 00 00 00          mov    $0x0,%esi
31:   bf 00 00 00 00          mov    $0x0,%edi
36: e8 00 00 00 00          callq  3b <_Z41__static_initialization_and_destruction_0ii+0x3b>
3b:   90                      nop
3c: c9                      leaveq 
3d:   c3                      retq   

000000000000003e <_GLOBAL__sub_I_mytemplate.cpp>:
3e: 55                      push   %rbp
3f:   48 89 e5                mov    %rsp,%rbp
42: be ff ff 00 00          mov    $0xffff,%esi
47:   bf 01 00 00 00          mov    $0x1,%edi
4c: e8 af ff ff ff          callq  0 <_Z41__static_initialization_and_destruction_0ii>
51:   5d                      pop    %rbp
52: c3                      retq   

Disassembly of section .text._Z18MyTemplateFunctionIiEvT_:

0000000000000000 <_Z18MyTemplateFunctionIiEvT_>:
0:   55                      push   %rbp
1:    48 89 e5                mov    %rsp,%rbp
4: 48 83 ec 10             sub    $0x10,%rsp
8:  89 7d fc                mov    %edi,-0x4(%rbp)
b:   be 00 00 00 00          mov    $0x0,%esi
10:    bf 00 00 00 00          mov    $0x0,%edi
15:  e8 00 00 00 00          callq  1a <_Z18MyTemplateFunctionIiEvT_+0x1a>
1a:    48 89 c2                mov    %rax,%rdx
1d:  8b 45 fc                mov    -0x4(%rbp),%eax
20:    89 c6                   mov    %eax,%esi
22:  48 89 d7                mov    %rdx,%rdi
25:    e8 00 00 00 00          callq  2a <_Z18MyTemplateFunctionIiEvT_+0x2a>
2a:  be 00 00 00 00          mov    $0x0,%esi
2f:    48 89 c7                mov    %rax,%rdi
32:  e8 00 00 00 00          callq  37 <_Z18MyTemplateFunctionIiEvT_+0x37>
37:    90                      nop
38:  c9                      leaveq 
39:    c3                      retq   

Disassembly of section .text._Z18MyTemplateFunctionIdEvT_:

0000000000000000 <_Z18MyTemplateFunctionIdEvT_>:
0:  55                      push   %rbp
1:   48 89 e5                mov    %rsp,%rbp
4:    48 83 ec 10             sub    $0x10,%rsp
8: f2 0f 11 45 f8          movsd  %xmm0,-0x8(%rbp)
d:  be 00 00 00 00          mov    $0x0,%esi
12:   bf 00 00 00 00          mov    $0x0,%edi
17: e8 00 00 00 00          callq  1c <_Z18MyTemplateFunctionIdEvT_+0x1c>
1c:   48 89 c2                mov    %rax,%rdx
1f: 48 8b 45 f8             mov    -0x8(%rbp),%rax
23:   66 48 0f 6e c0          movq   %rax,%xmm0
28: 48 89 d7                mov    %rdx,%rdi
2b:   e8 00 00 00 00          callq  30 <_Z18MyTemplateFunctionIdEvT_+0x30>
30: be 00 00 00 00          mov    $0x0,%esi
35:   48 89 c7                mov    %rax,%rdi
38: e8 00 00 00 00          callq  3d <_Z18MyTemplateFunctionIdEvT_+0x3d>
3d:   90                      nop
3e: c9                      leaveq 
3f:   c3                      retq  
*/
