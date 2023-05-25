#include <iostream>

template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

void test()
{ 
    constexpr int N = 9;
    printf("Fibonacci(%d)=%d\n",N, Fibonacci<N>::value);
}

int main() {

    /*
       for (int i = 0; i < n; ++i) {
       std::cout << " " << Fibonacci<i>::value;
       }
       std::cout << std::endl;

       有for循环会报错，因为编译时没法展开
       只能用递归实现
     */

    test();
    //int i = Fibonacci<9>::value;
    return 0;
}
