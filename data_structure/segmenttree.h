#ifndef _SEGMENT_TREE_H_
#define _SEGMENT_TREE_H_

#include <functional>
#include <vector>
#include <iostream>

// verify : https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_A
template<class Monoid> struct SegmentTree {
private:
    using Func = std::function<Monoid(Monoid, Monoid)>;
    Func F;
    Monoid UNITY;
    int n;
    std::vector<Monoid> node;
public:
    SegmentTree() {}

    // val で埋める
    SegmentTree(int m, const Monoid &val, const Func f, const Monoid &unity) {
        F = f;
        UNITY = unity;
        n = 1; while (n < m) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        if (val != UNITY) {
            for (int i = 0; i < m; i++) node[i] = val;
            build();
        }
    }

    SegmentTree(const std::vector<Monoid>& v, const Func f, const Monoid &unity) {
        F = f;
        UNITY = unity;
        int sz = v.size();
        n = 1; while (n < sz) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        build();
    }

    // 最後に build が必要
    void set(int k, const Monoid &x) {
        node[n + k - 1] = x;
    }

    void build() {
        for (int i = n - 2; i >= 0; i--) node[i] = F(node[2 * i + 1], node[2 * i + 2]);
    }

    void update_query(int x, const Monoid &val) {
        if (x >= n || x < 0) return;
        x += n - 1;
        node[x] = val;
        while (x > 0) {
            x = (x - 1) >> 1;
            node[x] = F(node[2 * x + 1], node[2 * x + 2]);
        }
    }

    // [a,b)
    Monoid get_query(int a, int b) {
        Monoid L = UNITY, R = UNITY;
        for (a += n, b += n; a < b; a >>= 1, b >>= 1) {
            if (a & 1) L = F(L, node[a++ - 1]);
            if (b & 1) R = F(node[--b - 1], R);
        }
        return F(L, R);
    }

    Monoid operator[](int x)const {
        return node[n + x - 1];
    }

    int size() {
        return n;
    }

    void print() {
        for (int i = 0; i < n; i++) {
            std::cout << i << "\t: " << node[n + i - 1] << std::endl;
        }
    }
};

#endif
