#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_

#include <vector>
#include <algorithm>
#include "unionfind.h"

// 最小全域木 O(ElogV)?
struct Edge {
    long long cost, from, to;
    bool operator>(const Edge e)const {
        return this->cost > e.cost;
    }
    bool operator<(const Edge e)const {
        return this->cost < e.cost;
    }
};

long long Kruskal(std::vector< Edge > G, int v) {
    sort(G.begin(),G.end());
    UnionFind uni(v);
    long long res = 0;
    for(int i = 0; i < G.size(); i++) {
        if (uni.unite(G[i].from, G[i].to)){
            res += G[i].cost;
        }
    }
    return res;
}

#endif
