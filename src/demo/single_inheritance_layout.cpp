#include <iostream>
#include <stdio.h>

using namespace std;

class Base1 
{
    public:
        virtual void f() { cout << "Base1::f" << endl; }
        virtual void g() { cout << "Base1::g" << endl; }
        virtual void h() { cout << "Base1::h" << endl; }
};

class Derive : public Base1
{
    public:
        virtual void f() { cout << "Derive::f" << endl; }
        virtual void g1() { cout << "Derive::g1" << endl; }
        virtual void h1() { cout << "Derive::h1" << endl; }
};

typedef void(*Fun)(void);

int main()
{
    Derive d;
    Fun pVtab = (Fun)*(int64_t *)*(int64_t *)&d;
    //Base1's vtable

    //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+0);

    printf("sizeof(Fun):%d\n",sizeof(Fun));

    int i = 0;
    int n = 5;
    
    // linux pVtab最后不为 NULL，所以要有n
    while(n--)
    {
        //printf("pVtab:%0x\n",pVtab);

        pVtab();

        i++;
        pVtab = (Fun)*(((int64_t *)*(int64_t *)&d)+i);
    }

    return 0;
}
/*
 // 输出
 sizeof(Fun):8
 Derive::f
 Base1::g
 Base1::h
 Derive::g1
 Derive::h1
 */
