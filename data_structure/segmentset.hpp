#ifndef _SEGMENT_SET_HPP_
#define _SEGMENT_SET_HPP_

#include <map>
#include <cassert>

template<typename T>
struct SegmentSet {
private:
    std::map<T,T> mp; // (l,r)
public:
    SegmentSet(){}
    auto begin() { return mp.begin(); }
    auto end() { return mp.end(); }
    // x∈[l,r]となる(l,r)を返す.無い場合end()
    auto get(T x) {
        auto it = mp.upper_bound(x);
        if (it == mp.begin() || (--it)->second < x) return mp.end();
        return it;
    }
    // [l,r]を追加
    void insert(T l,T r) {
        assert(l <= r);
        auto itl = mp.upper_bound(l), itr = mp.upper_bound(r);
        if (itl != mp.begin() && (--itl)->second < l) itl++;
        if (itl != itr) {
            l = std::min(l , itl->first);
            r = std::max(r, prev(itr)->second);
            mp.erase(itl,itr);
        }
        mp[l] = r;
    }
    // [l,r]を削除
    void erase(T l, T r) {
        assert(l <= r);
        auto itl = mp.upper_bound(l), itr = mp.upper_bound(r);
        if (itl != mp.begin() && (--itl)->second < l) ++itl;
        if (itl == itr) return;
        T tl = std::min(l, itl->first), tr = std::max(r,prev(itr)->second);
        mp.erase(itl, itr);
        if (tl < l) mp[tl] = l-1;
        if (r < tr) mp[r+1] = tr;
    }
};

#endif