#ifndef _HEAVY_LIGHT_DECOMPOSITION_H_
#define _HEAVY_LIGHT_DECOMPOSITION_H_

#include <cassert>
#include <vector>

// HL分解
struct HeavyLightDecomposition {
private:
    std::vector<std::vector<int>> g;
    std::vector<int> vid,inv,head,sz,dep,par;
    bool built = false;
    void dfs1(int x) {
        for (auto &y : g[x]) {
            if (y != par[x]) continue;
            std::swap(y, g[x].back());
            g[x].pop_back();
            break;
        }
        for (auto &y : g[x]) {
            par[y] = x;
            dep[y] = dep[x] + 1;
            dfs1(y);
            sz[x] += sz[y];
            if (sz[y] > sz[g[x][0]]) std::swap(y, g[x][0]);
        }
    }
    void dfs2(int x, int &id) {
        vid[x] = id++;
        for (auto y : g[x]) {
            head[y] = (y == g[x][0] ? head[x] : y);
            dfs2(y,id);
        }
    }
    std::vector<std::pair<int,int>> path(int x, int y, bool with_lca) const {
        assert(built);
        std::vector<std::pair<int,int>> ret;
        while (1) {
            if (vid[x] > vid[y]) std::swap(x,y);
            if (head[x] == head[y]) {
                ret.emplace_back(vid[x] + (with_lca ? 0 : 1), vid[y]+1);
                return ret;
            } else {
                ret.emplace_back(vid[head[y]], vid[y]+1);
                y = par[head[y]];
            }
        }
    }
public:
    HeavyLightDecomposition(int n) : g(n), vid(n),inv(n),head(n),sz(n,1),dep(n,0),par(n,-1) {}
    void add_edge(int x, int y) {
        g[x].push_back(y);
        g[y].push_back(x);
        built = false;
    }
    void build(int root = 0) {
        dfs1(root);
        head[root] = root;
        int id = 0;
        dfs2(root,id);
        for (int x = 0; x < (int)g.size(); x++) inv[vid[x]] = x;
        built = true;
    }
    int lca(int x, int y) const {
        assert(built);
        while (1) {
            if (vid[x] > vid[y]) std::swap(x,y);
            if (head[x] == head[y]) return x;
            y = par[head[y]];
        }
    }
    int dist(int x, int y) const {
        assert(built);
        return dep[x] + dep[y] - 2*dep[lca(x,y)];
    }
    // 辺のid O(1)
    int edge_id(int x, int y) const {
        assert(built);
        if (x == par[y]) return vid[y];
        if (y == par[x]) return vid[x];
        return -1;
    }
    // 頂点のid O(1)
    int vertex_id(int x) const {
        assert(built);
        return vid[x];
    }
    // パスx-y上の頂点を表す区間[l,r)のリストを返す O(logN)
    // x=yならO(1)
    std::vector<std::pair<int,int>> vertices(int x, int y) const {
        return path(x,y,true);
    }
    // パスx-y上の辺を表す区間[l,r)のリストを返す O(logN)
    std::vector<std::pair<int,int>> edges(int x, int y) const {
        return path(x,y,false);
    }
};

#endif