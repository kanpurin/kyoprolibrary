#ifndef _SLIDE_MIN_H_
#define _SLIDE_MIN_H_

#include <vector>
#include <deque>

// スライド最小値 O(N)
// return[i] = min( v[i,i+k) )
template< typename T >
std::vector< T > slide_min(const std::vector< T > &v, int k)
{
    std::deque< int > deq;
    std::vector< T > ret;
    for (int i = 0; i < v.size(); i++) {
        while (!deq.empty() && v[deq.back()] >= v[i]) {
            deq.pop_back();
        }
        deq.push_back(i);
        if (i - k + 1 >= 0) {
            ret.emplace_back(v[deq.front()]);
            if (deq.front() == i - k + 1) deq.pop_front();
        }
    }
    return ret;
}

#endif
