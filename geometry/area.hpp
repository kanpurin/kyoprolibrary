#ifndef _AREA_HPP_
#define _AREA_HPP_

#include <vector>

#include "template.hpp"
#include "cross.hpp"

// 多角形の面積の2倍
// 反時計回りなら正，時計回りなら負となる
coord_t area(const std::vector<Point> &p) {
    int n = p.size();
    if (n <= 2) return 0;
    coord_t ans = 0;
    for (int i = 0; i < n; i++) ans += cross(p[i],p[(i+1)%n]);
    return ans;
}

#endif
