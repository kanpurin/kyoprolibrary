#ifndef _NIBU_JUDGE_H_
#define _NIBU_JUDGE_H_

#include <vector>
#include <stack>

// 二部グラフ判定
bool nibu_judge(const std::vector< std::vector< int > > &G) {
    int n = G.size();
    std::vector< int > mark(n,-1); // 0 or 1
    struct vertex { int v; int p; int m; };
    for (int i = 0; i < n; i++) {
        if (mark[i] != -1) continue;
        std::stack<vertex> sta;
        sta.push({i,-1,0}); // start is 0
        while (!sta.empty()) {
            vertex q = sta.top(); sta.pop();
            int v = q.v, p = q.p, m = q.m;
            if (mark[v] != -1) {
                if (mark[v] != m) return false;
                continue;
            }
            mark[v] = m;
            for (int u : G[v]) {
                if (u == p) continue;
                sta.push({u,v,1 - m});
            }
        }
    }
    return true;
}

#endif
