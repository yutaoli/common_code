#include <iostream>

template <int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

int main() {
    constexpr int result = Fibonacci<6>::value;  // 计算斐波那契数列的第6项
    std::cout << result << std::endl;  // 输出结果: 8
    return 0;
}
/*
 [root@ecs-KwUzX computing_values]# ./fibonacci 
 8

 */
