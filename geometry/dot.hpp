#ifndef _DOT_HPP_
#define _DOT_HPP_

#include "template.hpp"

// AB,CDの内積
inline coord_t dot(const Point &A, const Point &B, const Point &C, const Point &D) {
    return (B.x - A.x) * (D.x - C.x) + (B.y - A.y) * (D.y - C.y);
}

// AB,ACの内積
inline coord_t dot(const Point &A, const Point &B, const Point &C) {
    return dot(A,B,A,C);
}

// OA,OBの内積
inline coord_t dot(const Point &A, const Point &B) {
    return dot(Point(0,0),A,B);
}

// AとBの内積
inline coord_t dot(const Vector &A, const Vector &B) {
    return dot(Point(A.x,A.y),Point(B.x,B.y));
}

#endif
