#ifndef _CROSS_HPP_
#define _CROSS_HPP_

#include "template.hpp"

// AB×CD
coord_t cross(const Point &A, const Point &B, const Point &C, const Point &D) {
    return (B.x - A.x) * (D.y - C.y) - (B.y - A.y) * (D.x - C.x);
}

// AB×AC
coord_t cross(const Point &A, const Point &B, const Point &C) {
    return cross(A,B,A,C);
}

// OA×OB
coord_t cross(const Point &A, const Point &B) {
    return cross(Point(0,0),A,B);
}

#endif
