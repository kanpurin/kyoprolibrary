#ifndef _WARSHALL_FLOYD_H_
#define _WARSHALL_FLOYD_H_

#include <vector>
#include <limits>

// ワーシャルフロイド法 O(V^3)
// verify : https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_C
template<typename T>
struct Warshall_floyd {
private:
    int V; // 頂点数
    std::vector< std::vector< T > > d; // i から j の最短距離
public:
    T inf = std::numeric_limits< T >::max();

    Warshall_floyd(int V) : V(V) {
        d.resize(V, std::vector< T >(V, inf));
        for (int i = 0; i < V; i++) {
            d[i][i] = 0;
        }
    }
    void add_edge(int from, int to, int cost, bool directed = false) {
        d[from][to] = cost;
        if (!directed) d[to][from] = cost;
    }

    void build() {
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (d[i][k] != inf && d[k][j] != inf) {
                        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                    }
                }
            }
        }
    }

    // from から to の距離
    // d[i][i] < 0 の時は負閉路
    T dist(int from, int to) {
        return d[from][to];
    }
};

#endif
