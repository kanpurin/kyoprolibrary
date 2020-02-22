#ifndef _DINIC_H_
#define _DINIC_H_

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

// 最大流O(V^2E)
struct Dinic {
private:
    struct edge {
        int to;
        long long cap;
        int rev;
        bool isrev;
        int idx;
    };

    std::vector< std::vector< edge > > graph;
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
                if (e.cap > 0 && min_cost[e.to] == -1) {
                    min_cost[e.to] = min_cost[p] + 1;
                    que.push(e.to);
                }
            }
        }
        return min_cost[t] != -1;
    }

    long long dfs(int idx, const int t, long long flow) {
        if (idx == t) return flow;
        for (int &i = iter[idx]; i < (int)graph[idx].size(); i++) {
            edge &e = graph[idx][i];
            if (e.cap > 0 && min_cost[idx] < min_cost[e.to]) {
                long long d = dfs(e.to, t, std::min(flow, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
public:

    Dinic(int V) : graph(V) {}

    // from から to への容量capの有向辺
    void add_edge(int from, int to, long long cap, int idx = -1) {
        graph[from].push_back({to, cap, (int)graph[to].size(), false, idx});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1, true, idx});
    }

    // from s to t
    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            iter.assign(graph.size(), 0);
            long long f = 0;
            while ((f = dfs(s, t, 1e9 + 6)) > 0) flow += f;
        }
        return flow;
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
