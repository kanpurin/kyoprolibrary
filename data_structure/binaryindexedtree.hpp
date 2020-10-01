#ifndef _BINARY_INDEXED_TREE_HPP_
#define _BINARY_INDEXED_TREE_HPP_

#include <vector>
#include <iostream>

// 0-indexedとして使う
// 実装は1-indexed
template< typename T >
struct BinaryIndexedTree {
    std::vector< T > data;

    BinaryIndexedTree() {}

    BinaryIndexedTree(int sz) {
        data.assign(++sz, 0);
    }

    // [0,k)
    inline T sum(int k) const {
        T ret = 0;
        for (k; k > 0; k -= k & -k) ret += data[k];
        return (ret);
    }

    // [left,right)
    inline T sum(int left, int right) const {
        return sum(right-1) - sum(left - 1);
    }

    // k番目にxを加算
    // 負の数も可
    inline void add(int k, T x) {
        for (++k; k < data.size(); k += k & -k) data[k] += x;
    }

    // [0,x]の和がk以上となる最小のx(0-indexed)
    int lower_bound(long long k) const {
        if (k <= 0) return 0;
        int res = 0;
        int N = 1; while (N < (int)data.size()) N *= 2;
        for (int i = N / 2; i > 0; i /= 2) {
            if (res + i < (int)data.size() && data[res + i] < k) {
                k -= data[res + i];
                res += i;
            }
        }
        return res;
    }
    
    friend std::ostream& operator<<(std::ostream &os, const BinaryIndexedTree &bit) {
        os << "[ ";
        for (int i = 0; i < (int)bit.data.size() - 1; i++) {
            os << bit.sum(i, i + 1);
            if (i < (int)bit.data.size() - 2) os << ", ";
        }
        os << " ]";
        return os;
    }
};

#endif
