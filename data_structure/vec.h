#ifndef _VEC_H_
#define _VEC_H_

#include <cmath>
#include <iostream>

struct vec {
private:
    long long _gcd(long long a, long long b) {
        return b ? _gcd(b, a % b) : a;
    }

public:
    long long x, y;
    int round;  // 何周か(普段は使わん)
    vec() : x(0), y(0), round(0) {}
    vec(long long X, long long Y, int round = 0) : round(round) {
        int cx = (X >= 0) ? 1 : -1;
        int cy = (Y >= 0) ? 1 : -1;
        X = std::abs(X);
        Y = std::abs(Y);
        if (X == 0 && Y == 0) {
            x = 0, y = 0;
        } else {
            long long g = _gcd(X, Y);
            x = cx * X / g;
            y = cy * Y / g;
        }
    }
    vec operator+(const vec &v) const {
        return vec(x + v.x, y + v.y, round + v.round);
    }
    vec operator-(const vec &v) const {
        return vec(x - v.x, y - v.y, round - v.round);
    }
    // roundに注意
    vec operator-() const {
        return vec(-x, -y, -round);
    }

    // 180°回転
    vec rotate180() {
        if (*this >= vec(-1, 0, round)) {
            return vec(-x, -y, round + 1);
        }
        return vec(-x, -y, round);
    }
    // 90°回転
    vec rotate90() {
        if (*this >= vec(0, -1, round)) {
            return vec(-y, x, round + 1);
        }
        return vec(-y, x, round);
    }
    // -90°回転
    vec rotate_90() {
        if (*this < vec(0, 1, round)) {
            return vec(y, -x, round - 1);
        }
        return vec(y, -x, round);
    }

    bool operator==(const vec &v) const {
        return x == v.x && y == v.y && round == v.round;
    }

    // 偏角の比較
    bool operator<(const vec &v) const {
        if (round != v.round) return round < v.round;
        if (*this == v) return false;
        if (y > 0 && v.y > 0) {
            if ((x > 0 && v.x > 0) || (x < 0 && v.x < 0)) return y * v.x < x * v.y;
            if (x <= 0 && v.x >= 0) return false;
            return true;
        } else if (y < 0 && v.y < 0) {
            if ((x < 0 && v.x < 0) || (x > 0 && v.x > 0)) return y * v.x < x * v.y;
            if (x >= 0 && v.x <= 0) return false;
            return true;
        } else if (y > 0 && v.y < 0) {
            return true;
        } else if (y < 0 && v.y > 0) {
            return false;
        } else if (y == 0 && x > 0 && (v.y != 0 || (v.y == 0 && v.x < 0))) {
            return true;
        } else if (y == 0 && x < 0 && v.y < 0) {
            return true;
        } else if (v.y == 0 && v.x < 0 && y > 0) {
            return true;
        } else {
            return false;
        }
    }

    bool operator<=(const vec &v) const {
        return *this < v || *this == v;
    }
    bool operator>=(const vec &v) const {
        return !(*this < v);
    }
    bool operator>(const vec &v) const {
        return !(*this <= v);
    }
    friend std::ostream &operator<<(std::ostream &os, const vec &v) {
        return os << "[" << v.x << "," << v.y << "]";
    }
};

#endif
