#ifndef _MOD_INT_HPP_
#define _MOD_INT_HPP_

#include <iostream>
#include <string>
#include <cassert>

template< int MOD >
struct mint {
public:
    unsigned int x;
    mint() : x(0) {}
    mint(long long v) {
        long long w = (long long)(v % (long long)(MOD));
        if (w < 0) w += MOD;
        x = (unsigned int)(w);
    }
    mint(std::string &s) {
        unsigned int z = 0;
        for (int i = 0; i < s.size(); i++) {
            z *= 10;
            z += s[i] - '0';
            z %= MOD;
        }
        x = z;
    }
    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }
    mint& operator+=(const mint &a) {
        if ((x += a.x) >= MOD) x -= MOD;
        return *this;
    }
    mint& operator-=(const mint &a) {
        if ((x -= a.x) >= MOD) x += MOD;
        return *this;
    }
    mint& operator*=(const mint &a) {
        unsigned long long z = x;
        z *= a.x;
        x = (unsigned int)(z % MOD);
        return *this;
    }
    mint& operator/=(const mint &a) {return *this = *this * a.inv(); }
    friend mint operator+(const mint& lhs, const mint& rhs) {
        return mint(lhs) += rhs;
    }
    friend mint operator-(const mint& lhs, const mint& rhs) {
        return mint(lhs) -= rhs;
    }
    friend mint operator*(const mint& lhs, const mint& rhs) {
        return mint(lhs) *= rhs;
    }
    friend mint operator/(const mint& lhs, const mint& rhs) {
        return mint(lhs) /= rhs;
    }
    friend bool operator==(const mint& lhs, const mint& rhs) {
        return lhs.x == rhs.x;
    }
    friend bool operator!=(const mint& lhs, const mint& rhs) {
        return lhs.x != rhs.x;
    }
    friend std::ostream& operator<<(std::ostream &os, const mint &n) {
        return os << n.x;
    }
    friend std::istream &operator>>(std::istream &is, mint &n) {
        unsigned int x;
        is >> x;
        n = mint(x);
        return is;
    }
    mint inv() const {
        assert(x);
        return pow(MOD-2);
    }
    mint pow(long long n) const {        
        assert(0 <= n);
        mint p = *this, r = 1;
        while (n) {
            if (n & 1) r *= p;
            p *= p;
            n >>= 1;
        }
        return r;
    }
};

#endif
