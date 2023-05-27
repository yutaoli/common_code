#include <stdio.h>

// 基类模板
template <typename Derived>
class Base {
public:
    void foo() {
        static_cast<Derived*>(this)->fooImpl();
    }
};

// 派生类
class Derived : public Base<Derived> {
public:
    void fooImpl() {
        printf("%s\n",__FUNCTION__);
        // 实现派生类特定的行为
        // ...
    }
};

int main() {
    Derived d;
    Derived d2;
    d.foo();  // 调用派生类的 fooImpl 函数
    d2.foo();  // 调用派生类的 fooImpl 函数
    return 0;
}
