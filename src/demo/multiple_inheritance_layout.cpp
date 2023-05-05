#include <iostream>

using namespace std;
class Base1 
{
    public:
        virtual void f() { cout << "Base1::f" << endl; }
        virtual void g() { cout << "Base1::g" << endl; }
        virtual void h() { cout << "Base1::h" << endl; }
        int a;
};

class Base2 
{
    public:
        virtual void f() { cout << "Base2::f" << endl; }
        virtual void g() { cout << "Base2::g" << endl; }
        virtual void h() { cout << "Base2::h" << endl; }
        int a;
};

class Base3 
{
    public:
        virtual void f() { cout << "Base3::f" << endl; }
        virtual void g() { cout << "Base3::g" << endl; }
        virtual void h() { cout << "Base3::h" << endl; }
        int a;
};

class Derive : public Base1, public Base2, public Base3 {
    public:
        virtual void f() { cout << "Derive::f" << endl; }
        virtual void g1() { cout << "Derive::g1" << endl; }
        int a;
};

typedef void(*Fun)(void);
int main()
{
    Derive d;

    Fun pVtab = (Fun)*(int64_t *)*(int64_t *)&d;
    printf("sizeof(Fun)=%d,sizeof(int)=%d,sizeof(int64_t*)=%d\n",
            sizeof(Fun), sizeof(int), sizeof(int64_t *));

    //Base1's vtable
    int i = 0;
    int n = 4;
    // linux pVtab最后不为 NULL，所以要有n
    while(n--)
    {
        //printf("pVtab:%0x\n",pVtab);

        pVtab();

        i++;
        pVtab = (Fun)*(((int64_t *)*(int64_t *)&d)+i);
    }

    //Base2's vtable
    int offset = 2;
    pVtab = (Fun)*(int64_t *)*((int64_t *)&d + offset);
    i = 0;
    n = 3;
    while(n--)
    {
        //printf("pVtab:%0x\n",pVtab);

        pVtab();

        i++;
        pVtab = (Fun)*((int64_t *)*((int64_t *)&d + offset)+i);
    }



    //Base3's vtable
    offset = 4;
    pVtab = (Fun)*(int64_t *)*((int64_t *)&d + offset);
    i = 0;
    n = 3;
    while(n--)
    {
        //printf("pVtab:%0x\n",pVtab);

        pVtab();

        i++;
        pVtab = (Fun)*((int64_t *)*((int64_t *)&d + offset)+i);
    }

    return 0;
}
/*
 输出：
 sizeof(Fun)=8,sizeof(int)=4,sizeof(int64_t*)=8
 Derive::f
 Base1::g
 Base1::h
 Derive::g1
 Derive::f
 Base2::g
 Base2::h
 Derive::f
 Base3::g
 Base3::h

 */
