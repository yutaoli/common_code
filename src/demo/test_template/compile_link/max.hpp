#ifndef MAX_HPP
#define MAX_HPP

template<typename T>
T max(T a, T b)
{
    if(a < b) return b;
    return a;
}

#endif
