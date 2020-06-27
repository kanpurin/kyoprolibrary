#ifndef _COMBINATION_H_
#define _COMBINATION_H_

#include <vector>

// combination
long long comb(long long n, long long r) {
    std::vector<std::vector<long long>> v(n + 1, std::vector<long long>(n + 1, 0));
    for (long long i = 0; i < (long long)v.size(); i++) {
        v[i][0] = 1;
        v[i][i] = 1;
    }
    for (long long j = 1; j < (long long)v.size(); j++) {
        for (long long k = 1; k < j; k++) {
            v[j][k] = v[j - 1][k - 1] + v[j - 1][k];
        }
    }
    return v[n][r];
}

#endif
