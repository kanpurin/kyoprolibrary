#ifndef _ENUM_DIV_H_
#define _ENUM_DIV_H_

#include <vector>

// enum_div 約数列挙O(√N)
// 順番はバラバラ
template<typename T>
std::vector<T> enum_div(T n) {
    std::vector<T> ret;
    for (T i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            ret.push_back(i);
            if (i * i != n)
            {
                ret.push_back(n / i);
            }
        }
    }
    return ret;
}

#endif
