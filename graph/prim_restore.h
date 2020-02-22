#ifndef _PRIM_RESTORE_H_
#define _PRIM_RESTORE_H_

#include <vector>
#include <queue>

// 最小全域木 O(ElogV)?
// 辺復元
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
    std::vector< std::vector< int > > connected; // [i][j] : iからのj番目の辺、[i].size()=次数

    Prim(int V) : V(V) {
        connected.resize(V);
        G.resize(V);
    }

    void add_edge(int from, int to, T weight) {
        G[from].push_back({ weight, to });
        G[to].push_back({ weight, from });
    }

    T build() {
        T res = 0;
        std::vector< bool > used(V, false);
        typedef std::pair< Edge, int > P;
        std::priority_queue< P, std::vector< P >, std::greater< P > > pq;
        for (const Edge &e : G[0]) {
            pq.push({e,0});
        }
        used[0] = true;
        while (!pq.empty()) {
            auto ee = pq.top(); pq.pop();
            auto e = ee.first;
            auto par = ee.second; //parent
            if (used[e.to]) continue;
            used[e.to] = true;
            connected[e.to].push_back(par);
            connected[par].push_back(e.to);
            res += e.cost;
            for (const Edge &u : G[e.to]) {
                if (used[u.to]) continue;
                pq.push({ u, e.to });
            }
        }
        return res;
    }
};

#endif
