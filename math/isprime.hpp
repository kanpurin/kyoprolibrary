#ifndef _IS_PRIME_HPP_
#define _IS_PRIME_HPP_

#include <random>
#include <vector>

// MillerRabin素数判定法 O(logN)
// verify:https://yukicoder.me/problems/no/774
bool isprime(long long N) {
    if (N <= 1) return false;
    if (N == 2) return true;
    if (N % 2 == 0) return false;

    auto modpow = [](__int128_t a, long long n, long long mo) {
        __int128_t r = 1;
        a %= mo;
        while (n) r = r * ((n % 2) ? a : 1) % mo, a = a * a % mo, n >>= 1;
        return r;
    };

    std::vector<long long> A = {2, 325, 9375, 28178, 450775,
                           9780504, 1795265022};
    long long s = 0, d = N - 1;
    while (d % 2 == 0) d >>= 1, s++;

    for (long long a : A) {
        if (a % N == 0) return true;
        long long j;
        __int128_t r = modpow(a, d, N);
        if (r == 1) continue;
        for (j = 0; j < s; j++) {
            if (r == N - 1) break;
            r = r * r % N;
        }
        if (j == s) return false;
    }
    return true;
}

#endif