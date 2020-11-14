#ifndef _GAUSS_JORDAN_MOD2_H_
#define _GAUSS_JORDAN_MOD2_H_

#include <vector>
#include <bitset>
#include "../data_structure/mint.hpp"

constexpr int MOD = 1e9 + 7;

// mod2掃き出し法 O(N^3) 
// 拡大係数行列ならtrue
// 行列のランクを返す
const int bitsize = 30;
int GaussJordan_mod2(std::vector<std::bitset<bitsize>> &A, bool is_extended = false) {
    int rank = 0;
    for (int col = 0; col < bitsize; ++col) {
        if (is_extended && col == bitsize - 1) break;
        int pivot = -1;
        for (int row = rank; row < (int)A.size(); ++row) {
            if (A[row][col]) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(A[pivot], A[rank]);
        for (int row = 0; row < (int)A.size(); ++row) {
            if (row != rank && A[row][col]) A[row] ^= A[rank];
        }
        ++rank;
    }
    return rank;
}

#endif