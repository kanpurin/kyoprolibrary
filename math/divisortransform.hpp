#ifndef _DIVISOR_TRANSFORM_HPP_
#define _DIVISOR_TRANSFORM_HPP_

#include <vector>

// b[i]=Σ_{j|i}a[j] / b[i]=Σ_{i|j}a[j]
// ゼータ変換:上位/下位の集合の和をとる, メビウス変換:逆変換
// subset:上位集合 gcd畳み込みは下位
template < class T >
void divisor_transform(std::vector< T >& a, bool zeta, bool subset) {
    int n = a.size();
    std::vector< bool > sieve(n, false);
    for (int p = 2; p < n; ++p) {
        if (sieve[p]) continue;
        if (!subset) {
            if (zeta) for (int k=0; k*p<n; k++) sieve[k*p]=true, a[k*p]+=a[k];
            else for (int k=(n-1)/p; k>=0; k--) sieve[k*p]=true, a[k*p]-=a[k];
        }
        else {
            if (zeta) for (int k=(n-1)/p; k>=0; k--) sieve[k*p]=true, a[k]+=a[k*p];
            else for (int k=0; k*p<n; k++) sieve[k*p]=true, a[k]-=a[k*p];
        }
    }
}

#endif