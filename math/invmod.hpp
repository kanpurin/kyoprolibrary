#ifndef _INVMOD_H_
#define _INVMOD_H_

#include "../math/extgcd.hpp"

// gcd(a,mod) = 1
long long invMod(long long a, long long mod) {
    long long x,y;
    if (extgcd(a,mod,x,y) != 1) {
        return -1;
    }
    return (x % mod + mod) % mod;
}

#endif
