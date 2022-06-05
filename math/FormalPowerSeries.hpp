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
            FormalPowerSeries< mod0 > l0(n), r0(m);
            FormalPowerSeries< mod1 > l1(n), r1(m);
            FormalPowerSeries< mod2 > l2(n), r2(m);
            for (int i = 0; i < n; i++) l0.a[i] = this->a[i].x, l1.a[i] = this->a[i].x, l2.a[i] = this->a[i].x;
            for (int j = 0; j < m; j++) r0.a[j] = b.a[j].x, r1.a[j] = b.a[j].x, r2.a[j] = b.a[j].x;
            l0 *= r0;
            l1 *= r1;
            l2 *= r2;
            crt(*this,l0,l1,l2);
            return *this;
        }
    }

    template<const int MOD0, const int MOD1, const int MOD2>
    static void crt(FPS &fps,
             const FormalPowerSeries<MOD0> &fps0, 
             const FormalPowerSeries<MOD1> &fps1, 
             const FormalPowerSeries<MOD2> &fps2) {
        assert(fps0.size() == fps1.size() && fps0.size() == fps2.size());
        int n = (int)fps0.size();
        fps.resize(n);
        static const mint<MOD1> im0 = mint<MOD1>(MOD0).inv();
        static const mint<MOD2> im1 = mint<MOD2>(MOD1).inv(), im0m1 = im1/MOD0;
        static const mint<MOD> m0 = MOD0, m0m1 = m0*MOD1;
        for (int i = 0; i < n; i++) {
            int y0 = fps0.a[i].x;
            int y1 = (im0*(fps1.a[i]-y0)).x;
            int y2 = (im0m1*(fps2.a[i]-y0)-im1*y1).x;
            fps.a[i] = m0m1*y2+y0+m0*y1;
        }
    }

    struct Fact {
    private:
        int N;
    public:
        std::vector< mint< MOD > > FACT, IFACT;
        // nCk
        Fact(int n) : N(n) {
            FACT.resize(n + 1);
            IFACT.resize(n + 1);
            FACT[0] = 1;
            for (int i = 1; i <= n; i++) {
                FACT[i] = FACT[i - 1] * i;
            }
            IFACT[n] = FACT[n].inv();
            for (int i = n-1; i >= 0; i--) {
                IFACT[i] = IFACT[i+1] * (i+1);
            }
        }
    };

    FPS rev() const {
        FPS ret(*this);
        reverse(ret.a.begin(), ret.a.end());
        return ret;
    }

    void shrink() {
        while (this->a.size() && this->a.back() == 0) this->a.pop_back();
    }

    static std::vector<FPS> subproduct_tree(const std::vector<mint<MOD>> &xs) {
        int n = (int) xs.size();
        int k = 1;
        while(k < n) k <<= 1;
        std::vector<FPS> g(2 * k, {1});
        for(int i = 0; i < n; i++) g[k + i] = {-xs[i], 1};
        for(int i = k; i-- > 1;) g[i] = g[i << 1] * g[i << 1 | 1];
        return g;
    }

    
    FPS _sqrt(int s) const {
		assert(this->a[0]==1);
		static const mint<MOD> half=mint<MOD>(1)/2;
		FPS f({1}),g({1}),z({1});
		for(int n=1;n<s;n*=2){
			for (int i = 0; i < n; i++) z[i]*=z[i];
			z.ntt(true);
            
			FPS delta(2*n),gbuf(2*n);
			for (int i = 0; i<n; i++) delta[n+i] = z[i] - (i<size()?this->a[i]:0) - (n+i<size()?this->a[n+i]:0);
            copy(g.a.begin(),g.a.end(), gbuf.a.begin());
			delta.ntt(false);
			gbuf.ntt(false);
			for (int i = 0;i < 2*n; i++) delta[i]*=gbuf[i];
			delta.ntt(true);

			f.resize(2*n);
            for(int i=n;i<2*n;i++) f[i]=-half*delta[i];
			
			if(2*n>=s)break;
			
			z=f;
			z.ntt(false);
			
			FPS eps=gbuf;
			for (int i = 0;i < 2*n;i++) eps[i]*=z[i];
			eps.ntt(true);
			
			for(int i = 0; i < n; i++)eps[i]=0;
			eps.ntt(false);
			
			for(int i = 0; i < 2*n; i++)eps[i]*=gbuf[i];
			eps.ntt(true);
			g.resize(2*n);
			for(int i = n; i < 2*n; i++)g[i]=-eps[i];
		}
		f.resize(s);
		return f;
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

    FPS operator+(const mint<MOD> &a) const { return FPS(*this) += a; }
    FPS operator+(const FPS &a) const { return FPS(*this) += a; }
    FPS operator-(const mint<MOD> &a) const { return FPS(*this) -= a; }
    FPS operator-(const FPS &a) const { return FPS(*this) -= a; }
    FPS operator*(const mint<MOD> &a) const { return FPS(*this) *= a; }
    FPS operator*(const long long a) const { return FPS(*this) *= a; }
    FPS operator*(const FPS &a) const { return FPS(*this) *= a; }
	FPS operator/(const mint<MOD> &a) const { return FPS(*this) /= a;}
    FPS operator/(const FPS &a) const { return FPS(*this) /= a; }
    FPS operator%(const FPS &a) const { return FPS(*this) %= a; }
    FPS &operator+=(const mint<MOD> &v) {
        this->a[0] += v;
        return *this;
    }
    FPS &operator+=(const FPS &r) {
        this->resize(max((int)this->size(),r.size()));
        for(int i = 0; i < (int)r.size(); i++) this->a[i] += r.a[i];
        return *this;
    }
    FPS &operator-=(const mint<MOD> &v) {
        this->a[0] -= v;
        return *this;
    }
    FPS &operator-=(const FPS &r) {
        this->resize(max((int)this->size(),r.size()));
        for(int i = 0; i < (int)r.size(); i++) this->a[i] -= r.a[i];
        return *this;
    }
    FPS &operator*=(const mint<MOD> &v) {
        for (int i = 0; i < this->size(); i++) this->a[i] *= v;
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
	FPS &operator/=(const mint<MOD> &v){
		return *this *= v.inv();
	}
    FPS &operator/=(const FPS &r) {
        if (this->size() < r.size()) {
            this->a.clear();
            return *this;
        }
        int n = this->size() - r.size() + 1;
        if ((int)r.size() <= 64) {
            FPS f(*this), g(r);
            g.shrink();
            mint<MOD> coeff = g.a.back().inv();
            for (auto &x : g.a) x *= coeff;
            int deg = (int)f.size() - (int)g.size() + 1;
            int gs = g.size();
            FPS quo(deg);
            for (int i = deg - 1; i >= 0; i--) {
                quo[i] = f[i + gs - 1];
                for (int j = 0; j < gs; j++) f[i + j] -= quo[i] * g[j];
            }
            *this = quo * coeff;
            this->resize(n, 0);
            return *this;
        }
        return *this = ((*this).rev().low(n) * r.rev().inverse(n)).low(n).rev();
    }

    FPS &operator%=(const FPS &Q) {
        if(Q.size() > this->size()) return *this;
        if(Q.size() < 32) {
            int dQ = Q.size()-1;
            while(dQ && Q.a[dQ] == 0) dQ--;
            assert(Q.a[dQ] != 0);
            for(int i = this->size()-1; i >= dQ; i--){
                if(this->a[i] == 0) continue;
                mint<MOD> x = this->a[i] / Q.a[dQ];
                this->a[i] = 0;
                for(int j = 1; j <= dQ; j++){
                    this->a[i - j] -= x * Q.a[dQ - j];
                }
            }
            shrink();
            return *this;
        }
        FPS P = (*this) / Q;
        P *= Q;
        int dR = -1;
        for(int i = 0; i < Q.size()-1; i++){
            P.a[i] = this->a[i] - P.a[i];
            if(P.a[i] != 0) dR = i;
        }
        this->a.resize(dR + 1);
        for(int i = 0; i <= dR; i++) this->a[i] = P.a[i];
        return *this;
    }

    FPS low(int s) const {
        return FPS(std::vector<mint<MOD>>(this->a.begin(),this->a.begin()+std::min(std::max(s,1),this->size())));
    }

    // f(x)^-1 定数項は0でない
    FPS inverse(int deg = -1) const {
        int n = this->size();
        assert(n != 0 && this->a[0].x != 0);
        if(deg == -1) deg = n;

        if (!any) {
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
        else {
            FPS r({this->a[0].inv()});
            for (int i = 1; i < deg; i <<= 1)
                r = (r*2 - r.square()*(*this).low(i<<1)).low(i<<1);
            return r.low(deg);
        }
    }

    // f(x)^2
    FPS& square_inplace() {
        if (this->size() == 0) {
            return *this;
        }
        if (!any) {
            int n = this->size(), sz = 1 << std::__lg(2*(n+n-1)-1);
            if (n <= 60) return *this = convolution_naive(*this,*this);
            this->resize(sz), this->ntt(false);
            for (int i = 0; i < sz; i++) this->a[i] *= this->a[i];
            this->ntt(true), this->resize(n+n-1);
            return *this;
        }
        else {
            int n = this->a.size();
            static constexpr int mod0 = 998244353, mod1 = 1300234241, mod2 = 1484783617;
            FormalPowerSeries< mod0 > f0(n);
            FormalPowerSeries< mod1 > f1(n);
            FormalPowerSeries< mod2 > f2(n);
            for (int i = 0; i < n; i++) f0.a[i] = this->a[i].x, f1.a[i] = this->a[i].x, f2.a[i] = this->a[i].x;
            f0.square_inplace();
            f1.square_inplace();
            f2.square_inplace();
            crt(*this,f0,f1,f2);
            return *this;
        }
    }
    FPS square() const { return FPS(*this).square_inplace(); }

    // f'(x)
    FPS& differential_inplace() {
        const int n = (int)this->a.size();
        assert(n > 0);
        for(int i = 1; i < n; i++) this->a[i-1] = this->a[i] * i;
        this->a[n-1] = 0;
        return *this;
    }
    FPS differential() const { return FPS(*this).differential_inplace(); }

    // ∫f(x)dx
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

    // log(f(x)) 定数項は1である
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

    // exp(f(x)) 定数項は0である
    FPS& exp_inplace(int deg = -1) {
        if (!any) {
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
        else {
            assert(this->size() == 0 || this->a[0] == 0);
            if (deg == -1) deg = (int)this->size();
            FPS r({1});
            for (int i = 1; i < deg; i <<= 1) {
                r = (r*(this->low(i << 1)+mint<MOD>(1)-r.log(i << 1))).low(i << 1);
            }
            return *this = r.low(deg);
        }
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

    // f(x)^k O(NK) N:長さ K:項数
    // 定数項は0でない
    FPS pow_sparse(const long long k) {
        assert(this->size() > 0 && this->a[0] != 0);
        FPS g(this->size());
        std::vector<int> fl;
        mint<MOD> invf0 = this->a[0].inv();
        std::vector<long long> inv(g.size(),1);
        g[0] = this->a[0].pow(k);
        for (int i = 2; i < (int)g.size(); i++) inv[i] = MOD - (MOD / i) * inv[MOD%i] % MOD;
        for (int i = 0; i < (int)this->size(); i++) {
            if (this->a[i] == 0) continue;
            fl.push_back(i);
        }
        for (int i = 1; i < (int)g.size(); i++) {
            for (int j = 0; j < (int)fl.size(); j++) {
                int p = fl[j];
                if (1 <= p && p <= i) g[i] += this->a[p]*g[i-p]*p*k;
                if (1 <= p && p <= i-1) g[i] -= this->a[p]*g[i-p]*(i-p);
            }
            g[i] = g[i]*this->a[0]*inv[i];
        }
        return g;
    }

    // sqrt(f(x))
    // 存在しないなら長さ0になる
    FPS& sqrt_inplace(int deg = -1) {
        if (deg == -1) deg = this->size();
        int n = this->size(), z = 0;
        for(;z<n&&this->a[z]==0;z++);
        if(z==n) {this->resize(deg); return *this;}
        if(z%2) return *this = {};
        mint<MOD> w = this->a[z].sqrt();
        if(w*w!=this->a[z]) return *this = {};
        int s=deg-z/2;
        mint<MOD> az = this->a[z];
        this->a.erase(this->a.begin(),this->a.begin()+z);
        *this /= az;
        if (!any) *this = this->_sqrt(s);
        else {
            FPS g({1});
            mint<MOD> two_inv = mint<MOD>(2).inv();
            for (int i = 1; i < s; i*=2) {
                g.resize(i*2);
                g += (*this).low(i*2)*g.inverse();
                g *= two_inv;
            }
            *this = g.low(s);
        }
        *this *= w;
        this->a.insert(this->a.begin(),z/2,0);
        return *this;
    }
    FPS sqrt(int deg = -1) const { return FPS(*this).sqrt_inplace(deg); }

    // f(x+c)
    FPS& shift_inplace(const mint<MOD> &c) {
        int n = this->size();
        Fact fc(n);
        for (int i = 0; i < n; i++) this->a[i] *= fc.FACT[i];
        reverse(this->a.begin(), this->a.end());
        FPS g(n);
        mint<MOD> cp = 1; 
        for (int i = 0; i < n; i++) g[i] = cp * fc.IFACT[i], cp *= c;
        this->convolution_inplace(g);
        this->a.resize(n);
        reverse(this->a.begin(), this->a.end());
        for (int i = 0; i < n; i++) this->a[i] *= fc.IFACT[i];
        return *this;
    }
    FPS shift(const mint<MOD> &c) const { return FPS(*this).shift_inplace(c); }
    
    // O(NlogN+Mlog^2M)
    std::vector<mint<MOD>> multipoint_evaluation(const std::vector<mint<MOD>> &xs) {
        auto g = subproduct_tree(xs);
        int m = (int) xs.size(), k = (int) g.size() / 2;
        g[1] = (*this) % g[1];
        for(int i = 2; i < k + m; i++) g[i] = g[i >> 1] % g[i];
        std::vector<mint<MOD>> ys(m);
        for(int i = 0; i < m; i++) {
            ys[i] = (g[k + i].size() == 0 ? mint<MOD>(0) : g[k + i][0]);
        }
        return ys;
    }
    std::vector<mint<MOD>> multipoint_evaluation(const FPS &xs) {
        return multipoint_evaluation(xs.a);
    }

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