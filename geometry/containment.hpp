#ifndef _CONTAINMENT_HPP_
#define _CONTAINMENT_HPP_

#include <vector>

#include "template.hpp"
#include "dot.hpp"
#include "cross.hpp"

// 0:含まれない 1:辺上にある 2:含まれる
int containment(const std::vector<Point> &polygon, const Point &point) {
    int n = polygon.size();
    bool in = false;
    for(int i = 0; i < n; i++) {
        Point a(polygon[i].x-point.x,polygon[i].y-point.y);
        Point b(polygon[(i+1)%n].x-point.x,polygon[(i+1)%n].y-point.y);
        if (a.y > b.y) std::swap(a,b);
        if (a.y <= 0 && 0 < b.y && cross(a,b) < 0) in = !in;
        if (cross(a,b) == 0 && dot(a,b) <= 0) return 1;
    }
    return in ? 2 : 0;
}

#endif
