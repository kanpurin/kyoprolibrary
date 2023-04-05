#ifndef _CONTAINMENT_HPP_
#define _CONTAINMENT_HPP_

#include <vector>

#include "template.hpp"
#include "dot.hpp"
#include "cross.hpp"

// O(logN)
// 0:含まれない 1:辺上にある 2:含まれる
// 反時計回りに与えられる
int containment(const std::vector<Point> &polygon, const Point &point) {
    int n = polygon.size();
    int l = 1, r = n-1;
    Point initpoint = polygon[0];
    coord_t b1 = cross(initpoint, polygon[l], point);
    coord_t b2 = cross(initpoint, point, polygon[r]);
    if (b1 < 0 || b2 < 0) return 0;
    while(r-l>1) {
        int m = (l+r)/2;
        (cross(initpoint, point, polygon[m]) >= 0 ? r : l) = m;
    }
    coord_t v = cross(point, polygon[l], polygon[r]);
    return v == 0 ? 1 : v > 0 ? (b1 == 0 || b2 == 0 ? 1 : 2) : 0;
}

#endif
