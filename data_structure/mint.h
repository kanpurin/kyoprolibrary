#ifndef _MOD_INT_H_
#define _MOD_INT_H_

#include <iostream>
#include <string>

constexpr int MOD = 1e9 + 7;

struct mint {
public:
    long long x;
    mint(long long x = 0) :x((MOD+x)%MOD) {}
    mint(std::string &s) {
        long long z = 0;
        for (int i = 0; i < s.size(); i++) {
            z *= 10;
            z += s[i] - '0';
            z %= MOD;
        }
        this->x = z;
    }
    mint& operator+=(const mint &a) {
        if ((x += a.x) >= MOD) x -= MOD;
        return *this;
    }
    mint& operator-=(const mint &a) {
        if ((x += MOD - a.x) >= MOD) x -= MOD;
        return *this;
    }
    mint& operator*=(const mint &a) {
        (x *= a.x) %= MOD;
        return *this;
    }
    mint& operator/=(const mint &a) {
        long long n = MOD - 2;
        mint u = 1, b = a;
        while (n > 0) {
            if (n & 1) {
                u *= b;
            }
            b *= b;
            n >>= 1;
        }
        return *this *= u;
    }
    mint operator+(const mint &a) const {
        mint res(*this);
        return res += a;
    }
    mint operator-(const mint &a) const {
        mint res(*this);
        return res -= a;
    }
    mint operator*(const mint &a) const {
        mint res(*this);
        return res *= a;
    }
    mint operator/(const mint &a) const {
        mint res(*this);
        return res /= a;
    }
    friend std::ostream& operator<<(std::ostream &os, const mint &n) {
        return os << n.x;
    }
    bool operator==(const mint &a) const {
        return this->x == a.x;
    }
    mint pow(long long k) const {
        mint ret = 1;
        mint p = this->x;
        while (k > 0) {
            if (k & 1) {
                ret *= p;
            }
            p *= p;
            k >>= 1;
        }
        return ret;
    }
};

#endif
