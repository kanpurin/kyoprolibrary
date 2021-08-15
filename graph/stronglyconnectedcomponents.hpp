#ifndef _STRONGLY_CONNECTED_COMPONENTS_HPP_
#define _STRONGLY_CONNECTED_COMPONENTS_HPP_

#include <vector>
#include <algorithm>

// 強連結成分分解 O(N+M)
// verify : https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_C
struct StronglyConnectedComponents {
private:
    std::vector< std::vector< int > > g, rg;
    std::vector< int > order; // dfsの順
    std::vector< bool > visited;
    int V;

    void dfs(int v) {
        if (visited[v]) return;
        visited[v] = true;
        for (int u : g[v]) dfs(u);
        order.push_back(v);
    }

    void rdfs(int v, int c) {
        if (comp[v] != -1) return;
        comp[v] = c;
        for (int u : rg[v]) rdfs(u, c);
    }

public:
    std::vector< std::vector< int > > Graph; // DAG(多重辺アリ)
    std::vector< int > comp; // comp[i] : iが属する成分

    StronglyConnectedComponents(int v) : V(v) {
        g.resize(v);
        rg.resize(v);
        visited.resize(v);
        comp.resize(v);
    }

    void add_vertex(int n = 1) {
        V += n;
        g.resize(V);
        rg.resize(V);
        visited.resize(V);
        comp.resize(V);
    }

    void add_edge(int from, int to) {
        g[from].push_back(to);
        rg[to].push_back(from);
    }

    void build() {
        Graph = std::vector< std::vector< int > >();
        order = std::vector< int >();
        visited.assign(V, false);
        comp.assign(V, -1);
        for (int i = 0; i < V; i++) dfs(i);
        std::reverse(order.begin(), order.end());
        int number = 0;
        for (int i = 0; i < V; i++) {
            if (comp[order[i]] == -1) {
                rdfs(order[i], number++);
            }
        }
        Graph.resize(number);
        for (int i = 0; i < V; i++) {
            for (int v : g[i]) {
                if (comp[i] == comp[v]) continue;
                Graph[comp[i]].push_back(comp[v]);
            }
        }
    }
};

#endif
