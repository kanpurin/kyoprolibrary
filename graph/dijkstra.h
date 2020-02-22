#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

// dijkstra O(ElogV)
// verify : https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_A
template<typename T>
struct Dijkstra {
private:
    int V;
    struct edge { int to; T cost; };
    vector<vector<edge>> G;
public:
    const T inf = numeric_limits<T>::max();

    // s から i の最小コスト
    // 経路がない場合は inf
    vector<T> d;

    Dijkstra(int V) : V(V){
        G.resize(V);
    }

    // 辺の追加
    // 有向の場合　directed = true
    void add_edge(int from, int to, T weight, bool directed = false) {
        G[from].push_back({to,weight});
        if (!directed) G[to].push_back({from,weight});
    }

    void build(int s) {
        d.assign(V, inf);
        typedef pair<T, int> P; //(cost,v)
        priority_queue<P, vector<P>, greater<P>> pq;
        d[s] = 0;
        pq.push(P(d[s], s));

        while (!pq.empty()) {
            P p = pq.top(); pq.pop();
            int v = p.second;
            if (d[v] < p.first) continue;
            for (const edge &e : G[v])
            {
                // 最小値が更新されるとき push
                if (d[e.to] > d[v] + e.cost) {
                    d[e.to] = d[v] + e.cost;
                    pq.push({d[e.to], e.to});
                }
            }
        }
    }
};

#endif
