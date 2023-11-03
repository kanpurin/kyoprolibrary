#ifndef _PROJECT_SELECTION_PROBLEM_HPP_
#define _PROJECT_SELECTION_PROBLEM_HPP_

#include <vector>
#include <cassert>
#include "dinic.hpp"

struct ProjectSelectionProblem {
    const long long INF = 1e18+1;
    int n; // 要素数
    std::vector<int> k; // 選択肢数
    std::vector<int> s; // 要素を表す頂点の開始index
    std::vector<std::vector<long long>> cost;
    Dinic G;
    long long ans;
    
    // k:各要素の選択肢数
    ProjectSelectionProblem(std::vector<int> k) : k(k) {
        n = k.size();
        s = std::vector<int>(n+1);
        s[0] = 0;
        for (int i = 0; i < n; i++) {
            assert(k[i] >= 2);
            s[i+1] = s[i]+k[i]-1;
        }
        cost = std::vector<std::vector<long long>>(n);
        for (int i = 0; i < n; i++) cost[i] = std::vector<long long>(k[i],0);
        G = Dinic(s[n]+2);
        ans = 0;
    }

    inline void add_cost(long long cost) {
        ans += cost;
    }

    // aの選択のコスト
    void add_cost_1(int a,std::vector<long long> c) {
        assert(0 <= a && a < n);
        assert(k[a] == (int)c.size());
        for (int i = 0; i < k[a]; i++) {
            cost[a][i] += c[i];
        }
    }

    // x:abの選択間のコスト(Monge)
    void add_cost_2(int a,int b,std::vector<std::vector<long long>> c) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        assert((int)c.size() == k[a]);
        for (int i = 0; i < k[a]; i++) {
            assert((int)c[i].size() == k[b]);
            cost[a][i] += c[i][0];
            for (int j = k[b]-1; j >= 0; j--) c[i][j] -= c[i][0];
        }
        for (int i = 1; i < k[b]; i++){
            cost[b][i] += c[k[a]-1][i];
            for (int j = 0; j < k[a]; j++) c[j][i] -= c[k[a]-1][i];
        }
        for (int i = 0; i < k[a]-1; i++){
            for (int j = 0; j < k[b]-1; j++){
                long long _c = c[i+1][j]+c[i][j+1]-c[i][j]-c[i+1][j+1];
                assert(_c >= 0);
                if (_c == 0) continue;
                G.add_edge(s[b]+j, s[a]+i, _c);
            }
        }
    }

    // a1,a2,...,axの選択が(b1以上)∨(b2以上)∨...∨(bx以上)のとき
    // いずれかのiについてaiの選択siがbi<=siのときc(>=0)のコスト
    // 選択肢は0-indexed
    void add_cost_x_geq(std::vector<int> a, std::vector<int> b, long long c) {
        assert(a.size() == b.size());
        assert(c >= 0);
        if (c == 0) return;
        int x = a.size();
        for (int i = 0; i < x; i++) {
            if (b[i] <= 0) {
                add_cost(c);
                return;
            }
        }
        int sv = G.add_vertex();
        for (int i = 0; i < x; i++) {
            assert(0 <= a[i] && a[i] < n);
            if (b[i] >= k[i]) continue;
            G.add_edge(s[i]+b[i]-1,sv,c);
        }
        G.add_edge(sv,s[n]+1,c);
    }

    // a1,a2,...,axの選択が(b1以下)∨(b2以下)∨...∨(bx以下)のとき
    // いずれかのiについてaiの選択siがsi<=biのときc(>=0)のコスト
    // 選択肢は0-indexed
    void add_cost_x_leq(std::vector<int> a, std::vector<int> b, long long c) {
        assert(a.size() == b.size());
        assert(c >= 0);
        if (c == 0) return;
        int x = a.size();
        for (int i = 0; i < x; i++) {
            if (b[i] >= k[i]-1) {
                add_cost(c);
                return;
            }
        }
        int sv = G.add_vertex();
        for (int i = 0; i < x; i++) {
            assert(0 <= a[i] && a[i] < n);
            if (b[i] < 0) continue;
            G.add_edge(sv,s[i]+b[i],c);
        }
        G.add_edge(s[n],sv,c);
    }

    long long mincost() {
        for (int i = 0; i < n; i++){
            for (int j = 0; j < k[i]-2; j++) G.add_edge(s[i]+j+1, s[i]+j, INF);
            add_cost(cost[i][k[i]-1]);
            for (int j = 0; j < k[i]-1; j++){
                if (cost[i][j] > cost[i][j+1]) {
                    G.add_edge(s[n], s[i]+j, cost[i][j]-cost[i][j+1]);
                }
                else if (cost[i][j] < cost[i][j+1]) {
                    G.add_edge(s[i]+j, s[n]+1, cost[i][j+1]-cost[i][j]);
                    add_cost(-cost[i][j+1]+cost[i][j]);
                }
            }
        }
        return ans+G.max_flow(s[n], s[n]+1);
    }

    // 先にmincostが必要
    // 選択した選択肢
    std::vector<int> answer() {
        std::vector<int> slct(n);
        auto v = G.min_cnt(s[n],s[n]+1);
        std::vector<bool> scut(s[n]+2);
        for (int i = 0; i < (int)v.size(); i++) {
            if (v[i] > s[n]+1) continue;
            scut[v[i]] = true;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k[i]-1; j++) {
                if (!scut[s[i]+j]) { slct[i] = j; break; }
                else if (j == k[i]-2) { slct[i] = k[i]-1; }
            }
        }
        return slct;
    }
};

#endif
