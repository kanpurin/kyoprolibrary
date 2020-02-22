#ifndef _LCA_DOUBLING_H_
#define _LCA_DOUBLING_H_

#include <vector>
#include <cmath>

// 最近共通祖先(Doubling)
// verify : https://onlinejudge.u-aizu.ac.jp/problems/GRL_5_C
// <O(NlogN),O(lonN)>
struct LowestCommonAncestor {
private:
    int n;
    int log;
    std::vector< std::vector< int > > parent;
    std::vector< int > dep;
    std::vector< std::vector< int > > G;
    //vector<ll> dis; // root からの距離
    void dfs(const std::vector< std::vector< int > > &G, int v, int p, int d) {
        parent[0][v] = p;
        dep[v] = d;
        //dis[v] = di;
        for (int to : G[v]) {
            if (to != p) dfs(G, to, v, d + 1);
            //if (e.to != p) dfs(G, e.to, v, d + 1,di + e.cost);
        }
    }
public:
    LowestCommonAncestor(int n) : n(n) {
        G.resize(n);
    }

    void add_edge(int from, int to) {
        G[from].push_back(to);
        G[to].push_back(from);
    }

    // add_edge後にする
    void build(int root = 0) {
        log = log2(n) + 1;
        parent.resize(log, std::vector< int >(n));
        dep.resize(n);
        //dis.resize(n);
        dfs(G, root, -1, 0);
        for (int k = 0; k + 1 < log; k++) {
            for (int v = 0; v < (int)G.size(); v++) {
                if (parent[k][v] < 0) {
                    parent[k + 1][v] = -1;
                }
                else {
                    parent[k + 1][v] = parent[k][parent[k][v]];
                }
            }
        }
    }

    int depth(int v) {
        return dep[v];
    }

    // O(logN)
    int lca(int u, int v) {
        if (dep[u] > dep[v]) std::swap(u, v);
        for (int k = 0; k < log; k++) if ((dep[v] - dep[u]) >> k & 1) v = parent[k][v];
        if (u == v) return u;
        for (int k = log - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

    int dist(int u, int v) {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
        //return dis[u] + dis[v] - 2 * dis[lca(u, v)];
    }
};

#endif
