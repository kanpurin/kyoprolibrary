#ifndef _COMBINATION_PRE_HPP_
#define _COMBINATION_PRE_HPP_

#include <vector>
#include "../data_structure/mint.hpp"

// comb (前計算O(NlogMOD) クエリO(1))
template<typename T >
struct Combination {
private:
    int N;

public:
    std::vector< T > FACT, IFACT;
    // nCk
    Combination(int n) : N(n) {
        FACT.resize(n + 1);
        IFACT.resize(n + 1);
        FACT[0] = 1;
        IFACT[0] = 1;
        for (int i = 1; i <= n; i++) {
            FACT[i] = FACT[i - 1] * i;
        }
        IFACT[n] = T(1) / FACT[n];
        for (int i = n-1; i >= 0; i--) {
            IFACT[i] = IFACT[i+1] * (i+1);
        }
    }

    T comb(int n, int r) {
        if (n < 0 || r < 0 || r > n) return 0;
        if (r > n / 2) r = n - r;
        return FACT[n] * IFACT[n - r] * IFACT[r];
    }
};

#endif
