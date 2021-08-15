#ifndef _ZETA_MOBIUS_TRANSFORM_HPP_
#define _ZETA_MOBIUS_TRANSFORM_HPP_

#include <vector>

// ゼータ変換:上位/下位の集合の和をとる, メビウス変換:逆変換
template < class T >
void transform(std::vector< T >& f, bool zeta, bool subset) {
    int n = f.size();
    if (!subset) {
        for (int i = 0; i < n; i++) for (int j = 0; j < 1<<n; j++) {
            if (!(j & (1<<i))) f[j] += (zeta?f[j|(1<<i)]:-f[j|(1<<i)]);
        }
    }
    else {
        for (int i = 0; i < n; i++) for (int j = 0; j < 1<<n; j++) {
            if (j & (1<<i)) f[j] += (zeta?f[j^(1<<i)]:-f[j^(1<<i)]);
        }
    }
}

#endif
