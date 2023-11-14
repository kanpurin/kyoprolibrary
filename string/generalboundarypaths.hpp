#ifndef _GENERAL_BOUNDARY_PATHS_HPP_
#define _GENERAL_BOUNDARY_PATHS_HPP_

#include <cassert>
#include <vector>
#include <stack>
#include <tuple>

#include "../data_structure/mint.hpp"
#include "../math/combination_pre.hpp"

// 上下限のある単調増加数列の数え上げ
template< int MOD >
struct GeneralBoundaryPaths {
private:
    Combination<mint<MOD>> cb;

    // maroonrk's ntt
    void ntt(std::vector<mint<MOD>> &a, bool inv){
        const int n = a.size();
        mint<MOD>*const f = a.data();
        static constexpr unsigned int mod=MOD;
        static constexpr unsigned int mod2=mod*2;
        static constexpr int L=30;
        static mint<MOD> g[L],ig[L],p2[L];
        if(g[0].x==0){
            mint<MOD> root = 2;
            while (root.pow((MOD - 1) / 2) == 1) root += 1;
            for(int i = 0;i < L;i++){
                mint<MOD> w=-root.pow(((mod-1)>>(i+2))*3);
                g[i]=w;
                ig[i]=w.inv();
                p2[i]=mint<MOD>(1<<i).inv();
            }
        }
        if(!inv){
            int b=n;
            if(b>>=1){//input:[0,mod)
                for(int i = 0; i < b; i++){
                    unsigned int x=f[i+b].x;
                    f[i+b].x=f[i].x+mod-x;
                    f[i].x+=x;
                }
            }
            if(b>>=1){//input:[0,mod*2)
                mint<MOD> p=1;
                for(int i=0,k=0;i<n;i+=b*2){
                    for(int j=i;j < i+b;j++){
                        unsigned int x=(f[j+b]*p).x;
                        f[j+b].x=f[j].x+mod-x;
                        f[j].x+=x;
                    }
                    p*=g[__builtin_ctz(++k)];
                }
            }
            while(b){
                if(b>>=1){//input:[0,mod*3)
                    mint<MOD> p=1;
                    for(int i=0,k=0;i<n;i+=b*2){
                        for(int j=i;j<i+b;j++){
                            unsigned int x=(f[j+b]*p).x;
                            f[j+b].x=f[j].x+mod-x;
                            f[j].x+=x;
                        }
                        p*=g[__builtin_ctz(++k)];
                    }
                }
                if(b>>=1){//input:[0,mod*4)
                    mint<MOD> p=1;
                    for(int i=0,k=0;i<n;i+=b*2){
                        for(int j=i;j<i+b;j++){
                            unsigned int x=(f[j+b]*p).x;
                            f[j].x=(f[j].x<mod2?f[j].x:f[j].x-mod2);
                            f[j+b].x=f[j].x+mod-x;
                            f[j].x+=x;
                        }
                        p*=g[__builtin_ctz(++k)];
                    }
                }
            }
        }else{
            int b=1;
            if(b<n/2){//input:[0,mod)
                mint<MOD> p=1;
                for(int i=0,k=0;i<n;i+=b*2){
                    for(int j=i;j<i+b;j++){
                        unsigned long long x=f[j].x+mod-f[j+b].x;
                        f[j].x+=f[j+b].x;
                        f[j+b].x=x*p.x%mod;
                    }
                    p*=ig[__builtin_ctz(++k)];
                }
                b<<=1;
            }
            for(;b<n/2;b<<=1){
                mint<MOD> p=1;
                for(int i=0,k=0;i<n;i+=b*2){
                    for(int j=i;j<i+b/2;j++){//input:[0,mod*2)
                        unsigned long long x=f[j].x+mod2-f[j+b].x;
                        f[j].x+=f[j+b].x;
                        f[j].x=(f[j].x)<mod2?f[j].x:f[j].x-mod2;
                        f[j+b].x=x*p.x%mod;
                    }
                    for(int j=i+b/2;j<i+b;j++){//input:[0,mod)
                        unsigned long long x=f[j].x+mod-f[j+b].x;
                        f[j].x+=f[j+b].x;
                        f[j+b].x=x*p.x%mod;
                    }
                    p*=ig[__builtin_ctz(++k)];
                }
            }
            if(b<n){//input:[0,mod*2)
                for(int i=0;i<b;i++){
                    unsigned int x=f[i+b].x;
                    f[i+b].x=f[i].x+mod2-x;
                    f[i].x+=x;
                }
            }
            mint<mod> z=p2[std::__lg(n)];
            for(int i=0;i<n;i++)f[i]*=z;
        }
    }
public:
    GeneralBoundaryPaths(int MAXSIZE) : cb(MAXSIZE) {}

