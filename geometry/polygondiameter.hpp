#ifndef _POLYGON_DIAMETER_HPP_
#define _POLYGON_DIAMETER_HPP_

#include <vector>

#include "template.hpp"
#include "dist.hpp"
#include "cross.hpp"

// 凸多角形の最遠点対のindex
// 頂点は反時計回りで与えられる
std::pair<int,int> polygon_diameter(const std::vector<Point> &p) {
    int n = p.size();
    if (n == 2) {
        return {0, 1};
    }
    int i = 0, j = 0;
    for (int k = 0; k < n; k++) {
        if (p[k] < p[i]) i = k;
        if (p[j] < p[k]) j = k;
    }
    coord_t res = 0;
    std::pair<int,int> ans;
    int si = i, sj = j;
    while (i != sj || j != si) {
        coord_t d = dist(p[i],p[j]);
        if (res < d) {
            res = d;
            ans = {i,j};
        }
        if (cross(p[i],p[(i+1)%n],p[j],p[(j+1)%n]) < 0) i = (i+1)%n;
        else j = (j+1)%n;
    }
    return ans;
}

#endif
