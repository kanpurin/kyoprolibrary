#ifndef ALIEN_DP_HPP
#define ALIEN_DP_HPP

#include <functional>
#include "mongedag_lite.hpp"

// N頂点MongeコストDAGのd辺最短路長
long long AlienDP(int n, int d,
                  const std::function<long long(int,int)> &cost) {
    using MONGEDAG = LiteMongeDAG;
    long long l = -2e9, r = 2e9;
    while(r-l>1) {
        long long k = (l+r)/2;
        MONGEDAG g(n,[&cost,k](int a,int b){return cost(a,b) + k;});
        (d < g.d[n-1] ? l : r) = k;
    }
    MONGEDAG g(n,[&cost,r](int a,int b){return cost(a,b) + r;});
    return g.dist[n-1] - r*d;
}

#endif