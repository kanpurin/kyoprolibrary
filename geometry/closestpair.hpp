#ifndef _CLOSEST_PAIR_HPP_
#define _CLOSEST_PAIR_HPP_

#include <vector>
#include <algorithm>
#include <numeric>

#include "template.hpp"

// 最近点の距離の2乗 O(NlogN)
// pはx座標でソートする
// 呼び出し後のpはy座標でソートされている
coord_t closest_pair(std::vector<Point> &p, int l = 0, int r = -1) {
    int n = p.size();
    if (r == -1) r = n;
    if (r-l <= 1) return std::numeric_limits<coord_t>::max();
    int m = (l+r) / 2;
    coord_t x = p[m].x;
    coord_t d = std::min(closest_pair(p,l,m),closest_pair(p,m,r));
    inplace_merge(p.begin()+l, p.begin()+m, p.begin()+r, [](Point a, Point b) {return a.y < b.y;});

    std::vector<int> b;
    for (int i = l; i < r; i++) {
        if ((p[i].x-x)*(p[i].x-x) >= d) continue;
        for (int j = b.size()-1; j >= 0; j--) {
            coord_t dx = p[i].x - p[b[j]].x;
            coord_t dy = p[i].y - p[b[j]].y;
            if (dy*dy >= d) break;
            d = std::min(d,dx*dx+dy*dy);
        }
        b.push_back(i);
    }
    return d;
}

#endif