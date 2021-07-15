#ifndef _FORMAL_POWER_SERIES_HPP_
#define _FORMAL_POWER_SERIES_HPP_

#include <iostream>
#include <functional>
#include <vector>
#include <cassert>
#include <algorithm>
#include "../data_structure/mint.hpp"

// å`éÆìIôpãâêî
// any:îCà”modÇ≈åvéZÇ∑ÇÈÇ©
// verify:https://judge.yosupo.jp/
template < const int MOD , bool any = false>
struct FormalPowerSeries {
private:
    using FPS = FormalPowerSeries<MOD,any>;

    void ntt(bool inverse) {
        static bool first = true;
        static mint<MOD> dw[30], idw[30];
        if (first) {
            first = false;
            mint<MOD> root = 2;
            while (root.pow((MOD - 1) / 2) == 1) root += 1;
            for (int i = 0; i < 30; i++) dw[i] = -root.pow((MOD - 1) >> (i + 2)), idw[i] = mint<MOD>(1) / dw[i];
        }
        int n = this->size();
        assert((n & (n - 1)) == 0);
        if (not inverse) {
            for (int m = n; m >>= 1;) {
                mint<MOD> w = 1;
                for (int s = 0, k = 0; s < n; s += 2 * m) {
                    for (int i = s, j = s + m; i < s + m; i++, j++) {
                        auto x = this->a[i], y = this->a[j]*w;
                        if (x.x >= MOD) x.x -= MOD;
                        this->a[i].x = x.x + y.x, this->a[j].x = x.x+(MOD-y.x);
                    }
                    w *= dw[__builtin_ctz(++k)];
                }
            }
        } else {
            for (int m = 1; m < n; m *= 2) {
                mint<MOD> w = 1;
                for (int s = 0, k = 0; s < n; s += 2 * m) {
                    for (int i = s, j = s + m; i < s + m; i++, j++) {
                        auto x = this->a[i], y = this->a[j];
                        this->a[i] = x+y, this->a[j].x = x.x+(MOD-y.x), this->a[j] *= w;
                    }
                    w *= idw[__builtin_ctz(++k)];
                }
            }
        }
        auto c = mint<MOD>(1) / mint<MOD>(inverse ? n : 1);
        for (auto&& e : this->a) e *= c;
    }
    
    FPS convolution_naive(FPS &a, FPS &b) const {
        int n = int(a.size()), m = int(b.size());
        FPS ans(n+m-1);
        if (n < m) {
            for (int j = 0; j < m; j++) {
                for (int i = 0; i < n; i++) ans[i + j] += a[i]*b[j];
            }
        } 
        else {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) ans[i + j] += a[i]*b[j];
            }
        }
        return ans;
    }

    FPS& convolution_inplace(FPS b) {
        if (this->size() == 0 || b.size() == 0) {
            this->a.clear();
            return *this;
        }
        if (!any) {
            int n = this->size(), m = b.size(), sz = 1 << std::__lg(2*(n+m-1)-1);
            if (std::min(n, m) <= 60) return *this = convolution_naive(*this,b);
            this->resize(sz), this->ntt(false);
            b.resize(sz), b.ntt(false);
            for (int i = 0; i < sz; i++) this->a[i] *= b[i];
            this->ntt(true), this->resize(n + m - 1);
            return *this;
        }
        else {
            int n = this->a.size(), m = b.a.size();
            static constexpr int mod0 = 998244353, mod1 = 1300234241, mod2 = 1484783617;
            using Mint0 = mint< mod0 >;
            using Mint1 = mint< mod1 >;
            using Mint2 = mint< mod2 >;
            FormalPowerSeries< mod0 > l0(n), r0(m);
            FormalPowerSeries< mod1 > l1(n), r1(m);
            FormalPowerSeries< mod2 > l2(n), r2(m);
            for (int i = 0; i < n; i++) l0.a[i] = this->a[i].x, l1.a[i] = this->a[i].x, l2.a[i] = this->a[i].x;
            for (int j = 0; j < m; j++) r0.a[j] = b.a[j].x, r1.a[j] = b.a[j].x, r2.a[j] = b.a[j].x;
            l0 *= r0;
            l1 *= r1;
            l2 *= r2;
            this->resize(n + m - 1);
            static const Mint1 im0 = Mint1(1) / Mint1(mod0);
            static const Mint2 im1 = Mint2(1) / Mint2(mod1), im0m1 = im1 / mod0;
            static const mint<MOD> m0 = mod0, m0m1 = m0 * mod1;
            for (int i = 0; i < n + m - 1; i++) {
                int y0 = l0.a[i].x;
                int y1 = (im0 * (l1[i] - y0)).x;
                int y2 = (im0m1 * (l2[i] - y0) - im1 * y1).x;
                this->a[i] = m0m1 * y2 + y0 + m0 * y1;
            }
            return *this;
        }
    }

