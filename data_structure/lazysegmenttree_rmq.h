#ifndef _LAZY_SEGMENT_TREE_RMQ_H_
#define _LAZY_SEGMENT_TREE_RMQ_H_

#include <functional>
#include <vector>
#include <iostream>
#include <limits>

// RAQ RUQ and RMQ
// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_F
// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_H
template<typename Monoid>
struct LazySegmentTree {
private:
    using Func = std::function<Monoid(Monoid, Monoid)>;
    Func F = [](Monoid a, Monoid b) { return min(a, b); };
    Monoid UNITY = std::numeric_limits<Monoid>::max();
    int n;
    std::vector<Monoid> node, lazy;
    std::vector<int> prop; // 何を伝播させるか 1:add 2:update

    // propagation
    void eval(int k, int l, int r) {
        if (prop[k] != 0) {
            node[k] = lazy[k] + (prop[k] == 1 ? node[k] : 0);
            if (r - l > 1) {
                lazy[2 * k + 1] = lazy[k] + (prop[k] == 1 ? lazy[2 * k + 1] : 0);
                lazy[2 * k + 2] = lazy[k] + (prop[k] == 1 ? lazy[2 * k + 2] : 0);
                if (prop[2 * k + 1] != 2) prop[2 * k + 1] = prop[k];
                if (prop[2 * k + 2] != 2) prop[2 * k + 2] = prop[k];
            }
            lazy[k] = 0;
            prop[k] = 0;
        }
    }
public:
    LazySegmentTree(int m, Monoid val) {
        n = 1; while (n < m) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        lazy.resize(n * 2 - 1, 0);
        prop.resize(n * 2 - 1, 0);
        if (val != LazySegmentTree::UNITY) {
            for (int i = 0; i < m; i++) node[n + i - 1] = val;
            build();
        }
    }

    LazySegmentTree(const std::vector<Monoid>& v) {
        int sz = v.size();
        n = 1; while (n < sz) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        lazy.resize(n * 2 - 1, 0);
        prop.resize(n * 2 - 1, 0);
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

    // [a,b) add
    void add_query(int a, int b, const Monoid &x, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);

        // out of range
        if (b <= l || r <= a) return;

        if (a <= l && r <= b) {
            prop[k] = 1;
            lazy[k] += x;
            eval(k, l, r);
        }
        else {
            add_query(a, b, x, 2 * k + 1, l, (r - l) / 2 + l);
            add_query(a, b, x, 2 * k + 2, (r - l) / 2 + l, r);
            node[k] = F(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    // [a,b) update
    void update_query(int a, int b, const Monoid &x, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);

        // out of range
        if (b <= l || r <= a) return;

        if (a <= l && r <= b) {
            prop[k] = 2;
            lazy[k] = x;
            eval(k, l, r);
        }
        else {
            update_query(a, b, x, 2 * k + 1, l, (r - l) / 2 + l);
            update_query(a, b, x, 2 * k + 2, (r - l) / 2 + l, r);
            node[k] = F(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    // [a,b) min
    Monoid get_query(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (r <= a || b <= l) return UNITY;
        eval(k, l, r);
        if (a <= l && r <= b) return node[k];
        Monoid vl = get_query(a, b, 2 * k + 1, l, (r - l) / 2 + l);
        Monoid vr = get_query(a, b, 2 * k + 2, (r - l) / 2 + l, r);
        return F(vl, vr);
    }

    Monoid operator[](int x) {
        return get_query(x, x + 1);
    }

    void print() {
        for (int i = 0; i < n; i++) {
            std::cout << i << "\t: " << get_query(i, i + 1) << std::endl;
        }
    }
};

#endif
