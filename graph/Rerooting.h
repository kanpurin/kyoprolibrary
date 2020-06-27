#ifndef _REROOTING_H_
#define _REROOTING_H_

#include <vector>
#include <algorithm>

// [問題設定]
// それぞれ1からNまでの番号が付いたN個の頂点が、N-1本の無向辺によって繋がれたグラフが与えられる。
// 各頂点について、その頂点からスタートしてすべての頂点を訪れるための最短のステップ数を出力せよ。
//
// [解法]
// 各頂点からの最遠頂点までの距離を辺の数の2倍から引く
// 全方位木DPで解く

using T = long long; // データ型
int n; // 頂点
std::vector< std::vector< int > > tree;
std::vector< T > dist; // 根付き木中の i の
std::vector< T > ans;

void dfs1(int v, int p = -1) {
    T res = 0;
    for (auto &u : tree[v]) {
        if (u == p) continue;
        dfs1(u, v);
        res = std::max(res, dist[u] + 1);
    }
    dist[v] = res;
}

void dfs2(int v, T d_par, int p = -1) {
    // v における答えの計算
    ans[v] = std::max(d_par + 1, dist[v]);

    // 左右からの累積演算を用いて子に送る d_par の計算
    // lf[i] : [:i-1]の演算結果
    // ri[i] : [i+1:]の演算結果
    auto f = [](T a, T b) { return std::max(a, b); }; // 演算
    int e = -1; // 演算の単位元
    std::vector< T > lf(tree[v].size(), e), ri(tree[v].size(), e); // 累積配列
    for (int i = 1; i < (int)tree[v].size(); i++) {
        if (tree[v][i - 1] == p) lf[i] = f(d_par, lf[i - 1]);
        else lf[i] = f(dist[tree[v][i - 1]], lf[i - 1]);
    }
    for (int i = tree[v].size() - 2; i >= 0; i--) {
        if (tree[v][i + 1] == p) ri[i] = f(d_par, ri[i + 1]);
        else ri[i] = f(dist[tree[v][i + 1]], ri[i + 1]);
    }
    for (int i = 0; i < (int)tree[v].size(); i++) {
        if (tree[v][i] == p) continue;
        dfs2(tree[v][i], f(lf[i], ri[i]) + 1, v);
    }
}

/*
int main() {
    cin >> n;
    tree.resize(n);
    dist.resize(n);
    ans.resize(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        u--; v--;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
    dfs1(0);
    dfs2(0, 0);
    for (int i = 0; i < n; i++) {
        cout << 2 * (n - 1) - ans[i] << endl;
    }
    return 0;
}
*/

#endif
