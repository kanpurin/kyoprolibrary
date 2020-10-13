#ifndef _FORMAL_POWER_SERIES_HPP_
#define _FORMAL_POWER_SERIES_HPP_

#include <iostream>
#include <functional>
#include <vector>
#include <cassert>
#include <algorithm>
#include "../data_structure/mint.hpp"

// 形式的冪級数
// any:任意modで計算するか
// verify:https://judge.yosupo.jp/
// 参考:https://ei1333.github.io/luzhiled/snippets/math/formal-power-series.html
template < const int MOD , bool any = false>
struct FormalPowerSeries {
private:
    using P = FormalPowerSeries< MOD, any >;

    template < class T, class F = std::multiplies< T > >
    T power(T a, long long n, F op = std::multiplies< T >(), T e = {1}) const {
        assert(n >= 0);
        T res = e;
        while (n) {
            if (n & 1) res = op(res, a);
            if (n >>= 1) a = op(a, a);
        }
        return res;
    }

    template< int _MOD >
    void ntt(std::vector< mint < _MOD > >& a, bool inverse) {
        static std::vector< mint< _MOD > > dw(30), idw(30);
        if (dw[0] == 0) {
            mint< _MOD > root = 2;
            while (power(root, (_MOD - 1) / 2) == 1) root += 1;
            for (int i = 0; i < 30; i++) dw[i] = -power(root, (_MOD - 1) >> (i + 2)), idw[i] = mint<_MOD>(1) / dw[i];
        }
        int n = a.size();
        assert((n & (n - 1)) == 0);
        if (not inverse) {
            for (int m = n; m >>= 1;) {
                mint< _MOD > w = 1;
                for (int s = 0, k = 0; s < n; s += 2 * m) {
                    for (int i = s, j = s + m; i < s + m; i++, j++) {
                        auto x = a[i], y = a[j] * w;
                        if (x.x >= _MOD) x.x -= _MOD;
                        a[i].x = x.x + y.x, a[j].x = x.x + (_MOD - y.x);
                    }
                    w *= dw[__builtin_ctz(++k)];
                }
            }
        } else {
            for (int m = 1; m < n; m *= 2) {
                mint< _MOD > w = 1;
                for (int s = 0, k = 0; s < n; s += 2 * m) {
                    for (int i = s, j = s + m; i < s + m; i++, j++) {
                        auto x = a[i], y = a[j];
                        a[i] = x + y, a[j].x = x.x + (_MOD - y.x), a[j] *= w;
                    }
                    w *= idw[__builtin_ctz(++k)];
                }
            }
        }
        auto c = mint<_MOD>(1) / mint< _MOD >(inverse ? n : 1);
        for (auto&& e : a) e *= c;
    }

    template< int _MOD >
    std::vector< mint< _MOD > > convolution(std::vector< mint< _MOD > > l, std::vector< mint< _MOD > > r) {
        if (l.empty() || r.empty()) return {};
        int n = l.size(), m = r.size(), sz = 1 << std::__lg(2 * (n + m - 1) - 1);
        if (std::min(n, m) < 30) {
            std::vector< long long > res(n + m - 1);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++) res[i + j] += (l[i] * r[j]).x;
            return {begin(res), end(res)};
        }
        bool eq = l == r;
        l.resize(sz), ntt(l, false);
        if (eq) r = l;
        else r.resize(sz), ntt(r, false);
        for (int i = 0; i < sz; i++) l[i] *= r[i];
        ntt(l, true), l.resize(n + m - 1);
        return l;
    }

    P pre(const P &p, int sz) const {
        P ret;
        ret.a = std::vector<mint<MOD>>(p.a.begin(), p.a.begin() + std::min((int)p.a.size(), sz));
        return ret;
    }

