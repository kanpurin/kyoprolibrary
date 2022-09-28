#ifndef _BABY_STEP_GIANT_STEP_HPP_
#define _BABY_STEP_GIANT_STEP_HPP_

#include <functional>
#include <map>

// Baby-step Giant-step
// f^{t}(s)=g となる最小の非負整数t 位数n 逆関数f^{-1}が存在する
// fm(x):=f^{m}(x)
template<typename T, typename KeyType>
long long bsgs(T s, 
               T g, 
               long long n, 
               long long m, 
               const std::function<T(T)> &f, 
               const std::function<T(T)> &fm, 
               const std::function<KeyType(T)> &mk_key) {
    std::map<KeyType,int> mp;
    for (int j = 1; j <= m; j++) {
        g = f(g);
        KeyType key = mk_key(g);
        mp[key] = -j;
    }
    for (int i = 1; ; i++) {
        s = fm(s);
        KeyType key = mk_key(s);
        if (mp.count(key)) return m*i+mp[key];
        if (n<=m*i) break;
    }
    return -1;
}

#endif
