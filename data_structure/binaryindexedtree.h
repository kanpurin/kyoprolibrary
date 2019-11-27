#ifndef _BINARY_INDEXED_TREE_H_
#define _BINARY_INDEXED_TREE_H_

#include <vector>
#include <iostream>

// 0-indexedとして使う
// 実装は1-indexed
template< typename T >
struct BinaryIndexedTree {
    std::vector< T > data;

    BinaryIndexedTree(int sz) {
        data.assign(++sz, 0);
    }

    // [0,k]
    inline T sum(int k) {
        T ret = 0;
        for (++k; k > 0; k -= k & -k) ret += data[k];
        return (ret);
    }

    // [left,right]
    inline T sum(int left, int right) {
        return sum(right) - sum(left - 1);
    }

    // k番目にxを加算
    // 負の数も可
    inline void add(int k, T x) {
        for (++k; k < data.size(); k += k & -k) data[k] += x;
    }

    // k番目の値 0-indexed
    // 値 = index, 個数 = data[index]
    // 0がdata[0]個, 1がdata[1]個...
    int get(int64_t k) {
        ++k;
        int res = 0;
        int N = 1; while (N < (int)data.size()) N *= 2;
        for (int i = N / 2; i > 0; i /= 2) {
            if (res + i < (int)data.size() && data[res + i] < k) {
                k = k - data[res + i];
                res = res + i;
            }
        }
        return res + 1;
    }

    void print() {
        std::cout << "[ ";
        for (int i = 0; i < data.size() - 1; i++) {
            std::cout << sum(i, i);
            if (i < data.size() - 2) std::cout << ", ";
        }
        std::cout << " ]" << std::endl;
    }
};

#endif
