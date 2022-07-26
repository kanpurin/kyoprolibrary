#ifndef _PUSH_RELABEL_HPP_
#define _PUSH_RELABEL_HPP_

#include <vector>
#include <queue>
#include <algorithm>

// O(V^2√E)
// FREEZE:復元できない(freeze nodeから超過量を戻せば復元できる)
struct PushRelabel {
private:
    bool FREEZE;
    int n;
    struct Edge {
        int to;
        long long cap;
        int rev;
        bool isrev;
    };
    std::vector<int> hs, ds; // hs:高さ ds:高さiの頂点数
    std::vector<long long> fs; // 超過流量
    std::vector<bool> active;
    std::vector<std::vector<int>> bs; // 高さiの頂点集合
    int cur; // 現在の最大の高さ(<n)

    // Global labeling
    inline int _bfs(int t) {
        fill(hs.begin(), hs.end(), n);
        fill(ds.begin(), ds.end(), 0);
        fill(fs.begin(), fs.end(), 0LL);
        fill(active.begin(), active.end(), false);
        for_each(bs.begin(), bs.end(), [](std::vector<int> &b){ b.clear(); });
        std::queue<int> que; que.push(t);
        hs[t] = 0;
        cur = 0;
        int d = 0;
        while(!que.empty()) {
            int v = que.front(); que.pop();
            d = hs[v];
            for(Edge &e : graph[v]) {
                int u = e.to;
                if(hs[u] <= d + 1 || graph[u][e.rev].cap == 0) continue;
                hs[u] = d + 1;
                if(d+1 < n) ++ds[d+1];
                que.push(u);
            }
        }
        return d+1;
    }

    // vがactiveでなくなったらtrue
    bool _push(int v, int gap) {
        for(Edge &e : graph[v]) {
            int u = e.to;
            if(e.cap == 0 || hs[v] <= hs[u] || (FREEZE && hs[u] >= gap)) continue;
            long long df = std::min(fs[v], e.cap); // 流量
            e.cap -= df;
            graph[u][e.rev].cap += df;
            fs[v] -= df;
            fs[u] += df;
            if(!active[u]) {
                bs[hs[u]].push_back(u);
                cur = std::max(cur,hs[u]);
                active[u] = true;
            }
            if(fs[v] == 0) {
                active[v] = false;
                return true;
            }
        }
        return false;
    }

    void _relabel(int v,int &gap) {
        int old_hv = hs[v];
        hs[v] = (FREEZE ? n : 2*n);
        for(Edge &e : graph[v]) {
            int u = e.to;
            if(e.cap == 0 || (FREEZE && hs[u] >= gap)) continue;
            hs[v] = std::min(hs[v],hs[u] + 1);
        }

        --ds[old_hv];
        if(ds[old_hv] == 0 && old_hv < gap) {
            gap = old_hv;
            hs[v] = n;
        } 
        else if(hs[v] == gap) {
            ++gap;
        }
        if(!FREEZE || hs[v] < n) {
            ++ds[hs[v]];
            bs[hs[v]].push_back(v);
            cur = std::max(cur,hs[v]);
        } 
        else {
            active[v] = false;
        }
    }

public:
    std::vector<std::vector<Edge>> graph;
    PushRelabel() {}
    PushRelabel(int n, bool FREEZE = false) : graph(n),
                                              hs(n), 
                                              ds(2*n), 
                                              fs(n), 
                                              active(n), 
                                              bs(2*n),
                                              n(n),
                                              FREEZE(FREEZE) {}

    inline void add_edge(int from, int to, long long cap) {
        graph[from].emplace_back(Edge{to, cap, (int) graph[to].size(),false});
        graph[to].emplace_back(Edge{from, 0, (int) graph[from].size()-1,true});
    }

    long long max_flow(int s, int t) {
        // 初期化
        int gap = _bfs(t);
        if (hs[s] == n) return 0;
        hs[s] = n;

        // sから流せるだけ流す
        for(Edge &e : graph[s]) {
            int u = e.to;
            long long df =  e.cap; // 流量
            if (e.cap == 0) continue;
            e.cap -= df;
            graph[u][e.rev].cap += df;
            fs[u] += df;
            bs[hs[u]].push_back(u);
            cur = std::max(cur,hs[u]);
            active[u] = true;
        }

        while(true) {
            while(cur >= 0 && bs[cur].size() == 0) --cur; // cur更新
            if(cur < 0) break; // active node なし

            int v = bs[cur].back(); bs[cur].pop_back();
            if(v == s || v == t || hs[v] != cur) continue;

            // Gap-relabeling
            if(gap < hs[v] && hs[v] < n) {
                --ds[hs[v]];
                hs[v] = n;
                ++ds[hs[v]];
                bs[hs[v]].push_back(v);
                cur = std::max(cur,hs[v]);
                continue;
            }
            // push
            if (_push(v,gap)) continue;
            // relabel
            _relabel(v,gap);

        }
        return fs[t];
    }
};

#endif
