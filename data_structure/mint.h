#ifndef _MOD_INT_H_
#define _MOD_INT_H_

#include <iostream>

template<int mod>
struct mint {
private:
    int64_t x;
public:
    mint(int64_t x = 0) :x(x%mod) {}
    mint(std::string s) {
        int64_t z = 0;
        for(int i = 0; i < s.size(); i++) {
            z *= 10;
            z += s[i] - '0';
            z %= mod;
        }
        this->x = z;
    }
    mint& operator+=(const mint a) {
        if ((x += a.x) >= mod) x -= mod;
        return *this;
    }
    mint& operator-=(const mint a) {
        if ((x += mod - a.x) >= mod) x -= mod;
        return *this;
    }
    mint& operator*=(const mint a) {
        (x *= a.x) %= mod;
        return *this;
    }
    mint operator+(const mint a) const {
        mint res(*this);
        return res += a;
    }
    mint operator-(const mint a) const {
        mint res(*this);
        return res -= a;
    }
    mint operator*(const mint a) const {
        mint res(*this);
        return res *= a;
    }
    friend std::ostream& operator<<(std::ostream& os, const mint& n) {
        return os << n.x;
    }
    bool operator==(const mint a) const {
        return this->x == a.x;
    }
};

#endif