    void rectangle_lattice_path(std::vector<mint<MOD>> &a, std::vector<mint<MOD>> &b) {
        int n = a.size()-1, m = b.size()-1;
        assert(n >= 0 && m >= 0);
        assert(a[0] == b[0]);
        if (n == 0) a[n] = b[m];
        if (m == 0) b[m] = a[n];
        if (n == 0 || m == 0) return;
        std::vector<mint<MOD>> fps(n+m-1),fpsn(n),fpsm(m),fpsa(n+1),fpsb(m+1),c,d;
        mint<MOD> c0 = b[m], d0 = a[n];
        for (int k = 0; k <= n+m-2; k++) fps[k] = cb.FACT[k];
        for (int k = 0; k < n; k++) fpsn[k]=cb.comb(m-1+k,k), fpsa[k+1]=a[k+1]*cb.IFACT[n-k-1];
        if (n != m) for (int k = 0; k < m; k++) fpsm[k]=cb.comb(n-1+k,k);
        for (int k = 0; k < m; k++) fpsb[k+1]=b[k+1]*cb.IFACT[m-k-1];
        a[0] = b[0] = 0;

        int sz = 1 << std::__lg(2*(n+m+std::max(n,m)-1)-1);
        fps.resize(sz); ntt(fps,false);
        fpsa.resize(sz); ntt(fpsa,false);
        fpsb.resize(sz); ntt(fpsb,false);
        c.resize(sz); d.resize(sz);
        for (int i = 0; i < sz; i++) c[i]=fps[i]*fpsb[i], d[i]=fps[i]*fpsa[i];
        ntt(c,true); ntt(d,true);
        for (int k = m; k < n+m; k++) c[k-m+1] = c[k] * cb.IFACT[k-m];
        for (int k = n; k < n+m; k++) d[k-n+1] = d[k] * cb.IFACT[k-n];
        c.resize(n+1); d.resize(m+1);
        c[0] = c0; d[0] = d0;

        sz = 1 << std::__lg(4*n-1);
        fpsn.resize(sz); ntt(fpsn,false);
        a.resize(sz); ntt(a,false);
        for (int i = 0; i < sz; i++) a[i] *= fpsn[i];
        ntt(a,true);
        for (int i = 1; i <= n; i++) c[i] += a[i];
        sz = 1 << std::__lg(4*m-1);
        if (n != m) fpsm.resize(sz), ntt(fpsm,false);
        b.resize(sz); ntt(b,false);
        if (n != m) for (int i = 0; i < sz; i++) b[i] *= fpsm[i];
        else        for (int i = 0; i < sz; i++) b[i] *= fpsn[i];
        ntt(b,true);
        for (int i = 1; i <= m; i++) d[i] += b[i];
        a = c, b = d;
    }

