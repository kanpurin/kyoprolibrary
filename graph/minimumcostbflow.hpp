#ifndef _MINIMUM_COST_B_FLOW_HPP_
#define _MINIMUM_COST_B_FLOW_HPP_

#include <vector>
#include <cassert>

#include "minimuncostflow.hpp"

// b-flow
// source,sinkの需給設定に注意
// O(ElogVΣmax(b_i,0))
template<typename Cap, typename Cost>
struct MinCostBFlow{
    MinCostFlow<Cap,Cost> G;
    std::vector<Cap> b;
    Cap pre_flow;
    Cost pre_cost;
    Cost INF;
public:
    MinCostBFlow(int v) : G(v+2), b(v,0), pre_flow(0), pre_cost(0), INF(G.INF) {}

    // 最小流量制約・負コスト可
    void add_edge(int from, int to, Cap low, Cap high, Cost cost) {
        assert(0 <= from && from <= G.V-2);
        assert(0 <= to && to <= G.V-2);
        assert(0 <= low && low <= high);
        Cap amount = (cost>=0?low:high);
        add_demand(from,amount);
        add_supply(to,amount);
        pre_flow += amount;
        pre_cost += cost*amount;
        if (low == high) return;
        G.add_edge(cost>=0?from:to,
                   cost>=0?to:from,
                   high-low,
                   cost>=0?cost:-cost);
    }

    // 供給量(sourceとか)
    void add_supply(int v,Cap amount) {
        b[v] += amount;
    }

    // 需要量(sinkとか)
    void add_demand(int v,Cap amount) {
        b[v] -= amount;
    }

    // 流せないならINF
    Cost min_cost_flow() {
        Cap flow = 0;
        int source = G.V-2, sink = G.V-1;
        for(int i = 0; i < G.V-2; i++) {
            if (b[i] > 0) G.add_edge(source,i,b[i],0), flow += b[i];
            else if (b[i] < 0) G.add_edge(i,sink,-b[i],0);
        }
        Cost ans = G.min_cost_flow(source,sink,flow);
        if (ans == INF) return INF;
        else return ans + pre_cost;
    }
};

#endif
