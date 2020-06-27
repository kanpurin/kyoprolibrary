#ifndef _COMBINATION_ONLY_ONE_H_
#define _COMBINATION_ONLY_ONE_H_

#include "powmod.h"

// comb (一つのみ) O(r)
long long comb(ll n, ll r, ll mod) {
    long long ret = 1;
    while (true) {
        if (r == 0) break;
        long long N = n % mod;
        long long R = r % mod;
        if (N < R) return 0;
        for (int i = 0; i < R; i++)
        {
            ret = ret * (N - i) % mod;
        }
        long long imul = 1;
        for (int i = 0; i < R; i++)
        {
            imul = imul * (i + 1) % mod;
        }
        ret = ret * powMod(imul, mod - 2, mod) % mod;
        n /= mod; r /= mod;
    }
    return ret;
}

#endif
