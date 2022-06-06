#ifndef _SEGMENT_SET_HPP_
#define _SEGMENT_SET_HPP_

#include <map>
#include <cassert>

struct SegmentSet {
private:
    std::map<int,int> mp; // (右端,左端)
public:
    SegmentSet(){}
    auto begin() { return mp.begin(); }
    auto end() { return mp.end(); }
    // (r,l)を返す
    auto get(int x) {
        auto it = mp.lower_bound(x);
        if (it == mp.end() || x < it->second) return mp.end();
        return it;
    }
    // [l,r]を追加
    void insert(int l,int r) {
        assert(l <= r);
        auto it1 = get(l);
        if (it1 != mp.end()) l = it1->second;
        auto it2 = get(r);
        if (it2 != mp.end()) r = it2->first;
        for (auto it = mp.lower_bound(l); it != mp.end(); ) {
            if (r < it->first) break;
            it = mp.erase(it);
        }
        mp.insert({r,l});
    }
};

#endif