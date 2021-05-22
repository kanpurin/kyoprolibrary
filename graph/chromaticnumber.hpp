#ifndef _CHROMATIC_NUMBER_HPP_
#define _CHROMATIC_NUMBER_HPP_

#include <vector>

// 単純無向グラフの彩色数
// G:隣接行列 O(N2^N)
int chromatic_number(std::vector<std::vector<int>> &G) {
    auto powMod = [](long long k, long long n, long long mod) {
        long long x = 1;
        while (n > 0) {
            if (n & 1) {
                x = x * k % mod;
            }
            k = k * k % mod;
            n >>= 1;
        }
        return x;
    };
    int n = G.size();
    std::vector<int> neighbor(n, 0);
    for (int i = 0; i < n; ++i) {
        int S = (1<<i);
        for (int j = 0; j < n; ++j)
            if (G[i][j]) S |= (1<<j);
        neighbor[i] = S;
    }
    std::vector<int> I(1<<n);
    I[0] = 1;
    for (int S = 1; S < (1<<n); ++S) {
        int v = __builtin_ctz(S);
        I[S] = I[S & ~(1<<v)] + I[S & ~neighbor[v]];
    }
    long long MOD1 = 998244353;
    long long MOD2 = 1e9+7;
    long long MOD3 = 1e9+9;
    int l = 0, r = n;
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        long long g1 = 0;
        long long g2 = 0;
        long long g3 = 0;
        for (int S = 0; S < (1<<n); ++S) {
            if ((n - __builtin_popcount(S)) & 1) {
                g1 -= powMod(I[S], mid, MOD1);
                g2 -= powMod(I[S], mid, MOD2);
                g3 -= powMod(I[S], mid, MOD3);
            }
            else {
                g1 += powMod(I[S], mid, MOD1);
                g2 += powMod(I[S], mid, MOD2);
                g3 += powMod(I[S], mid, MOD3);
            }
            g1 = (g1 % MOD1 + MOD1) % MOD1;
            g2 = (g2 % MOD2 + MOD2) % MOD2;
            g3 = (g3 % MOD3 + MOD3) % MOD3;
        }
        if (g1 != 0 && g2 != 0 && g3 != 0) r = mid;
        else l = mid;
    }
    return r;
}

#endif