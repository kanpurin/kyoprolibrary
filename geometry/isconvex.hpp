#ifndef _IS_CONVEX_HPP_
#define _IS_CONVEX_HPP_

#include <vector>

#include "template.hpp"
#include "cross.hpp"

// 多角形の凸性判定
// 頂点は反時計回りで与えられる
bool isConvex(const std::vector<Point> &p) {
    int n = p.size();
    if (n <= 2) return true;
    for (int i = 0; i < n; i++) {
        if (cross(p[i],p[(i+1)%n],p[(i+1)%n],p[(i+2)%n]) < 0) 
            return false;
    }
    return true;
}

#endif
