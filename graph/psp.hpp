#ifndef _PROJECT_SELECTION_PROBLEM_HPP_
#define _PROJECT_SELECTION_PROBLEM_HPP_

#include <vector>
#include <cassert>
#include "dinic.hpp"

struct ProjectSelectionProblem {
    const long long INF = std::numeric_limits<long long>::max();
    int n; // 要素数
    std::vector<int> k; // 選択肢数
    std::vector<int> s; // 要素を表す頂点の開始index
    std::vector<std::vector<long long>> cost;
    Dinic G;
    long long ans;
private:
    void add_select_cost(int a, int i, long long c) {
        if (cost[a][i] == INF) return;
        if (c == INF) cost[a][i] = c;
        else cost[a][i] += c;
    }
public:
    
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

    inline void add_cost(long long c) {
        assert(c != INF);
        ans += c;
    }

    // aの選択のコスト
    void add_cost_1(int a,std::vector<long long> c) {
        assert(0 <= a && a < n);
        assert(k[a] == (int)c.size());
        for (int i = 0; i < k[a]; i++) {
            add_select_cost(a,i,c[i]);
        }
    }

    // x:abの選択間のコスト(Monge)
    void add_cost_2(int a,int b,std::vector<std::vector<long long>> c) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        assert(a != b);
        assert((int)c.size() == k[a]);
        for (int i = 0; i < k[a]; i++) assert((int)c[i].size() == k[b]);

        // INFの処理
        {
            // INF階段作成
            std::vector<std::vector<long long>> c1=c, c2=c;
            for (int i = 0; i < k[a]; i++) {
                for (int j = 0; j < k[b]; j++) {
                    if (j > 0) c1[i][j] = c1[i][j]==INF&&c1[i][j-1]==INF?INF:0;
                    if (i > 0) c2[i][j] = c2[i][j]==INF&&c2[i-1][j]==INF?INF:0;
                }
            }
            for (int i = k[a]-1; i >= 0; i--) {
                for (int j = k[b]-1; j >= 0; j--) {
                    if (i < k[a]-1) c1[i][j] = c1[i][j]==INF&&c1[i+1][j]==INF?INF:0;
                    if (j < k[b]-1) c2[i][j] = c2[i][j]==INF&&c2[i][j+1]==INF?INF:0;
                    // 作成不可能
                    assert(c[i][j]!=INF || c1[i][j]==INF || c2[i][j]==INF);
                }
            }

            // 1行/1列すべてINFの処理
            for (int i = 0; i < k[a]; i++) {
                if (c1[i][k[b]-1]==INF) cost[a][i] = INF;
                if (c2[i][0]==INF) cost[a][i] = INF;
            }
            for (int j = 0; j < k[b]; j++) {
                if (c1[0][j]==INF) cost[b][j] = INF;
                if (c2[k[a]-1][j]==INF) cost[b][j] = INF;
            }

            // INFの辺を張る
            for (int i = 1; i < k[a]; i++) {
                for (int j = 0; j < k[b]-1; j++) {
                    if (c1[i][j]!=INF||c1[i-1][j]==INF||c1[i][j+1]==INF) continue;
                    G.add_edge(s[a]+i-1,s[b]+j,INF);
                }
            }
            for (int i = 0; i < k[a]-1; i++) {
                for (int j = 1; j < k[b]; j++) {
                    if (c2[i][j]!=INF||c2[i+1][j]==INF||c2[i][j-1]==INF) continue;
                    G.add_edge(s[b]+j-1,s[a]+i,INF);
                }
            }

            // mongeとなるようにINFを変更する
            std::vector<std::vector<long long>> c3(k[a]+2,
                                                   std::vector<long long>(k[b]+2,INF));
            for (int i = 0; i < k[a]; i++)
                for (int j = 0; j < k[b]; j++)
                    c3[i+1][j+1] = c[i][j];
            c3[0][0] = 0;
            c3[k[a]+1][k[b]+1] = 0;
            bool pre = true;
            int px = 0,py = 0;
            for (int i = px+1; i <= k[a]+1; i++) {
                for (int j = py+1; j <= k[b]+1; j++) {
                    if (pre&&c3[i][j]!=INF&&c3[i-1][j]==INF&&c3[i][j-1]==INF) {
                        for (int x = px+1; x < i; x++)  c3[x][py] = 0;
                        for (int y = py+1; y <= j; y++) c3[px][y] = 0;
                        for (int x = px+1; x < i; x++)
                            for (int y = py+1; y <= j; y++)
                                c3[x][y] = -c3[px][py];
                        pre = false;
                    }
                    if (i==k[a]+1||j==k[b]+1) continue;
                    if (!pre&&c3[i][j]!=INF&&c3[i+1][j]==INF&&c3[i][j+1]==INF) {
                        px = i, py = j, i++, pre = true;
                    }
                }
            }
            for (int h = 1-k[b]; h < k[a]; h++) {
                for (int i = std::max(1,1+h), j = i-h; i<k[a]&&j<k[b]; i++,j++) {
                    if (c3[i][j]==INF||c3[i+1][j]!=INF||
                        c3[i][j+1]==INF||c3[i+1][j+1]==INF) continue;
                    c3[i+1][j] = c3[i][j]-c3[i][j+1]+c3[i+1][j+1];
                }
            }
            for (int h = 1-k[a]; h < k[b]; h++) {
                for (int j = std::max(1,1+h), i = j-h; i<k[a]&&j<k[b]; i++,j++) {
                    if (c3[i][j]==INF||c3[i+1][j]==INF||
                        c3[i][j+1]!=INF||c3[i+1][j+1]==INF) continue;
                    c3[i][j+1] = c3[i][j]-c3[i+1][j]+c3[i+1][j+1];
                }
            }
            for (int i = 0; i < k[a]; i++)
                for (int j = 0; j < k[b]; j++)
                    c[i][j] = c3[i+1][j+1];
        }

        for (int i = 0; i < k[a]; i++) {
            add_select_cost(a,i,c[i][0]);
            for (int j = k[b]-1; j >= 0; j--) c[i][j] -= c[i][0];
        }
        for (int i = 1; i < k[b]; i++){
            add_select_cost(b,i,c[k[a]-1][i]);
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

    // INFの場合は不可能
    long long mincost() {
        for (int i = 0; i < n; i++){
            for (int j = 0; j < k[i]-2; j++) G.add_edge(s[i]+j+1, s[i]+j, INF);
            long long mc = INF;
            for (int j = 0; j < k[i]; j++) mc = std::min(mc,cost[i][j]);
            if (mc == INF) return INF;
            for (int j = 0; j < k[i]; j++) add_select_cost(i,j,-mc);
            add_cost(mc);
            for (int j = 0; j < k[i]; j++){
                if (cost[i][j] == 0) continue;
                if (j == k[i]-1) G.add_edge(s[i]+j-1, s[n]+1, cost[i][j]);
                else if (j == 0) G.add_edge(s[n], s[i]+j, cost[i][j]);
                else G.add_edge(s[i]+j-1, s[i]+j, cost[i][j]);
            }
        }
        long long flow = G.max_flow(s[n], s[n]+1, INF);
        if (flow == INF) return INF;
        return ans+flow;
    }

    // 先にmincostが必要(INFの場合は不可)
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
