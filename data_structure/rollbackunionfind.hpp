#ifndef _ROLLBACK_UNION_FIND_HPP_
#define _ROLLBACK_UNION_FIND_HPP_

#include <iostream>
#include <vector>
#include <stack>
#include <utility>

// RollbackUnionFind
class RollbackUnionFind {
private:
    std::vector<int> par;
    std::stack<std::pair<int,int>> history;
    int inner_snap;
public:
    RollbackUnionFind(int n) : inner_snap(0) {
        par.resize(n, -1);
    }

    // O(logN)
    int root(int x) {
        if (par[x] < 0) return x;
        return root(par[x]);
    }

    // O(logN)
    bool unite(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        history.push(std::make_pair(x,par[x]));
        history.push(std::make_pair(y,par[y]));
        if (rx == ry) return false;
        if (size(rx) < size(ry)) std::swap(rx, ry);
        par[rx] += par[ry];
        par[ry] = rx;
        return true;
    }

    // O(logN)
    bool same(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        return rx == ry;
    }

    // O(1)
    int size(int x) {
        return -par[root(x)];
    }

    // O(1)
    void undo() {
        par[history.top().first] = history.top().second;
        history.top();
        par[history.top().first] = history.top().second;
        history.pop();
    }

    // O(1)
    void snapshot() { inner_snap = int(history.size() >> 1); }

    void rollback() {
        while (2*inner_snap < (int)history.size()) undo();
    }
};

#endif
