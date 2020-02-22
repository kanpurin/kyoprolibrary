#ifndef _LOWLINK_H_
#define _LOWLINK_H_

#include <vector>
#include <algorithm>

// 橋・関節点列挙 O(V)
struct LowLink {
private:
    std::vector< std::vector< int > > G;
    int V; // 頂点数
    std::vector< int > used, ord, low;

    int dfs(int idx, int k, int par) {
        used[idx] = true;
        ord[idx] = k++;
        low[idx] = ord[idx];
        bool is_articulation = false;
        int cnt = 0;
        for (auto &to : G[idx]) {
            if (!used[to]) {
                ++cnt;
                k = dfs(to, k, idx);
                low[idx] = std::min(low[idx], low[to]);
                is_articulation |= ~par && low[to] >= ord[idx];
                if (ord[idx] < low[to]) bridge.push_back(std::minmax(idx, (int)to));
            }
            else if (to != par) {
                low[idx] = std::min(low[idx], ord[to]);
            }
        }
        is_articulation |= par == -1 && cnt > 1;
        if (is_articulation) articulation.push_back(idx);
        return k;
    }

public:
    std::vector< int > articulation; // 関節点
    std::vector< std::pair< int, int > > bridge; // 橋

    LowLink(int V) : V(V) {
        G.resize(V);
    }

    // 無向辺を張る
    void add_edge(int v, int u) {
        G[v].push_back(u);
        G[u].push_back(v);
    }

    void build() {
        used.assign(V, 0);
        ord.assign(V, 0);
        low.assign(V, 0);
        int k = 0;
        for (int i = 0; i < V; i++) {
            if (!used[i]) k = dfs(i, k, -1);
        }
    }
};

#endif
