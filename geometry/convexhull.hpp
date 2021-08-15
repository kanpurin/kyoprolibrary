#ifndef _CONVEX_HULL_HPP_
#define _CONVEX_HULL_HPP_

#include <vector>
#include <cassert>
#include <algorithm>

#include "template.hpp"
#include "cross.hpp"

std::vector<Point> convexhull(std::vector<Point> p) {
    int n = p.size(), k = 0;
    assert((int)p.size() >= 3);
    sort(p.begin(), p.end());
    std::vector<Point> h(2*n);
    // 上側
    for (int i = 0; i < n; h[k++] = p[i++]) 
        while(k >= 2 && cross(h[k-2],h[k-1],p[i]) < 0) k--;
    // 下側
    for (int i = n-1, t = k+1; i > 0; h[k++] = p[--i]) 
        while(k >= t && cross(h[k-2],h[k-1],p[i-1]) < 0) k--;
    h.resize(k-1);
    return h;
}

#endif
