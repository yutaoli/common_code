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