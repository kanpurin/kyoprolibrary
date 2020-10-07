#ifndef _MILLER_RABIN_H_
#define _MILLER_RABIN_H_

#include <random>

// MillerRabin素数判定法
// verify:https://yukicoder.me/problems/no/774
bool MillerRabin(long long v, int loop = 10) {
    long long d = v - 1;
    int s = 0, i, j;
    if (v <= 1) return false;
    if (v == 2) return true;
    if (v % 2 == 0) return false;
    while (d % 2 == 0) d /= 2, s++;

    auto modpow = [](__int128_t a, long long n, long long mo) {
        __int128_t r = 1;
        a %= mo;
        while (n) r = r * ((n % 2) ? a : 1) % mo, a = a * a % mo, n >>= 1;
        return r;
    };

    for (i = 0; i < loop; i++) {
        long long a = abs(1LL * rand() * rand() + rand()) % (v - 2) + 1;
        long long r = modpow(a, d, v);
        if (r == 1 || r == v - 1) continue;
        for (j = 0; j < s; j++) {
            r = modpow(r, 2, v);
            if (r == v - 1) break;
        }
        if (j == s) return false;
    }
    return true;
}

#endif

int main() {
    return 0;
}