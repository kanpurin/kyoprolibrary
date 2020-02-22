#ifndef _LCS_H_
#define _LCS_H_

#include <vector>
#include <algorithm>
#include <iostream>

template< typename T >
int64_t LCS(const std::vector< T > &A, const std::vector< T > &B) {
    int n = (int)A.size(), m = (int)B.size();
    std::vector< std::vector< int64_t > > lcs(n + 1, std::vector< int64_t >(m + 1));
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            if (i == 0 || j == 0) {
                lcs[i][j] = 0;
            }
            else if (A[i - 1] == B[j - 1]) {
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
            }
            else {
                lcs[i][j] = std::max(lcs[i - 1][j], lcs[i][j - 1]);
            }
        }
    }
    return lcs[n][m];
}

#endif
