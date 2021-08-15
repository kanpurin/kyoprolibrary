#ifndef _FLOOR_RANGE_HPP_
#define _FLOOR_RANGE_HPP_

#include <vector>

// x=floor(n/i)となる1≦i≦nが存在するxについて，x=floor(n/i)となるiの範囲を返す
// O(√n)
template<typename T>
std::vector<std::pair<T,T>> floor_range(T n) {
    std::vector<std::pair<T,T>> res;
    for (T i = 1, la; i <= n; i = la+1) {
        la = n / (n / i);
        res.push_back({i,la});
    }
    return res;
}

#endif
