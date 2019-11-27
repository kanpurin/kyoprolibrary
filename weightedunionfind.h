#ifndef _WEIGHTED_UNION_FIND_H_
#define _WEIGHTED_UNION_FIND_H_

#include <vector>

template<typename T>
struct WeightedUnionFind {
private:
    std::vector<int> par;
    std::vector<int> rank;
    std::vector<T> diff_weight;
public:
    WeightedUnionFind(int n) {
        par.resize(n); rank.resize(n); diff_weight.resize(n);
        for (int i = 0; i < n; ++i) par[i] = i, rank[i] = 0, diff_weight[i] = 0;
    }

    int root(int x) {
        if (par[x] == x) {
            return x;
        }
        else {
            int r = root(par[x]);
            diff_weight[x] += diff_weight[par[x]];
            return par[x] = r;
        }
    }

    T weight(int x) {
        root(x);
        return diff_weight[x];
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }

    // y - x = w
    bool unite(int x, int y, T w) {
        w += weight(x); w -= weight(y);
        x = root(x); y = root(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) std::swap(x, y), w = -w;
        if (rank[x] == rank[y]) ++rank[x];
        par[y] = x;
        diff_weight[y] = w;
        return true;
    }

    // y - x
    T diff(int x, int y) {
        return weight(y) - weight(x);
    }
};

#endif
