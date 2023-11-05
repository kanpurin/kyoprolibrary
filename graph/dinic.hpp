#ifndef _DINIC_HPP_
#define _DINIC_HPP_

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

// 最大流O(V^2E) 実際もっと早い
// 容量全部同じO(E√E)
struct Dinic {
private:
    struct Edge {
        int to;
        long long cap;
        int rev;
        bool isrev;
    };

    std::vector< std::vector< Edge > > graph;
    std::vector< int > min_cost, iter;

    bool bfs(int s, int t) {
        min_cost.assign(graph.size(), -1);
        std::queue< int > que;
        min_cost[s] = 0;
        que.push(s);
        while (!que.empty() && min_cost[t] == -1) {
            int p = que.front();
            que.pop();
            for (auto &e : graph[p]) {
                if (e.cap == 0 || min_cost[e.to] != -1) continue;
                min_cost[e.to] = min_cost[p] + 1;
                que.push(e.to);
            }
        }
        return min_cost[t] != -1;
    }

    long long dfs(const int s, int v, long long flow) {
        if (v == s) return flow;
        long long res = 0;
        int min_cost_v = min_cost[v];
        for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
            Edge &e = graph[v][i];
            if (graph[e.to][e.rev].cap == 0 || min_cost_v <= min_cost[e.to]) continue;
            long long d = dfs(s, e.to, std::min(flow-res, graph[e.to][e.rev].cap));
            if (d <= 0) continue;
            e.cap += d;
            graph[e.to][e.rev].cap -= d;
            res += d;
            if (res == flow) return res;
        }
        min_cost[v] = (int)graph.size();
        return res;
    }
public:

    Dinic() {}
    Dinic(int V) : graph(V) {}

    // from から to への容量capの有向辺
    void add_edge(int from, int to, long long cap) {
        graph[from].push_back({to, cap, (int)graph[to].size(), false});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1, true});
    }

    // 頂点を追加
    // indexを返す
    int add_vertex() {
        graph.push_back(std::vector< Edge >());
        return graph.size()-1;
    }

    long long max_flow(int s, int t, long long flow_limit = 1e18 + 6) {
        long long flow = 0;
        while (bfs(s, t)) {
            iter.assign(graph.size(), 0);
            long long f = dfs(s, t, flow_limit-flow);
            if (!f) break;
            flow += f;
        }
        return flow;
    }

    // 先にmax_flowが必要
    // s側カットを出力
    std::vector< int > min_cnt(int s,int t) {
        std::vector< bool > visited(graph.size(),false);
        std::vector< int > ret;
        std::stack< int > sta;
        visited[s] = true;
        sta.push(s); ret.push_back(s);
        while(!sta.empty()) {
            int v = sta.top(); sta.pop();
            for (auto &e : graph[v]) {
                if (visited[e.to] || e.cap == 0) continue;
                visited[e.to] = true;
                sta.push(e.to);
                ret.push_back(e.to);
            }
        }
        return ret;
    }

    void output() {
        for (int i = 0; i < (int)graph.size(); i++) {
            for (auto &e : graph[i]) {
                if (e.isrev) continue;
                auto &rev_e = graph[e.to][e.rev];
                std::cout << i << "->" << e.to << " (flow: " << rev_e.cap << "/" << e.cap + rev_e.cap << ")" << std::endl;
            }
        }
    }
};

#endif
