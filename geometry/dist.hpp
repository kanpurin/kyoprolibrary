#ifndef _DIST_HPP_
#define _DIST_HPP_

#include "template.hpp"

// 距離の2乗
coord_t dist(const Point &A, const Point &B) {
    return (A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y);
}

#endif
