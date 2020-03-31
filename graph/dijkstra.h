#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include <vector>
#include <limits>
#include <utility>
#include <queue>
#include <tuple>

// dijkstra O(ElogV)
// verify : https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A
// ※拡張ダイクストラ
template<typename T>
struct Dijkstra {
private:
    int V;
    struct edge { int to; T cost; };
    std::vector< std::vector< edge > > G;
public:
    const T inf = std::numeric_limits< T >::max();

    // s から i の最小コスト
    // 経路がない場合は inf
    std::vector< T > d; // (頂点) ※

    Dijkstra(int V) : V(V){
        G.resize(V);
    }

    // 辺の追加
    // 有向の場合 directed = true
    void add_edge(int from, int to, T weight, bool directed = false) {
        G[from].push_back({to,weight});
        if (!directed) G[to].push_back({from,weight});
    }

    void build(int s) {
        d.assign(V, inf); // ※
        typedef std::tuple< T, int > P; // (距離,頂点) ※
        std::priority_queue< P, std::vector< P >, std::greater< P > > pq;
        d[s] = 0; // ※
        pq.push(P(d[s], s)); // ※
        while (!pq.empty()) {
            P p = pq.top(); pq.pop();
            int v = std::get<1>(p);
            // ※
            if (d[v] < std::get<0>(p)) continue; // ※
            for (const edge &e : G[v])
            {
                // ※
                if (d[e.to] > d[v] + e.cost) {
                    d[e.to] = d[v] + e.cost;
                    pq.push(P(d[e.to], e.to));
                }
            }
        }
    }
};

#endif
