#ifndef _SUBSET_CONVOLUTION_HPP_
#define _SUBSET_CONVOLUTION_HPP_

#include <vector>
#include <array>
// #include <cassert>

// h(s)=Σ[t⊆s]f(t)g(s\t)
// O(N^2×2^N)
// verify:https://judge.yosupo.jp/problem/subset_convolution
template<typename T>
std::vector<T> subsetconvolution(std::vector<T> &f, std::vector<T> &g) {
    const int max_size = 20;
    assert(g.size() == f.size());
    int n = 0; while(f.size() > 1<<n) n++;
    assert(f.size() == 1<<n);
    int s = f.size();
    std::vector<int> pc(s); for(int i = 1; i < s; i++) pc[i] = pc[(i-1)&i]+1;
    std::vector<std::array<T,max_size+1>> F(1<<n), G(1<<n);
    for (int i = 0; i < s; i++) {
        F[i][pc[i]] = f[i];
        G[i][pc[i]] = g[i];
    }
    // ゼータ変換
    for (int w = 1; w < s; w *= 2) {
        for (int k = 0; k < s; k += w * 2) {
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < pc[k+w+i]; j++) {
                    F[k+w+i][j] += F[k+i][j];
                    G[k+w+i][j] += G[k+i][j];
                }
            }
        }
    }
    // 多項式乗算
    for (int i = 0; i < s; i++) {
        std::array<T,max_size+1> H;
        fill(H.begin(), H.end(), 0);
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k <= n-j; k++) {
                H[j+k] += F[i][j]*G[i][k];
            }
        }
        F[i].swap(H);
    }
    // メビウス変換
    for (int w = s>>1; w; w >>= 1) {
        for (int k = 0; k < s; k += w*2) {
            for (int i = 0; i < w; ++i) {
                for (int j = pc[k+w+i]; j <= n; ++j) {
                    F[k+w+i][j] -= F[k+i][j];
                }
            }
        }
    }
    std::vector<T> ans(s);
    for (int i = 0; i < s; i++) ans[i] = F[i][pc[i]];
    return ans;
}

#endif