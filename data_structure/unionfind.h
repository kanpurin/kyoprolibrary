#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <iostream>
#include <vector>

class UnionFind {
private:
    std::vector<int> par;
public:
    UnionFind(int n) {
        par.resize(n, -1);
    }

    int root(int x) {
        if (par[x] < 0) return x;
        return par[x] = root(par[x]);
    }

    bool unite(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return false;
        if (size(rx) < size(ry)) std::swap(rx, ry);
        par[rx] += par[ry];
        par[ry] = rx;
        return true;
    }

    bool same(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        return rx == ry;
    }

    int size(int x) {
        return -par[root(x)];
    }
};

#endif
