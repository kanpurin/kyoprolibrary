#ifndef _DIAMETER_H_
#define _DIAMETER_H_

#include <vector>

// 木の直径
template< typename T >
struct Diameter {
private:
    struct edge {
        int to; T cost;
    };

    std::vector< std::vector< edge > > G;

    // (最遠点までのコスト,最遠点)
    std::pair< T, int > dfs(int v, int p) {
        std::pair< int, int > res = {0,v};
        for (const auto &e : G[v]) {
            if (e.to == p) continue;
            auto d = dfs(e.to, v);
            if (res.first < d.first + e.cost) {
                res.first = d.first + e.cost;
                res.second = d.second;
            }
        }
        return res;
    }
public:
    Diameter(int V) {
        G.resize(V);
    }

    // 無向辺を張る
    void add_edge(int v, int u, T cost) {
        G[v].push_back({u,cost});
        G[u].push_back({v,cost});
    }

    T build() {
        return dfs(dfs(0, -1).second, -1).first;
    }
};

#endif

int main() {
}
