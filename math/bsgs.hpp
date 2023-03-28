#ifndef _BABY_STEP_GIANT_STEP_HPP_
#define _BABY_STEP_GIANT_STEP_HPP_

#include <functional>
#include <unordered_set>

// Baby-step Giant-step
// https://maspypy.com/モノイド作用に関する離散対数問題
// f^{t}(s)=g となる最小の非負整数t 位数n
// fm(x):=f^{m}(x)
template<typename T>
long long bsgs(T s, T g, 
               long long n, long long m, 
               const std::function<T(T)> &f,
               const std::function<T(T)> &fm) {
    std::unordered_set<T> st;
    T _g = g;
    for (int j = 1; j <= m; j++) {
        _g = f(_g);
        st.insert(_g);
    }
    bool isFirst = true;
    for (int k = 1; (k-1)*m < n; k++) {
        T p = s;
        s = fm(s);
        if (st.find(s) == st.end()) continue;
        for (int j = 0; j < m; j++,p = f(p)) {
            if (p == g) return (k-1)*m+j;
        }
        if (!isFirst) break;
        isFirst = false;
    }
    return -1;
}

#endif
