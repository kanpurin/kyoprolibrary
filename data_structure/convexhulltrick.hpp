#ifndef _CONVEX_HULL_TRICK_HPP_
#define _CONVEX_HULL_TRICK_HPP_

#include <deque>
#include <utility>

// ConvexHullTrick O(Q)
// 追加する直線の傾きは単調減少
// クエリは単調増加
// ax+bの最小値
// verify:https://yukicoder.me/problems/no/409
template<typename T>
struct ConvexHullTrick {
private:
    using P = std::pair<T,T>;
    std::deque<P> deq;
    // 直線l,m,rの内mがいるかどうか
    inline bool check(P l, P m, P r) {
        return (m.first-l.first)*(r.second-m.second) >= (m.second-l.second)*(r.first-m.first);
    }
public:
    ConvexHullTrick() {}

    // ax+bの追加 aは単調減少
    void add_query(T a, T b) {
        if (deq.size() <= 1) {
            deq.push_back(make_pair(a,b));
            return;
        }
        P m = deq.back(); deq.pop_back();
        while(!deq.empty() && check(deq.back(),m,{a,b})) {
            m = deq.back(); deq.pop_back();
        }
        deq.push_back(m); deq.push_back({a,b});
    }

    // ax+bの最小値 xは単調増加
    T get_query(T x) {
        P m = deq.front(); deq.pop_front();
        while(!deq.empty() && m.first * x + m.second >= deq.front().first * x + deq.front().second) { 
            m = deq.front(); deq.pop_front();
        }
        deq.push_front(m);
        return m.first * x + m.second;
    }
};

#endif