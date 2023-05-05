int printf( const char *format, ...);

int global_init_var = 84;
int global_uninit_var;

void func1(int i)
{
    printf("%d\n", i);
}

int main()
{
    static int static_var = 85;
    static int static_var2;

    int a = 1;
    int b;

    func1(static_var + static_var2 + a + b);
    return a;
}
/*
   gcc -c simple_section.cpp // 只编译不链接


   [root@ecs-KwUzX demo]# objdump -h simple_section.o 

   simple_section.o：     文件格式 elf64-x86-64

   节：
   Idx Name          Size      VMA               LMA               File off  Algn
   0 .text         00000055  0000000000000000  0000000000000000  00000040  2**0
   CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
   1 .data         00000008  0000000000000000  0000000000000000  00000098  2**2
   CONTENTS, ALLOC, LOAD, DATA
   2 .bss          00000008  0000000000000000  0000000000000000  000000a0  2**2
   ALLOC
   3 .rodata       00000004  0000000000000000  0000000000000000  000000a0  2**0
   CONTENTS, ALLOC, LOAD, READONLY, DATA
   4 .comment      00000030  0000000000000000  0000000000000000  000000a4  2**0
   CONTENTS, READONLY
   5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000d4  2**0
   CONTENTS, READONLY
   6 .eh_frame     00000058  0000000000000000  0000000000000000  000000d8  2**3
   CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA



   [root@ecs-KwUzX demo]# objdump  -s -d simple_section.o 
   // -s:将所有段的内容以十六进制的方式打印出来
   // -d:反汇编

   simple_section.o：     文件格式 elf64-x86-64

   Contents of section .text:
   0000 554889e5 4883ec10 897dfc8b 45fc89c6  UH..H....}..E...
   0010 bf000000 00b80000 0000e800 00000090  ................
   0020 c9c35548 89e54883 ec10c745 fc010000  ..UH..H....E....
   0030 008b1500 0000008b 05000000 0001c28b  ................
   0040 45fc01c2 8b45f801 d089c7e8 00000000  E....E..........
   0050 8b45fcc9 c3                          .E...           
   Contents of section .data:
   0000 54000000 55000000                    T...U...        
   Contents of section .rodata:
   0000 25640a00                             %d..            
   Contents of section .comment:
   0000 00474343 3a202847 4e552920 31302e32  .GCC: (GNU) 10.2
   0010 2e312032 30323130 31333020 28526564  .1 20210130 (Red
   0020 20486174 2031302e 322e312d 31312900   Hat 10.2.1-11).
   Contents of section .eh_frame:
   0000 14000000 00000000 017a5200 01781001  .........zR..x..
   0010 1b0c0708 90010000 1c000000 1c000000  ................
   0020 00000000 22000000 00410e10 8602430d  ...."....A....C.
   0030 065d0c07 08000000 1c000000 3c000000  .]..........<...
   0040 00000000 33000000 00410e10 8602430d  ....3....A....C.
   0050 066e0c07 08000000                    .n......        

   Disassembly of section .text:

   0000000000000000 <_Z5func1i>:
	0: 55                      push   %rbp
	1:  48 89 e5                mov    %rsp,%rbp
	4:   48 83 ec 10             sub    $0x10,%rsp
	8:    89 7d fc                mov    %edi,-0x4(%rbp)
	b:   8b 45 fc                mov    -0x4(%rbp),%eax
	e:    89 c6                   mov    %eax,%esi
	10: bf 00 00 00 00          mov    $0x0,%edi
	15:   b8 00 00 00 00          mov    $0x0,%eax
	1a: e8 00 00 00 00          callq  1f <_Z5func1i+0x1f>
	1f:   90                      nop
	20: c9                      leaveq 
	21:   c3                      retq   

0000000000000022 <main>:
	22: 55                      push   %rbp
	23: 48 89 e5                mov    %rsp,%rbp
	26: 48 83 ec 10             sub    $0x10,%rsp
	2a:   c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%rbp)
    31:   8b 15 00 00 00 00       mov    0x0(%rip),%edx        # 37 <main+0x15>
    37: 8b 05 00 00 00 00       mov    0x0(%rip),%eax        # 3d <main+0x1b>
    3d:   01 c2                   add    %eax,%edx
    3f: 8b 45 fc                mov    -0x4(%rbp),%eax
    42:   01 c2                   add    %eax,%edx
    44: 8b 45 f8                mov    -0x8(%rbp),%eax
    47:   01 d0                   add    %edx,%eax
    49: 89 c7                   mov    %eax,%edi
    4b:   e8 00 00 00 00          callq  50 <main+0x2e>
    50: 8b 45 fc                mov    -0x4(%rbp),%eax
    53:   c9                      leaveq 
    54: c3                      retq   


[root@ecs-KwUzX demo]# objdump -x -s -d simple_section.o
// -x:打印所有头部信息，包括符号表和重定向入口
// -s:将所有段的内容以十六进制的方式打印出来
// -d:反汇编

simple_section.o：     文件格式 elf64-x86-64
simple_section.o
体系结构：i386:x86-64，标志 0x00000011：
HAS_RELOC, HAS_SYMS
起始地址 0x0000000000000000

节：
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000055  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000008  0000000000000000  0000000000000000  00000098  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000008  0000000000000000  0000000000000000  000000a0  2**2
                  ALLOC
  3 .rodata       00000004  0000000000000000  0000000000000000  000000a0  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .comment      00000030  0000000000000000  0000000000000000  000000a4  2**0
                  CONTENTS, READONLY
  5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000d4  2**0
                  CONTENTS, READONLY
  6 .eh_frame     00000058  0000000000000000  0000000000000000  000000d8  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
SYMBOL TABLE:
0000000000000000 l    df *ABS*	0000000000000000 simple_section.cpp
0000000000000000 l    d  .text	0000000000000000 .text
0000000000000000 l    d  .data	0000000000000000 .data
0000000000000000 l    d  .bss	0000000000000000 .bss
0000000000000000 l    d  .rodata	0000000000000000 .rodata
0000000000000004 l     O .data	0000000000000004 _ZZ4mainE10static_var
0000000000000004 l     O .bss	0000000000000004 _ZZ4mainE11static_var2
0000000000000000 l    d  .note.GNU-stack	0000000000000000 .note.GNU-stack
0000000000000000 l    d  .eh_frame	0000000000000000 .eh_frame
0000000000000000 l    d  .comment	0000000000000000 .comment
0000000000000000 g     O .data	0000000000000004 global_init_var
0000000000000000 g     O .bss	0000000000000004 global_uninit_var
0000000000000000 g     F .text	0000000000000022 _Z5func1i
0000000000000000         *UND*	0000000000000000 _Z6printfPKcz
0000000000000022 g     F .text	0000000000000033 main


Contents of section .text:
 0000 554889e5 4883ec10 897dfc8b 45fc89c6  UH..H....}..E...
 0010 bf000000 00b80000 0000e800 00000090  ................
 0020 c9c35548 89e54883 ec10c745 fc010000  ..UH..H....E....
 0030 008b1500 0000008b 05000000 0001c28b  ................
 0040 45fc01c2 8b45f801 d089c7e8 00000000  E....E..........
 0050 8b45fcc9 c3                          .E...           
Contents of section .data:
 0000 54000000 55000000                    T...U... （54000000即84，即global_init_var；55000000即85，即static_var）       
Contents of section .rodata:
 0000 25640a00                             %d..            
Contents of section .comment:
 0000 00474343 3a202847 4e552920 31302e32  .GCC: (GNU) 10.2
 0010 2e312032 30323130 31333020 28526564  .1 20210130 (Red
 0020 20486174 2031302e 322e312d 31312900   Hat 10.2.1-11).
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 22000000 00410e10 8602430d  ...."....A....C.
 0030 065d0c07 08000000 1c000000 3c000000  .]..........<...
 0040 00000000 33000000 00410e10 8602430d  ....3....A....C.
 0050 066e0c07 08000000                    .n......        

Disassembly of section .text:

0000000000000000 <_Z5func1i>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 10          	sub    $0x10,%rsp
   8:	89 7d fc             	mov    %edi,-0x4(%rbp)
   b:	8b 45 fc             	mov    -0x4(%rbp),%eax
   e:	89 c6                	mov    %eax,%esi
  10:	bf 00 00 00 00       	mov    $0x0,%edi
			11: R_X86_64_32	.rodata
  15:	b8 00 00 00 00       	mov    $0x0,%eax
  1a:	e8 00 00 00 00       	callq  1f <_Z5func1i+0x1f>
			1b: R_X86_64_PLT32	_Z6printfPKcz-0x4
  1f:	90                   	nop
  20:	c9                   	leaveq 
  21:	c3                   	retq   

0000000000000022 <main>:
  22:	55                   	push   %rbp
  23:	48 89 e5             	mov    %rsp,%rbp
  26:	48 83 ec 10          	sub    $0x10,%rsp
  2a:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%rbp)
  31:	8b 15 00 00 00 00    	mov    0x0(%rip),%edx        # 37 <main+0x15>
			33: R_X86_64_PC32	.data
  37:	8b 05 00 00 00 00    	mov    0x0(%rip),%eax        # 3d <main+0x1b>
			39: R_X86_64_PC32	.bss
  3d:	01 c2                	add    %eax,%edx
  3f:	8b 45 fc             	mov    -0x4(%rbp),%eax
  42:	01 c2                	add    %eax,%edx
  44:	8b 45 f8             	mov    -0x8(%rbp),%eax
  47:	01 d0                	add    %edx,%eax
  49:	89 c7                	mov    %eax,%edi
  4b:	e8 00 00 00 00       	callq  50 <main+0x2e>
			4c: R_X86_64_PLT32	_Z5func1i-0x4
  50:	8b 45 fc             	mov    -0x4(%rbp),%eax
  53:	c9                   	leaveq 
  54:	c3                   	retq   

    */
