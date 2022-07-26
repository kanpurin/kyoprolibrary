#ifndef _DINIC_CAPACITY_SCALING_HPP_
#define _DINIC_CAPACITY_SCALING_HPP_

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

// 最大流O(VElogU) 
struct DinicCapacityScaling {
private:
    struct Edge {
        int to;
        long long cap;
        int rev;
        bool isrev;
    };

    std::vector< std::vector< Edge > > graph;
    std::vector< int > min_cost, iter;
    long long max_cap;

    bool bfs(int s, int t, long long base) {
        min_cost.assign(graph.size(), -1);
        std::queue< int > que;
        min_cost[s] = 0;
        que.push(s);
        while (!que.empty() && min_cost[t] == -1) {
            int p = que.front();
            que.pop();
            for (auto &e : graph[p]) {
                if (e.cap < base || min_cost[e.to] != -1) continue;
                min_cost[e.to] = min_cost[p] + 1;
                que.push(e.to);
            }
        }
        return min_cost[t] != -1;
    }

    long long dfs(const int s, int v, long long base, long long flow) {
        if (v == s) return flow;
        long long res = 0;
        int min_cost_v = min_cost[v];
        for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
            Edge &e = graph[v][i];
            if (graph[e.to][e.rev].cap < base || 
                min_cost_v <= min_cost[e.to]) continue;
            long long d = dfs(s, e.to, base, std::min(flow-res, graph[e.to][e.rev].cap));
            if (d <= 0) continue;
            e.cap += d;
            graph[e.to][e.rev].cap -= d;
            res += d;
            if (flow - res < base) return res;
        }
        min_cost[v] = (int)graph.size();
        return res;
    }
public:

    DinicCapacityScaling() {}
    DinicCapacityScaling(int V) : graph(V), max_cap(0) {}

    // from から to への容量capの有向辺
    void add_edge(int from, int to, long long cap) {
        max_cap = std::max(max_cap, cap);
        graph[from].push_back({to, cap, (int)graph[to].size(), false});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1, true});
    }

    long long max_flow(int s, int t, long long flow_limit = 1e18 + 6) {
        long long flow = 0;
        for (int i = 63 - __builtin_clzll(max_cap); i >= 0; i--) {
            long long now = 1LL << i;
            while (bfs(s, t, now)) {
                iter.assign(graph.size(), 0);
                long long f = dfs(s, t, now, flow_limit-flow);
                if (!f) break;
                flow += f;
            }
        }
        return flow;
    }

    // 先にmax_flowが必要
    // s側カットを出力
    std::vector<int> min_cnt(int s,int t) {
        std::vector<bool> visited(graph.size());
        std::vector<int> ret;
        std::stack<int> sta;
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