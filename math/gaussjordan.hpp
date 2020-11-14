#ifndef _GAUSS_JORDAN_H_
#define _GAUSS_JORDAN_H_

#include <vector>
#include "../data_structure/mint.hpp"

constexpr int MOD = 1e9 + 7;

// 掃き出し法 O(N^3)
// 拡大係数行列ならtrue
// 行列のランクを返す
int GaussJordan(std::vector<std::vector<mint<MOD>>> &A, bool is_extended = false) {
    int m = A.size(), n = A[0].size();
    int rank = 0;
    for (int col = 0; col < n; ++col) {
        if (is_extended && col == n-1) break;
        int pivot = -1;
        for (int row = rank; row < m; ++row) {
            if (A[row][col] != 0) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;
        std::swap(A[pivot], A[rank]);
        auto inv = mint<MOD>(1) / A[rank][col];
        for (int col2 = 0; col2 < n; ++col2)
            A[rank][col2] = A[rank][col2] * inv;
        for (int row = 0; row < m; ++row) {
            if (row != rank && A[row][col] != 0) {
                auto fac = A[row][col];
                for (int col2 = 0; col2 < n; ++col2) {
                    A[row][col2] -= A[rank][col2] * fac;
                }
            }
        }
        ++rank;
    }
    return rank;
}

#endif