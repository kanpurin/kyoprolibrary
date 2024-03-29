#ifndef _POWMOD_HPP_
#define _POWMOD_HPP_

//powMod
long long powMod(long long k, long long n, long long mod) {
    long long x = 1;
    while (n > 0) {
        if (n & 1) {
            x = x * k % mod;
        }
        k = k * k % mod;
        n >>= 1;
    }
    return x;
}

#endif
