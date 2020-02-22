#ifndef _BELLMAN_FORD_H_
#define _BELLMAN_FORD_H_

#include <vector>
#include <limits>

//bellman_ford O(VE)
// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_1_B
template< typename T >
struct Bellman_Ford {
private:
    struct edge { int to; T cost; };
    int V; // 頂点数
    std::vector< std::vector< edge > > G;
    std::vector< T > d;
public:
    const T inf = std::numeric_limits< T >::max();

    Bellman_Ford(int V) : V(V){
        G.resize(V);
        d.resize(V);
    }

    // 有向辺を張る
    void add_edge(int from, int to, T cost) {
        G[from].push_back({to,cost});
    }

    // 頂点 s からの最短コストを d に入れる
    // sからたどり着く負閉路がある場合 False
    bool build(int s) {
        d.assign(V,inf);
        d[s] = 0;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                for (edge &e : G[j]) {
                    if (d[j] != inf && d[j] + e.cost < d[e.to]) {
                        d[e.to] = d[j] + e.cost;
                        if (i == V - 1) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    // s から v までの最短コスト
    // 行けない場合は inf
    T dist(int v) {
        return d[v];
    }
};

#endif
