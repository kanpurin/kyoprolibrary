#ifndef _SEGMENT_TREE_HPP_
#define _SEGMENT_TREE_HPP_

#include <functional>
#include <vector>
#include <iostream>

// verify : https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_A
template < class Monoid >
struct SegmentTree {
private:
    using Func = std::function< Monoid(Monoid, Monoid) >;
    Func F;
    Monoid UNITY;
    int n;
    std::vector< Monoid > node;

    int _binary_search(int a, int b, const std::function<bool(Monoid)> &f, Monoid &s, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (r <= a || b <= l) return n;
        if (a <= l && r <= b && !f(F(s,node[k]))) {
            s = F(s,node[k]);
            return n;
        }
        if (l == r - 1) {s = F(s,node[k]); return l;}
        int ret = _binary_search(a, b, f, s, 2 * k + 1, l, (r - l) / 2 + l);
        return ret != n ? ret : _binary_search(a, b, f, s, 2 * k + 2, (r - l) / 2 + l, r);
    }

public:
    SegmentTree() {}

    SegmentTree(const std::vector< Monoid > &v, const Func f, const Monoid &unity) {
        F = f;
        UNITY = unity;
        int sz = v.size();
        n = 1;
        while (n < sz) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        build();
    }

    // val で埋める
    SegmentTree(int m, const Monoid &val, const Func f, const Monoid &unity) {
        F = f;
        UNITY = unity;
        n = 1;
        while (n < m) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        if (val != UNITY) {
            for (int i = 0; i < m; i++) node[i + n - 1] = val;
            build();
        }
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

    // [l,r)
    Monoid get_query(int l, int r) {
        Monoid L = UNITY, R = UNITY;
        if (l < 0) l = 0;
        if (r < 0) return UNITY;
        if (l >= n) return UNITY;
        if (r >= n) r = n;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) L = F(L, node[l++ - 1]);
            if (r & 1) R = F(node[--r - 1], R);
        }
        return F(L, R);
    }

    // [l,r)を満たすkでf(get_query(l,k+1))=trueとなる最小のk
    // 無ければ-1
    int binary_search(int l, int r, const std::function<bool(Monoid)> &f) {
        Monoid s = UNITY;
        int ret = _binary_search(l,r,f,s);
        return ret != n ? ret : -1;
    }

    Monoid operator[](int x) const {
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

int main() {
    return 0;
}