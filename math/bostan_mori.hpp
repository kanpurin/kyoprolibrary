#ifndef _BOSTAN_MORI_HPP_
#define _BOSTAN_MORI_HPP_

// #include <vector>
#include "../math/formalpowerseries.hpp"
#include "../data_structure/mint.hpp"

// [x^K]P(x)/Q(x) O(dlogdlogK)
// verify:https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence
template< int MOD , bool any = false >
mint<MOD> Bostan_Mori(FormalPowerSeries<MOD,any> p,FormalPowerSeries<MOD,any> q, long long k) {
    p.a.resize(max(p.size(),q.size()));
    q.a.resize(max(p.size(),q.size()));
    typedef FormalPowerSeries<MOD,any> FPS;
    while(k) {
        FPS _q = q;
        for (int i = 1; i < _q.size(); i+=2) _q[i] = -_q[i];
        FPS v = q, u = p;
        v *= _q; u *= _q;
        if (k&1&&u.size()&1) p.a.pop_back();
        for (int i = k%2; i < u.size(); i+=2) p[i/2] = u[i];
        for (int i = 0; i < v.size(); i+=2) q[i/2] = v[i];
        k/=2;
    }
    return p[0]/q[0];
}

#endif
