#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_

#include <vector>
#include <algorithm>
#include "unionfind.h"

struct Kruskal {
private:
    int V; // 頂点数
    int E; // 辺数
    struct Edge {
        long long cost; int from, to;
        Edge(long long cost, int from, int to) :cost(cost), from(from), to(to) {}
        bool operator>(const Edge e)const {
            return this->cost > e.cost;
        }
        bool operator<(const Edge e)const {
            return this->cost < e.cost;
        }
    };

    std::vector<Edge> G;

public:
    Kruskal(int V) : V(V) {
        E = 0;
    }

    void add_edge(int from, int to, long long cost) {
        G.push_back(Edge(cost, from, to));
        E++;
    }

    long long build() {
        sort(G.begin(), G.end());
        UnionFind uni(V);
        long long res = 0;
        for (int i = 0; i < (int)G.size(); i++) {
            if (uni.unite(G[i].from, G[i].to)) {
                res += G[i].cost;
            }
        }
        return res;
    }
};

#endif
