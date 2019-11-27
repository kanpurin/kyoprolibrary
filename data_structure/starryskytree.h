#ifndef _STARRY_SKY_TREE_H_
#define _STARRY_SKY_TREE_H_

#include <functional>
#include <vector>
#include <iostream>

// RAQ and RMQ
template<typename Monoid>
struct StarrySkyTree {
private:
    using Func = std::function<Monoid(Monoid, Monoid)>;
    Func F;
    Monoid UNITY;
    int n;
    std::vector<Monoid> node, lazy;

    // propagation
    void eval(int k, int l, int r) {
        if (lazy[k] != 0) {
            node[k] += lazy[k];
            if (r - l > 1) {
                lazy[2 * k + 1] += lazy[k];
                lazy[2 * k + 2] += lazy[k];
            }
            lazy[k] = 0;
        }
    }
public:
    StarrySkyTree() {}

    //m=size, f=[](M a,M b){return ;}
    // size : m
    StarrySkyTree(int m, const Func f, const Monoid &unity) {
        build(m, f, unity);
    }

    StarrySkyTree(const std::vector<Monoid>& v, const Func f, const Monoid &unity) {
        build(v, f, unity);
    }

    void build(int m, const Func f, const Monoid &unity) {
        F = f;
        UNITY = unity;
        n = 1; while (n < m) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        lazy.resize(n * 2 - 1, 0);
    }

    void build(const std::vector<Monoid>& v, const Func f, const Monoid &unity) {
        F = f;
        UNITY = unity;
        int sz = v.size();
        n = 1; while (n < sz) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        lazy.resize(n * 2 - 1, 0);
        for(int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        for (int i = n - 2; i >= 0; i--) node[i] = F(node[2 * i + 1], node[2 * i + 2]);
    }

    // [a,b)
    void range_add(int a, int b, Monoid x, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);

        // out of range
        if (b <= l || r <= a) return;

        if (a <= l && r <= b) {
            lazy[k] += x;
            eval(k, l, r);
        }
        else {
            range_add(a, b, x, 2 * k + 1, l, (r - l) / 2 + l);
            range_add(a, b, x, 2 * k + 2, (r - l) / 2 + l, r);
            node[k] = F(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    void update(int x, int val) {
        if (x >= n || x < 0) return;
        x += n - 1;
        node[x] = val;
        while (x > 0) {
            x = (x - 1) >> 1;
            node[x] = F(node[2 * x + 1], node[2 * x + 2]);
        }
    }

    // [a,b)
    Monoid get(int a, int b, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (r <= a || b <= l) return UNITY;
        eval(k, l, r);
        if (a <= l && r <= b) return node[k];
        Monoid vl = get(a, b, 2 * k + 1, l, (r - l) / 2 + l);
        Monoid vr = get(a, b, 2 * k + 2, (r - l) / 2 + l, r);
        return F(vl, vr);
    }

    Monoid operator[](int x)const {
        return get(x, x + 1);
    }

    int size() {
        return n;
    }

    void print() {
        for(int i = 0; i < n; i++) {
            std::cout << i << "\t: " << get(i, i + 1) << std::endl;
        }
    }
};

#endif
