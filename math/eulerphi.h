#ifndef _EULER_PHI_H_
#define _EULER_PHI_H_

#include <vector>

// オイラーのφ関数 前処理O(NloglogN)
// N以下の数のφ関数を求める.
std::vector< int > euler_phi(int n) {
    std::vector< int > euler(n + 1);
    for (int i = 0; i <= n; i++) {
        euler[i] = i;
    }
    for (int i = 2; i <= n; i++) {
        if (euler[i] == i) {
            for (int j = i; j <= n; j += i) {
                euler[j] = euler[j] / i * (i - 1);
            }
        }
    }
    return euler;
}

#endif
