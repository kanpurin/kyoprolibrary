#ifndef _PICKS_THEOREM_HPP_
#define _PICKS_THEOREM_HPP_

#include <vector>
#include <algorithm>
#include <numeric>

#include "template.hpp"
#include "area.hpp"

// 多角形内の格子点の数え上げ
// (内部,辺上)
std::pair<long long,long long> picks_theorem(const std::vector<Point> &p) {
    int n = p.size();
    long long b = 0;
    for (int i = 0; i < n; i++) {
        coord_t dx = abs(p[(i+1)%n].x-p[i].x);
        coord_t dy = abs(p[(i+1)%n].y-p[i].y);
        b += std::gcd(dx,dy);
    }
    return {(abs(area(p))-b+2)/2,b};
}

#endif