    std::vector<mint<MOD>> stairs_lattice_path(std::vector<int> h, std::vector<mint<MOD>> a) {
        assert(h.size() ==  a.size());
        std::vector<mint<MOD>> b(h.back()+1);
        std::stack<std::tuple<int,int,int,int,int>> sta;   
        sta.emplace(make_tuple(0,a.size()-1,0,-1,-1));
        while(!sta.empty()) {
            auto p = sta.top(); sta.pop();
            const int left = std::get<0>(p);
            const int right = std::get<1>(p);
            const int bottom = std::get<2>(p);
            const int mid = std::get<3>(p);
            const int tmp = std::get<4>(p);
            if (tmp == -1) {
                long long sum = 0;
                const int width = right-left;
                for (int i = left; i <= right; i++) sum += h[i]-bottom+1;
                if (width <= 1 || h[right-1] == bottom || sum <= 300000) {
                    std::vector<std::vector<mint<MOD>>> dp(width+1);
                    for (int i = left; i <= right; i++) {
                        dp[i-left].resize(h[i]-bottom+1);
                        dp[i-left][0] = a[i];
                    }
                    for (int i = left; i <= right; i++) {
                        for (int j = 1; j < dp[i-left].size(); j++) {
                            if (i >= left+1 && j < dp[i-left-1].size()) dp[i-left][j] += dp[i-left-1][j];
                            dp[i-left][j] += dp[i-left][j-1];
                        }
                    }
                    copy(dp[width].begin(),dp[width].begin()+(h[right]-bottom+1),b.begin()+bottom);
                    continue;
                }

                int mid = right;
                while(mid > left && (h[mid-1]-bottom > right-mid || right-mid > h[mid]-bottom)) mid--;
                int height = right-mid;
                {
                    int mid2;
                    long long area = 0;
                    for (int i = left; i < right; i++) {
                        if (area >= (long long)(h[i]-bottom)*(right-i)) continue;
                        area = (long long)(h[i]-bottom)*(right-i);
                        mid2 = i;
                    }
                    if (area > 10*(long long)(right-mid)*(right-mid)) {
                        mid = mid2;
                        height = h[mid]-bottom;
                    } 
                }
                sta.emplace(std::make_tuple(left,right,bottom,mid,h[mid]));
                h[mid] = height+bottom;
                sta.emplace(std::make_tuple(left,mid,bottom,-1,-1));
            }
            else {
                std::vector<mint<MOD>> a2(b.begin()+bottom,b.begin()+h[mid]+1),b2(a.begin()+mid,a.begin()+right+1);
                b2[0] = a2[0]; rectangle_lattice_path(a2,b2);
                copy(a2.begin(),a2.end(),b.begin()+bottom);
                copy(b2.begin(),b2.end(),a.begin()+mid);
                sta.emplace(std::make_tuple(mid,right,h[mid],-1,-1));
                h[mid] = tmp;
            }
        }
        return b;
    }

    // ai <= ci <= bi を満たす単調増加数列cの個数
    mint<MOD> solve(std::vector<int> a,std::vector<int> b) {
        assert(a.size() == b.size());
        assert(a.size() > 0);
        std::vector<int> a2,b2;
        for (int i = 0; i < a.size(); i++) {
            if (i > 0) a[i] = std::max(a[i],a[i-1]);
            if (i < b.size()-1) b[i] = std::min(b[i],b[i+1]);
            if (a[i] > b[i]) return 0;
        }

        for (int _ = 0; _ < 2; _++) {
            // 圧縮
            a2.clear();
            b2.clear();
            for (int i = 0; i < a.size(); i++) {
                if (a[i] == b[i]) continue;
                a2.push_back(a[i]);
                b2.push_back(b[i]);
            }
            if (a2.size() == 0) return 1;
            for (int i = a2.size()-1; i >= 0; i--) {
                b2[i] -= a2[0];
                a2[i] -= a2[0];
            }
            // 座標反転
            a2.push_back(b2.back());
            b2.push_back(b2.back());
            a.clear();
            b.clear();
            for (int i = 0; i < a2.size(); i++) {
                for (int j = i>0?b2[i-1]:0; j < b2[i]; j++) {
                    a.push_back(i);
                }
                for (int j = i>0?a2[i-1]:0; j < a2[i]; j++) {
                    b.push_back(i);
                }
            }
        }
        a.push_back(b.back());
        b.push_back(b.back());

        int now = 0, now2 = 0;
        std::vector<mint<MOD>> res(b2[now2]+1,1);
        while(b.back() > now2) {
            std::vector<int> h(b2[now2]-now+1);
            for (int i = now; i <= b2[now2]; i++) h[i-now] = b[i]-now2;
            res = stairs_lattice_path(h,res);
            now = b2[now2];
            std::swap(now,now2);
            std::swap(b,b2);
        }
        return res.back();
    }
};

#endif