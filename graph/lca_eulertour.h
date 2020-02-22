#ifndef _LCA_EULER_TOUR_H_
#define _LCA_EULER_TOUR_H_

#include <vector>
#include <algorithm>
#include "segmenttree.h"

// 最近共通祖先(EulerTour)
// verify : https://onlinejudge.u-aizu.ac.jp/problems/GRL_5_C
// <O(N),O(lonN)>
struct LowestCommonAncestor {
private:
    int V;
    const int INF = 1e9 + 6;
    std::vector< int > ord, dep, id;
    SegmentTree< std::pair< int, int > > st; // (value,index)
    void dfs(int u, int p, int k, const std::vector< std::vector< int > > &Tree) {
        id[u] = (int)ord.size();
        ord.push_back(u);
        dep[u] = k;
        for (auto v : Tree[u]) {
            if (v != p) {
                dfs(v, u, k + 1, Tree);
                ord.push_back(u);
            }
        }
    }

public:

    // Tree以外を与えるとバグる
    LowestCommonAncestor(const std::vector< std::vector< int > > &G, int root = 0) {
        build(G, root);
    }

    void build(const std::vector< std::vector< int > > &G, int root = 0) {
        V = G.size();
        dep.resize(V);
        id.resize(V, -1);
        ord.reserve(2 * V - 2);
        dfs(root, -1, 0, G);
        st = SegmentTree< std::pair< int, int > >(2 * V - 2, { INF, -1 }, [](std::pair< int, int > a, std::pair< int, int > b) { return std::min(a, b); }, { INF, -1 });
        for (int i = 0; i < 2 * V - 2; i++) {
            st.set(i, { dep[ord[i]], i });
        }
        st.build();
    }

    // u, vのlca O(logN)
    int lca(int u, int v) {
        return ord[st.get_query(std::min(id[u], id[v]), std::max(id[u], id[v]) + 1).second];
    }

    // u, vの距離 O(logN)
    int dist(int u, int v) {
        int lca_ = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[lca_];
    }

    int depth(int v) {
        return dep[v];
    }
};

#endif
