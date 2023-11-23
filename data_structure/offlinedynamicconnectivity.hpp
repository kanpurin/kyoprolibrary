#ifndef _OFFLINE_DYNAMIC_CONNECTIVITY_HPP_
#define _OFFLINE_DYNAMIC_CONNECTIVITY_HPP_

#include <vector>
#include <functional>
#include <stack>

#include "rollbackunionfind.hpp"

// 連結成分内の値の総和(sum)(A)の総和(fold)(B)
// A:可換半群,B:可換モノイド
template<typename T>
struct OfflineDynamicConnectivity {
private:
    struct GetQuery {
        int q; // same,size,sum,fold
        int u,v;
    };
    struct UpdateQuery {
        int q; // connect,add
        int u,v,l,r;
        T x;
    };
    using Func = std::function< T(T, T) >;
    std::vector<std::vector<GetQuery>> getquery;
    std::vector<UpdateQuery> updatequery;
    bool updquery = true;
    RollbackUnionFind<T> uf;
    std::unordered_map<long long, std::pair<int,int>> mp;
    inline long long edge2ID(int u, int v) {
        return static_cast<long long>(u)<<32 | v;
    }
    std::pair<int,int> ID2edge(long long id) {
        return {id,id>>32};
    }
    void _getQuery(int q,int u,int v) {
        int sz = getquery.size();
        if (updquery) {
            getquery.resize(++sz);
            updquery = false;
        }
        getquery[sz-1].emplace_back(GetQuery{q,u,v});
    }
public:
    std::vector<std::pair<int,T>> ans;

    OfflineDynamicConnectivity(int n) {
        uf = RollbackUnionFind<T>(n);
    }
    OfflineDynamicConnectivity(std::vector<T> v, const Func f) {
        uf = RollbackUnionFind<T>(v,f);
    }
    OfflineDynamicConnectivity(int n, const T &val, const Func f) {
        uf = RollbackUnionFind<T>(n,val,f);
    }
    OfflineDynamicConnectivity(std::vector<T> v, const Func fa, const Func fb, const T &unityB) {
        uf = RollbackUnionFind<T>(v,fa,fb,unityB);
    }
    OfflineDynamicConnectivity(int n, const T &val, const Func fa, const Func fb, const T &unityB) {
        uf = RollbackUnionFind<T>(n,val,fa,fb,unityB);
    }

    void connect(int u,int v) {
        if (u > v) std::swap(u,v);
        long long id = edge2ID(u,v);
        if (mp[id].first == 0) {
            updquery = true;
            mp[id].second = getquery.size(); // 開始位置を記録
        }
        mp[id].first++;
    }

    void disconnect(int u,int v) {
        if (u > v) std::swap(u,v);
        long long id = edge2ID(u,v);
        if (mp.find(id) == mp.end()) return;
        mp[id].first--;
        if (mp[id].first == 0) {
            updquery = true;
            int l = mp[id].second;
            int r = getquery.size();
            updatequery.emplace_back(UpdateQuery{0,u,v,l,r,T()});
            mp.erase(id);
        }
    }

    // val[x] <- val[x] + val
    void add(int v, T val) {
        updquery = true;
        int l = getquery.size();
        updatequery.emplace_back(UpdateQuery{1,-1,v,l,-1,val});
    }

    void same(int u, int v) {
        _getQuery(0,u,v);
    }

    void size(int v) {
        _getQuery(1,-1,v);
    }

    void sum(int v) {
        _getQuery(2,-1,v);
    }

    void fold() {
        _getQuery(3,-1,-1);
    }

    void build() {
        int sz = getquery.size();
        int n = 1;
        while(n < sz) n <<= 1;
        std::vector<std::vector<UpdateQuery>> node(2*n-1);
        
        for (auto [id,bgn] : mp) {
            auto [u,v] = ID2edge(id);
            int l = bgn.second;
            int r = -1;
            updatequery.emplace_back(UpdateQuery{0,u,v,l,r,T()});
        }

        for (int i = 0; i < updatequery.size(); i++) {
            int l = updatequery[i].l;
            int r = updatequery[i].r;
            if (r == -1 || r >= n) r = n;
            for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
                if (l & 1) node[l++ - 1].emplace_back(updatequery[i]);
                if (r & 1) node[--r - 1].emplace_back(updatequery[i]);
            }
        }
        std::stack<int> sta;
        sta.push(0);
        while(!sta.empty()) {
            int now = sta.top(); sta.pop();
            for (int i = 0; i < node[now].size(); i++) {
                auto [q,u,v,l,r,x] = node[now][i];
                if (q == 0) uf.unite(u,v);
                else uf.add(v,x);       
            }
            if (now+1<n) {
                sta.push(2*now+2);
                sta.push(2*now+1);
                uf.snapshot();
            }
            else {
                if (now-n+1 >= (int)getquery.size()) break;
                for (int i = 0; i < getquery[now-n+1].size(); i++) {
                    auto [q,u,v] = getquery[now-n+1][i];
                    dump3(q,u,v);
                    // same,size,sum,fold
                    if (q == 0) {
                        ans.push_back({uf.same(u,v),T()});
                    }
                    else if (q == 1) {
                        ans.push_back({uf.size(v),T()});
                    }
                    else if (q == 2) {
                        ans.push_back({-1,uf.sum(v)});
                    }
                    else if (q == 3) {
                        ans.push_back({-1,uf.fold()});
                    }
                }
                if (sta.empty()) break;
                uf.rollback();
            }
        }
    }
};

#endif
