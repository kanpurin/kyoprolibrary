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

    // aの選択のコスト
    void add_cost_1(int a,std::vector<long long> x) {
        assert(0 <= a && a < n);
        assert(k[a] == (int)x.size());
        for (int i = 0; i < k[a]; i++) {
            cost[a][i] += x[i];
        }
    }

    // x:abの選択間のコスト(Monge)
    void add_cost_2(int a,int b,std::vector<std::vector<long long>> x) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        assert((int)x.size() == k[a]);
        for (int i = 0; i < k[a]; i++) {
            assert((int)x[i].size() == k[b]);
            cost[a][i] += x[i][0];
            for (int j = k[b]-1; j >= 0; j--) x[i][j] -= x[i][0];
        }
        for (int i = 1; i < k[b]; i++){
            cost[b][i] += x[k[a]-1][i];
            for (int j = 0; j < k[a]; j++) x[j][i] -= x[k[a]-1][i];
        }
        for (int i = 0; i < k[a]-1; i++){
            for (int j = 0; j < k[b]-1; j++){
                long long c = x[i+1][j]+x[i][j+1]-x[i][j]-x[i+1][j+1];
                assert(c >= 0);
                if (c == 0) continue;
                G.add_edge(s[b]+j, s[a]+i, c);
            }
        }
    }

    long long mincost() {
        for (int i = 0; i < n; i++){
            for (int j = 0; j < k[i]-2; j++) G.add_edge(s[i]+j+1, s[i]+j, INF);
            ans += cost[i][k[i]-1];
            for (int j = 0; j < k[i]-1; j++){
                if (cost[i][j] > cost[i][j+1]) {
                    G.add_edge(s[n], s[i]+j, cost[i][j]-cost[i][j+1]);
                }
                else {
                    G.add_edge(s[i]+j, s[n]+1, cost[i][j+1]-cost[i][j]);
                    ans -= cost[i][j+1]-cost[i][j];
                }
            }
        }
        return ans+G.max_flow(s[n], s[n]+1);
    }
};

#endif
