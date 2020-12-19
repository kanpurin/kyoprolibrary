#ifndef _COMPRESSION_HPP_
#define _COMPRESSION_HPP_

#include <vector>
#include <map>

// 数列の座標圧縮 要素の最大値を返す
template<typename T>
void compression(std::vector<T> &v) {
    std::map<T,T> mp;
    for (auto &val : v) mp[val] = 0;
    int now = 0;
    for(auto [k,_] : mp) mp[k] = now++;
    for (auto &val : v) val = mp[val];
}

#endif