#ifndef _PRIM_H_
#define _PRIM_H_

#include <vector>
#include <queue>

// 最小全域木 O(ElogV)?
// verify : https://onlinejudge.u-aizu.ac.jp/problems/GRL_2_A
template< typename T >
struct Prim {
private:
    struct Edge{
        T cost;
        int to;
        bool operator>(const Edge e)const {
            return this->cost > e.cost;
        }
        bool operator<(const Edge e)const {
            return this->cost < e.cost;
        }
    };
    int V; // 頂点数
    std::vector< std::vector< Edge > > G;
public:
    Prim(int V) : V(V) {
        G.resize(V);
    }

    void add_edge(int from, int to, T weight) {
        G[from].push_back({ weight, to });
        G[to].push_back({ weight, from });
    }

    T build() {
        T res = 0;
        std::vector< bool > used(V, false);
        std::priority_queue< Edge, std::vector< Edge >, std::greater< Edge > > pq;
        for (const Edge &e : G[0]) {
            pq.push(e);
        }
        used[0] = true;
        while (!pq.empty()) {
            Edge e = pq.top(); pq.pop();
            if (used[e.to]) continue;
            used[e.to] = true;
            res += e.cost;
            for (const Edge &u : G[e.to]) {
                if (used[u.to]) continue;
                pq.push(u);
            }
        }
        return res;
    }
};

#endif
