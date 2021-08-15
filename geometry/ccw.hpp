#ifndef _CCW_HPP_
#define _CCW_HPP_

#include "template.hpp"
#include "dist.hpp"
#include "dot.hpp"
#include "cross.hpp"

// 回転方向
int ccw(const Point &A, const Point &B, const Point &C) {
    if(cross(A,B,C) > 0) return +1;     // "COUNTER_CLOCKWISE"
    if(cross(A,B,C) < 0) return -1;     // "CLOCKWISE"
    if(dot(A,B,C) < 0) return +2;       // "ONLINE_BACK"
    if(dist(A,B) < dist(A,C)) return -2;// "ONLINE_FRONT"
    return 0;                           // "ON_SEGMENT"
}

#endif
