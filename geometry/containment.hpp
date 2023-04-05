#ifndef _CONTAINMENT_HPP_
#define _CONTAINMENT_HPP_

#include <vector>

#include "template.hpp"
#include "dot.hpp"
#include "cross.hpp"
#include "ccw.hpp"

// O(logN)
// 0:含まれない 1:辺上にある 2:含まれる
// 反時計回りに与えられる
int containment(const std::vector<Point> &polygon, const Point &point) {
    int n = polygon.size();
    int l = 1, r = n-1;
    Point initpoint = polygon[0];
    if (cross(initpoint, polygon[l], point) < 0 ||
        cross(initpoint, point, polygon[r]) < 0) {
        return 0;
    }
    if (ccw(initpoint,polygon[l],point) == 0 || 
        ccw(initpoint,polygon[r],point) == 0) {
        return 1;
    }
    while(r-l>1) {
        int m = (l+r)/2;
        if (cross(initpoint, polygon[l], point) >= 0 &&
            cross(initpoint, point, polygon[m]) >= 0)
            r = m;
        else
            l = m;
    }
    return ccw(polygon[l],polygon[r],point) + 1;
}

#endif
