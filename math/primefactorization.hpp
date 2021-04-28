#ifndef _PRIME_FACTORIZATION_HPP_
#define _PRIME_FACTORIZATION_HPP_

#include <vector>

// 素因数分解 O(√n)以下
// (素因数,個数)
template<typename T>
std::vector<std::pair<T,int>> prime_factorization(T n) {
    std::vector<std::pair<T, int>> res;
    for (T i = 2; i*i <= n; i++) {
        int cnt = 0;
        while (n % i == 0) {
            n /= i;
            cnt++;
        }
        if (cnt > 0) res.push_back({i,cnt});
    }
    if (n > 1) res.push_back({n,1});
    return res;
}

#endif