public:
    std::vector<mint<MOD>> a;

    FormalPowerSeries(int sz = 0) {
        this->a.resize(sz, 0);
    }

    FormalPowerSeries(const std::initializer_list<mint<MOD>> v) {
        this->a = v;
    }

    FormalPowerSeries(const std::vector<mint<MOD>> &v) {
        this->a = v;
    }

    int size() const {
        return a.size();
    }

    void resize(size_t sz, mint<MOD> m = mint<MOD>(0)) {
        this->a.resize(sz,m);
    }

    FPS operator-(const FPS &a) const { return FPS(*this) -= a; }
    FPS operator+(const FPS &a) const { return FPS(*this) += a; }
    FPS operator*(const long long a) const { return FPS(*this) *= a; }
    FPS operator*(const FPS &a) const { return FPS(*this) *= a; }
    FPS &operator+=(const FPS &r) {
        this->resize(std::max((int)this->size(),r.size()));
        for(int i = 0; i < (int)r.size(); i++) this->a[i] += r.a[i];
        return *this;
    }
    FPS &operator-=(const FPS &r) {
        this->resize(std::max((int)this->size(),r.size()));
        for(int i = 0; i < (int)r.size(); i++) this->a[i] -= r.a[i];
        return *this;
    }
    FPS &operator*=(const long long v) {
        for (int i = 0; i < this->size(); i++) this->a[i] *= v;
        return *this;
    }
    FPS &operator*=(const FPS &r) {
        this->convolution_inplace(r);
        return *this;
    }

    FPS low(int s) const {
        return FPS(std::vector<mint<MOD>>(this->a.begin(),this->a.begin()+std::min(std::max(s,1),this->size())));
    }

    // f(x)^-1 íËêîçÄÇÕ0Ç≈Ç»Ç¢
    FPS inverse(int deg = -1) const {
        int n = this->size();
        assert(n != 0 && this->a[0].x != 0);
        if(deg == -1) deg = n;
        FPS r({this->a[0].inv()});
        for(int m=1;m<deg;m*=2) {
            FPS f(std::vector<mint<MOD>>(this->a.begin(), this->a.begin() + std::min(n, 2*m)));
            FPS g(r);
            f.resize(2*m), f.ntt(false);
            g.resize(2*m), g.ntt(false);
            for (int i = 0; i < 2*m; i++) f[i] *= g[i];
            f.ntt(true);
            f.a.erase(f.a.begin(), f.a.begin() + m);
            f.resize(2*m), f.ntt(false);
            for (int i = 0; i < 2*m; i++) f[i] *= g[i];
            f.ntt(true); 
            for (int i = 0; i < 2*m; i++) f[i] = -f[i];
            r.a.insert(r.a.end(), f.a.begin(), f.a.begin() + m);
        }
        return r.low(deg);
    }

    // f'(x)
    FPS& differential_inplace() {
        const int n = (int)this->a.size();
        assert(n > 0);
        for(int i = 1; i < n; i++) this->a[i-1] = this->a[i] * i;
        this->a[n-1] = 0;
        return *this;
    }
    FPS differential() const { return FPS(*this).differential_inplace(); }

    // ÅÁf(x)dx
    FPS& integral_inplace() {
        const int n = (int)this->a.size();
        assert(n > 0);
        this->a.insert(this->a.begin(),0);
        std::vector<mint<MOD>> inv(n+1); inv[1] = 1;
        for (int i = 2; i <= n; i++) inv[i] = -inv[MOD%i]*(MOD/i);
        for (int i = 2; i <= n; i++) this->a[i] *= inv[i];
        return *this;
    }
    FPS integral() const { return FPS(*this).integ_inplace(); }

    // log(f(x)) íËêîçÄÇÕ1Ç≈Ç†ÇÈ
    FPS& log_inplace(int deg = -1) {
        int n = this->size();
        assert(n > 0 && this->a[0] == 1);
        if (deg == -1) deg = n;
        if (deg < n) this->resize(deg);
        FPS f_inv = this->inverse();
        this->differential_inplace();
        *this *= f_inv;
        this->resize(deg);
        this->integral_inplace();
        return *this;
    }  
    FPS log(const int deg = -1) const { return FPS(*this).log_inplace(deg); }

    // exp(f(x)) íËêîçÄÇÕ0Ç≈Ç†ÇÈ
    FPS& exp_inplace(int deg = -1) {
        int n = this->size();
        assert(n > 0 && (*this)[0] == 0);
        if (deg == -1) deg = n;
        assert(deg >= 0);
        FPS g({1}), g_fft;
        this->resize(deg);
        this->a[0] = 1;
        FPS h_drv = this->differential();
        for (int m = 1; m < deg; m *= 2) {
            FPS f_fft(std::vector<mint<MOD>>(this->a.begin(), this->a.begin() + m));
            f_fft.resize(2*m), f_fft.ntt(false);
            mint<MOD> invm = m; invm = invm.inv();

            if (m > 1) {
                FPS _f(m);
                for(int i = 0; i < m; i++) _f[i] = f_fft[i] * g_fft[i];
                _f.ntt(true);
                _f.a.erase(_f.a.begin(), _f.a.begin() + m/2);
                _f.resize(m), _f.ntt(false);
                for(int i = 0; i < m; i++) _f[i] *= g_fft[i];
                _f.ntt(true);
                _f.resize(m/2);
                for (int i = 0; i < m/2; i++) _f[i] = -_f[i];
                g.a.insert(g.a.end(), _f.a.begin(), _f.a.begin() + m/2);
            }

            FPS t(std::vector<mint<MOD>>(this->a.begin(), this->a.begin() + m)); 
            t.differential_inplace();
            {
                FPS r(std::vector<mint<MOD>>(h_drv.a.begin(), h_drv.a.begin() + m-1));
                r.resize(m); r.ntt(false);
                for (int i = 0; i < m; i++) r.a[i] *= f_fft.a[i];
                r.ntt(true);
                t -= r;
                t.a.insert(t.a.begin(), t.a.back()); t.a.pop_back();
            }

            t.resize(2*m); t.ntt(false); 
            g_fft = g; g_fft.resize(2*m); g_fft.ntt(false);
            for (int i = 0; i < 2*m; i++) t.a[i] *= g_fft.a[i];
            t.ntt(true);
            t.resize(m);
            
            FPS v(std::vector<mint<MOD>>(this->a.begin() + m, this->a.begin() + std::min(deg, 2*m))); v.resize(m);
            t.a.insert(t.a.begin(), m-1, 0); t.a.push_back(0);
            t.integral_inplace();
            for (int i = 0; i < m; i++) v.a[i] -= t.a[m+i];

            v.resize(2*m); v.ntt(false);
            for (int i = 0; i < 2*m; i++) v.a[i] *= f_fft.a[i];
            v.ntt(true);
            v.resize(m);

            for (int i = 0; i < std::min(deg-m,m); i++) this->a[m+i] = v.a[i];
        }
        return *this;
    }
    FPS exp(const int deg = -1) const { return FPS(*this).exp_inplace(deg); }

    // f(x)^k
    FPS& pow_inplace(long long k, int deg = -1) {
        int n = this->size();
        if (deg == -1) deg = n;
        assert(deg >= 0);
        int l = 0;
        while (this->a[l] == 0) ++l;
        if (l > deg/k) return *this = FPS(deg);
        mint<MOD> ic = this->a[l].inv();
        mint<MOD> pc = this->a[l].pow(k);
        this->a.erase(this->a.begin(), this->a.begin() + l);
        *this *= ic.x;
        this->log_inplace();
        *this *= k;
        this->exp_inplace();
        *this *= pc.x;
        this->a.insert(this->a.begin(), l*k, 0);
        this->resize(deg);
        return *this;
    }
    FPS pow(const long long k, const int deg = -1) const { return FPS(*this).pow_inplace(k, deg); }

    mint<MOD> &operator[](int x) {
        assert(0 <= x && x < (int)this->a.size());
        return a[x];
    }
    
    friend std::ostream &operator<<(std::ostream &os, const FPS &p) {
        os << "[ ";
        for (int i = 0; i < p.size(); ++i) {
            os << p.a[i] << " ";
        }
        os << "]";
        return os;
    }
};

#endif