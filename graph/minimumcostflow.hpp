#ifndef _MINIMUM_COST_FLOW_HPP_
#define _MINIMUM_COST_FLOW_HPP_

#include <vector>
#include <cassert>
#include <limits>
#include <queue>

// O(FElogV), 負辺有O(FElogV+EV),DAGならO(FElogV)，負閉路有は未定義
// verify:https://atcoder.jp/contests/past202107-open/tasks/past202107_m
template<typename Cap, typename Cost>
struct MinCostFlow {
protected:
    using P = std::pair<Cost,int>; // (最短距離, 頂点番号)
    struct edge {
        int to;
        Cap cap;
        Cost cost;
        int rev; // 逆辺のindex
        bool isrev;
    };
    std::vector<Cost> h;
    std::vector<Cost> dist;
    std::vector<int> prevv, preve; // 直前の頂点と辺
    bool negative; // 負辺があるか
public:
    int V; // 頂点数
    std::vector<std::vector<edge>> G; // グラフ
    Cost INF = std::numeric_limits<Cost>::max();
    MinCostFlow() {}
    MinCostFlow(int v) : h(v), dist(v), prevv(v), preve(v), negative(false), V(v), G(v) {}

    void add_edge(int from, int to, Cap cap, Cost cost) {
        assert(0 <= from && from < V);
        assert(0 <= to && to < V);
        assert(0 <= cap);
        if (cap == 0) return;
        if (cost < 0) negative = true;
        G[from].push_back({to,cap,cost,(int)G[to].size(),false});
        G[to].push_back({from,0,-cost,(int)G[from].size()-1,true});
    }

    Cost min_cost_flow(int s, int t, Cap f, bool DAG = false) {
        assert(0 <= s && s < V);
        assert(0 <= t && t < V);
        assert(s != t);
        assert(0 < f);
        Cost res = 0;
        fill(h.begin(), h.end(), 0);
        auto dijkstra = [&]() {
            std::priority_queue<P, std::vector<P>,std::greater<P>> pq;
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0; pq.push(P(0,s));
            while(!pq.empty()) {
                P p = pq.top(); pq.pop();
                int v = p.second;
                if (dist[v] < p.first) continue;
                for (int i = 0; i < (int)G[v].size(); i++) {
                    edge &e = G[v][i];
                    if (e.cap <= 0 || dist[e.to] <= dist[v] + e.cost + h[v] - h[e.to]) 
                        continue;
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v; preve[e.to] = i;
                    pq.push(P(dist[e.to],e.to));
                }
            }
        };

        // ポテンシャル計算
        if (DAG) {
            // DAG
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            std::vector<int> deg_in(V);
            for (int i = 0; i < V; i++) {
                for(const auto &e : G[i]) deg_in[e.to] += (e.cap > 0);
            }
            std::vector<int> st;
            for (int i = 0; i < V; i++) if (!deg_in[i]) st.push_back(i);
            for (int n = 0; n < V; n++) {
                if (int(st.size()) == n) assert(false); // DAGじゃない
                int now = st[n];
                for (int i = 0; i < (int)G[now].size(); i++) {
                    edge &e = G[now][i];
                    if (e.isrev) continue;
                    deg_in[e.to]--;
                    if (deg_in[e.to] == 0) st.push_back(e.to);
                    if (e.cap <= 0 || dist[now] == INF || dist[e.to] <= dist[now] + e.cost)
                        continue;
                    dist[e.to] = dist[now] + e.cost;
                    prevv[e.to] = now; preve[e.to] = i;
                }
            }
        }
        else if (!negative) dijkstra();
        else {
            // bellman-ford
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    for (int i = 0; i < (int)G[v].size(); i++) {
                        edge &e = G[v][i];
                        if (e.cap <= 0 || dist[v] == INF || dist[e.to] <= dist[v] + e.cost)
                            continue;
                        dist[e.to] = dist[v] + e.cost;
                        prevv[e.to] = v; preve[e.to] = i;
                        if (u == V - 1) assert(false); // 負閉路
                    }
                }
            }
        }

        while(true) {
            if (dist[t] == INF) return INF;
            for (int v = 0; v < V; v++) h[v] += dist[v];

            // s-t間最短路に沿って目一杯流す
            Cap d = f;
            for (int v = t; v != s; v = prevv[v]) {
                d = min(d,G[prevv[v]][preve[v]].cap);
            }

            f -= d; res += d * h[t];
            for (int v = t; v != s; v = prevv[v]) {
                edge &e = G[prevv[v]][preve[v]];
                e.cap -= d;
                G[v][e.rev].cap += d;
            }
            if (f == 0) break;

            dijkstra();
        }
        return res;
    }
};

#endif
