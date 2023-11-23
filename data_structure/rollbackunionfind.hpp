#ifndef _ROLLBACK_UNION_FIND_HPP_
#define _ROLLBACK_UNION_FIND_HPP_

#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include <functional>
#include <cassert>

// RollbackUnionFind(A:可換半群,B:可換モノイド)
// 連結成分内の値の総和(sum)(A)の総和(fold)(B)
template<typename T >
class RollbackUnionFind {
private:
    using Func = std::function< T(T, T) >;
    Func funcA, funcB;
    bool existA,existB;
    std::vector<int> par;
    std::stack<std::tuple<int,int,T>> history;
    std::vector<T> val;
    std::stack<int> inner_snap;
    std::vector<T> node;
    int _n;
    T unityB;

    void init_build(std::vector<T> v) {
        int sz = v.size();
        _n = 1;
        while(_n < sz) _n <<= 1;
        node.resize(_n * 2 - 1, unityB);
        for (int i = 0; i < sz; i++) node[i + _n - 1] = v[i];
        for (int i = _n - 2; i >= 0; i--) node[i] = funcB(node[2 * i + 1], node[2 * i + 2]);
    }

    void update_node(int i, T x) {
        if (get_node(i) == x) return;
        if (!existB) { node[i] = x; return; }
        i += _n - 1;
        node[i] = x;
        while (i > 0) {
            i = (i - 1) >> 1;
            node[i] = funcB(node[2 * i + 1], node[2 * i + 2]);
        }
    }

    inline T get_node(int i) const {
        return node[existB?_n+i-1:i];
    }

public:
    RollbackUnionFind(){}
    RollbackUnionFind(int n) {
        existA = existB = false;
        par.resize(n, -1);
        inner_snap.emplace(0);
    }

    RollbackUnionFind(std::vector<T> v, const Func f) {
        funcA = f;
        existA = true; existB = false;
        node = std::move(v);
        par.resize(node.size(), -1);
        inner_snap.emplace(0);
    }

    RollbackUnionFind(int n, const T &val, const Func f) {
        funcA = f;
        existA = true; existB = false;
        node.resize(n, val);
        par.resize(n, -1);
        inner_snap.emplace(0);
    }

    RollbackUnionFind(std::vector<T> v, const Func fa, const Func fb, const T &unityB) {
        funcA = fa; existA = true;
        funcB = fb; existB = true;
        par.resize(v.size(), -1);
        this->unityB = unityB;
        init_build(v);
        inner_snap.emplace(0);
    }

    RollbackUnionFind(int n, const T &val, const Func fa, const Func fb, const T &unityB) {
        funcA = fa; existA = true;
        funcB = fb; existB = true;
        par.resize(n, -1);
        this->unityB = unityB;
        init_build(std::vector<T>(n,val));
        inner_snap.emplace(0);
    }

    // O(logN)
    int root(int x) const {
        while(par[x] >= 0) x = par[x];
        return x;
    }

    // O(logN)
    bool unite(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return false;
        if (size(rx) < size(ry)) std::swap(rx, ry);
        history.emplace(make_tuple(rx,par[rx],get_node(rx)));
        history.emplace(make_tuple(ry,par[ry],get_node(ry)));
        par[rx] += par[ry];
        par[ry] = rx;
        if (existA) {
            update_node(rx,funcA(get_node(rx),get_node(ry)));
            update_node(ry,unityB);
        }
        return true;
    }

    // val[x] <- val[x] + val
    // O(logN)
    void add(int x, const T val) {
        assert(existA);
        int rx = root(x);
        history.emplace(make_tuple(rx,par[rx],get_node(rx)));
        update_node(rx,funcA(get_node(rx),val));
    }

    // O(logN)
    T sum(int x) const {
        assert(existA);
        return get_node(root(x));
    }

    // O(1)
    T fold() const {
        assert(existB);
        return node[0];
    } 

    // O(logN)
    bool same(int x, int y) const {
        int rx = root(x);
        int ry = root(y);
        return rx == ry;
    }

    // O(logN)
    int size(int x) const {
        return -par[root(x)];
    }

    void snapshot() {
        inner_snap.emplace(history.size());
    }

    void rollback() {
        assert(inner_snap.size() > 0);
        int snap = inner_snap.top(); inner_snap.pop();
        while(snap < history.size()) {
            auto p = history.top(); history.pop();
            par[std::get<0>(p)] = std::get<1>(p);
            update_node(std::get<0>(p),std::get<2>(p));
        }
    }
};

#endif
