#ifndef _LAZY_SEGMENT_TREE_RMQ_ADDITION_H_
#define _LAZY_SEGMENT_TREE_RMQ_ADDITION_H_

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
    std::vector<int> prop; // 何を伝播させるか 1:add 2:update
    std::vector<Monoid> node, lazy, lazy2; // lazy2:p*iとなるpを伝播
    std::vector<int> index; // 最小値のindex 複数ある場合は最左

    // propagation
    void eval(int k, int l, int r) {
        if (prop[k] != 0) {
            if (prop[k] == 1) node[k] += lazy[k];
            else if (prop[k] == 2) node[k] = (lazy2[k] >= 0 ? lazy2[k] * l + lazy[k] : lazy2[k] * (r - 1) + lazy[k]);
            // else if (prop[k] == 2) node[k] = (lazy2[k] > 0 ? lazy2[k] * l + lazy[k] : lazy2[k] * (r - 1) + lazy[k]);
            
            if (r - l > 1) {
                if (prop[k] == 1) lazy[2*k+1]+=lazy[k],lazy[2*k+2]+=lazy[k];
                else if (prop[k] == 2) lazy[2*k+1]=lazy[2*k+2]=lazy[k],lazy2[2*k+1]=lazy2[2*k+2]=lazy2[k];

                if (prop[2 * k + 1] != 2 && prop[2 * k + 1] != 3) prop[2 * k + 1] = prop[k];
                if (prop[2 * k + 2] != 2 && prop[2 * k + 2] != 3) prop[2 * k + 2] = prop[k];
            }
            lazy[k] = 0;
            lazy2[k] = 0;
            prop[k] = 0;
        }
    }

    Monoid _get_query(int a, int b, int &idx, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        if (r <= a || b <= l) { idx = -1; return UNITY; }
        eval(k, l, r);
        if (a <= l && r <= b) { idx = index[k]; return node[k]; }
        int l_idx, r_idx;
        Monoid vl = _get_query(a, b, l_idx, 2 * k + 1, l, (r - l) / 2 + l);
        Monoid vr = _get_query(a, b, r_idx, 2 * k + 2, (r - l) / 2 + l, r);
        idx = F(vl,vr) == vl && l_idx != -1 ? l_idx : r_idx;
        // idx = F(vl,vr) == vr && l_idx != -1 ? r_idx : l_idx;
        return F(vl, vr);
    }

public:

    LazySegmentTree(int m, Monoid val) {
        n = 1; while (n < m) n <<= 1;
        node.resize(n * 2 - 1, UNITY);
        lazy.resize(n * 2 - 1, 0);
        lazy2.resize(n * 2 - 1, 0);
        prop.resize(n * 2 - 1, 0);
        index.resize(n * 2 - 1, -1);
        for (int i = 0; i < m; i++) index[n + i - 1] = i;
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
        lazy2.resize(n * 2 - 1, 0);
        prop.resize(n * 2 - 1, 0);
        index.resize(n * 2 - 1, 0);
        for (int i = 0; i < sz; i++) index[n + i - 1] = i;
        for (int i = 0; i < sz; i++) node[i + n - 1] = v[i];
        build();
    }

    // 最後に build が必要
    void set(int k, const Monoid &x) {
        node[n + k - 1] = x;
    }

    void build() {
        for (int i = n - 2; i >= 0; i--) node[i] = F(node[2 * i + 1], node[2 * i + 2]);
        for (int i = n - 2; i >= 0; i--) index[i] = node[2 * i + 1] == F(node[2 * i + 1], node[2 * i + 2]) ? index[2 * i + 1] : index[2 * i + 2];
        //for (int i = n - 2; i >= 0; i--) index[i] = node[2 * i + 2] == F(node[2 * i + 1], node[2 * i + 2]) ? index[2 * i + 2] : index[2 * i + 1];
    }

    // [a,b) add
    void add_query(int a, int b, const Monoid x, int k = 0, int l = 0, int r = -1) {
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
            index[k] = F(node[2 * k + 1], node[2 * k + 2]) == node[2 * k + 1] ? index[2 * k + 1] : index[2 * k + 2];
            // index[k] = F(node[2 * k + 1], node[2 * k + 2]) == node[2 * k + 2] ? index[2 * k + 2] : index[2 * k + 1];
            node[k] = F(node[2 * k + 1], node[2 * k + 2]);
        }
    }
    
    // [a,b) update p*i+q
    void update_linear_query(int a, int b, Monoid p, Monoid q, int k = 0, int l = 0, int r = -1) {
        if (r < 0) r = n;
        eval(k, l, r);

        // out of range
        if (b <= l || r <= a) return;

        if (a <= l && r <= b) {
            prop[k] = 2;
            lazy[k] = q;
            lazy2[k] = p;
            if (p >= 0) index[k] = l;
            else index[k] = r - 1;
            eval(k, l, r);
        }
        else {
            update_linear_query(a, b, p, q, 2 * k + 1, l, (r - l) / 2 + l);
            update_linear_query(a, b, p, q, 2 * k + 2, (r - l) / 2 + l, r);
            index[k] = F(node[2 * k + 1], node[2 * k + 2]) == node[2 * k + 1] ? index[2 * k + 1] : index[2 * k + 2];
            // index[k] = F(node[2 * k + 1], node[2 * k + 2]) == node[2 * k + 2] ? index[2 * k + 2] : index[2 * k + 1];
            node[k] = F(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    // [a,b) update
    void update_query(int a, int b, Monoid x) {
        update_linear_query(a,b,0,x);
    }   

    // [a,b) min
    Monoid get_query(int a, int b) {
        int tmp;
        return _get_query(a,b,tmp);
    }

    // [a,b) min index
    int get_index_query(int a, int b) {
        int idx;
        _get_query(a,b,idx);
        return idx;
    }

    Monoid operator[](int x) {
        return get_query(x, x + 1);
    }

    void print() {
        for (int i = 0; i < n; i++) {
            std::cout << i << "\t: " << get_query(i, i + 1) << std::endl;
        }
    }

    template<typename T>
    void debug(const std::vector<T> &node) {
        int step=1,maxstep;
        for(maxstep=0;(1<<maxstep)<2*n-1;maxstep++);
        std::cout << maxstep << std::endl;
        for(int i = 0; i < 2 * n - 1; i++) {
            std::cout << node[i];
            for(int j=0;j<(1<<(maxstep-step));j++){
                std::cout << '\t';
            }
            if (i == (1<<step)-2) {
                step++;
                std::cout << std::endl;
            }
        }
    }

    void debug_node() {
        debug(node);
    }
};

#endif
