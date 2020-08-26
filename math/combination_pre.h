#ifndef _COMBINATION_PRE_H_
#define _COMBINATION_PRE_H_

#include <vector>
#include "mint"

// comb (前計算O(NlogMOD) クエリO(1))
struct Combination {
private:
    int N;

public:
    std::vector< mint > FACT, IFACT;
    // nCk
    Combination(int n) : N(n) {
        FACT.resize(n + 1);
        IFACT.resize(n + 1);
        FACT[0] = 1;
        IFACT[0] = 1;
        for (int i = 1; i <= n; i++) {
            FACT[i] = FACT[i - 1] * i;
            IFACT[i] = mint(1) / FACT[i];
        }
    }

    mint comb(int n, int r) {
        if (n < 0 || r < 0 || r > n) return 0;
        if (r > n / 2) r = n - r;
        return FACT[n] * IFACT[n - r] * IFACT[r];
    }
};

#endif