public:
    std::vector<mint<MOD>> a;

    FormalPowerSeries(std::initializer_list<mint<MOD>> v) {
        this->a = v;
    }

    FormalPowerSeries(int sz = 0) {
        this->a.resize(sz, 0);
    }
    
    // sizeとkの小さい方のサイズにresize
    P resize(int k) const {
        return pre(*this,k);
    }

    size_t size() const { return this->a.size(); }

    P operator+(const P &a) const { return P(*this) += a; }
    P operator+(const long long a) const { return P(*this) += a; }
    P operator-(const P &a) const { return P(*this) -= a; }
    P operator*(const P &a) const { return P(*this) *= a; }
    P operator*(const long long a) const { return P(*this) *= a; }
    P operator/(const P &a) const { return P(*this) /= a; }

    P &operator+=(const P &r) {
        this->a.resize(std::max(this->a.size(),r.size()));
        for(int i = 0; i < (int)r.size(); i++) this->a[i] += r.a[i];
        return *this;
    }
    
    P &operator+=(const long long v) {
        if (this->a.size() == 0) this->a.resize(1,(v % MOD + MOD) % MOD);
        else this->a[0] += v;
        return *this;
    }

    P &operator-=(const P &r) {
        this->a.resize(std::max(this->a.size(),r.size()));
        for(int i = 0; i < (int)r.size(); i++) this->a[i] -= r.a[i];
        return *this;
    }
    P &operator*=(const P &b) {
        if (!any) {
            this->a = convolution(this->a, b.a);
            return *this;
        }
        else {
            if (this->a.empty() || b.a.empty()) {
                this->a.clear();
                return *this;
            }
            int n = this->a.size(), m = b.a.size();
            static constexpr int mod0 = 998244353, mod1 = 1300234241, mod2 = 1484783617;
            using Mint0 = mint< mod0 >;
            using Mint1 = mint< mod1 >;
            using Mint2 = mint< mod2 >;
            std::vector< Mint0 > l0(n), r0(m);
            std::vector< Mint1 > l1(n), r1(m);
            std::vector< Mint2 > l2(n), r2(m);
            for (int i = 0; i < n; i++) l0[i] = this->a[i].x, l1[i] = this->a[i].x, l2[i] = this->a[i].x;
            for (int j = 0; j < m; j++) r0[j] = b.a[j].x, r1[j] = b.a[j].x, r2[j] = b.a[j].x;
            l0 = convolution(l0,r0);
            l1 = convolution(l1,r1);
            l2 = convolution(l2,r2);
            this->a.resize(n + m - 1);
            static const Mint1 im0 = Mint1(1) / Mint1(mod0);
            static const Mint2 im1 = Mint2(1) / Mint2(mod1), im0m1 = im1 / mod0;
            static const mint<MOD> m0 = mod0, m0m1 = m0 * mod1;
            for (int i = 0; i < n + m - 1; i++) {
                int y0 = l0[i].x;
                int y1 = (im0 * (l1[i] - y0)).x;
                int y2 = (im0m1 * (l2[i] - y0) - im1 * y1).x;
                this->a[i] = m0m1 * y2 + y0 + m0 * y1;
            }
            return *this;
        }
    }
    P &operator*=(const long long v) {
        for (int i = 0; i < this->a.size(); i++) this->a[i] *= v;
        return *this;
    }
    // a[0]は0でない
    P &operator/=(const P &a) {
        *this *= a.inverse();
        return *this;
    }

    // f(x)^-1 定数項は0でない
    P inverse(int deg = -1) const {
        assert(this->a.size() != 0 && this->a[0].x != 0);
        const int n = (int)this->a.size();
        if(deg == -1) deg = n;
        P ret(1);
        ret[0] = mint<MOD>(1) / a[0];
        for(int i = 1; i < deg; i <<= 1) {
            ret = pre((ret + ret - ret * ret * pre(*this,i << 1)),i << 1);
        }
        return pre(ret,deg);
    }

    // f'(x)
    P differential() const {
        const int n = (int) this->a.size();
        P ret(std::max(0, n - 1));
        for(int i = 1; i < n; i++) ret[i-1] = this->a[i] * i;
        return ret;
    }

    // ∫f(x)dx
    P integral() const {
        const int n = (int) this->a.size();
        P ret(n + 1);
        for(int i = 0; i < n; i++) ret[i + 1] = this->a[i] / (i + 1);
        return ret;
    }

    // log(f(x)) 定数項は1である
    P log(int deg = -1) const {
        assert(this->a.size() != 0 && this->a[0] == 1);
        const int n = (int)this->a.size();
        if(deg == -1) deg = n;
        return pre((this->differential() * this->inverse(deg)),deg - 1).integral();
    }

    // exp(f(x)) 定数項は0である
    P exp(int deg = -1) const {
        if (this->a.size() == 0) return P(0);
        assert(this->a[0] == 0);
        const int n = (int)this->a.size();
        if(deg == -1) deg = n;
        P ret(1);
        ret.a[0] = 1;
        for(int i = 1; i < deg; i <<= 1) {
            ret = pre((ret * (pre(*this,i << 1) + 1 - ret.log(i << 1))),i << 1);
        }
        return pre(ret,deg);
    }

    // f(x)^k
    P pow(long long k, int deg = -1) const {
        const int n = (int) this->a.size();
        if(deg == -1) deg = n;
        for(int i = 0; i < n; i++) {
            if(this->a[i].x != 0) {
                long long rev = (mint<MOD>(1) / this->a[i]).x;
                P C = *this * rev;
                P D(n - i);
                for(int j = i; j < n; j++) D[j - i] = C[j];
                D = (D.log() * k).exp() * power(this->a[i], k).x;
                P E(deg);
                if(i * k > deg) return E;
                auto S = i * k;
                for(int j = 0; j + S < deg && j < D.size(); j++) E[j + S] = D[j];
                return E;
            }
        }
        return *this;
    }

    mint< MOD > &operator[](int x) {
        assert(0 <= x && x < (int)this->a.size());
        return a[x];
    }

    friend std::ostream &operator<<(std::ostream &os, const P &p) {
        os << "[ ";
        for (int i = 0; i < p.size(); ++i) {
            os << p.a[i] << " ";
        }
        os << "]";
        return os;
    }
};

#endif