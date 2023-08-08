#ifndef LITE_MONGE_DAG_HPP
#define LITE_MONGE_DAG_HPP

#include <functional>
#include <vector>

// https://noshi91.hatenablog.com/entry/2023/02/18/005856()
// N頂点のMongeコストDAGの最短距離(簡易版)
struct LiteMongeDAG {
private:
    std::function<long long(int,int)> cost;
    std::vector<int> amin;

    void check(int i,int j) {
        long long x = dist[j] + cost(j,i);
        if (x < dist[i]) {
            dist[i] = x;
            amin[i] = j;
        }
    }

    void solve(int l, int r) {
        if (r - l == 1) return;
        int m = (l + r) / 2;
        for (int k = amin[l]; k <= amin[r]; k++) check(m,k);
        solve(l,m);
        for (int k = l + 1; k <= m; k++) check(r,k);
        solve(m,r);
    }
public:
    std::vector<long long> dist;

    LiteMongeDAG(int n, const std::function<long long(int,int)> &cost) : cost(cost) {
        dist.resize(n, 1e18);
        dist[0] = 0;
        amin.resize(n, 0);
        check(n-1,0);
        solve(0,n-1);
    }
};

#endif