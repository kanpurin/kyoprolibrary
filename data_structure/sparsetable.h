#ifndef _SPARSE_TABLE_H_
#define _SPARSE_TABLE_H_

#include <vector>
#include <functional>

// <O(NlogN),O(1)>
template< typename T >
struct SparseTable {
    using Func = std::function<T(T, T)>;
    std::vector<std::vector< T > > st;
    std::vector< int > lookup;
    Func F;// min or max

    SparseTable() {}

    SparseTable(const std::vector< T > &v, Func F) : F(F) {
        int b = 0;
        while ((1 << b) <= v.size()) ++b;
        st.assign(b, std::vector< T >(1 << b));
        for(int i = 0; i < v.size(); i++) st[0][i] = v[i];
        for(int i = 1; i < b; i++) for (int j = 0; j + (1 << i) <= (1 << b); j++) st[i][j] = F(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
        lookup.resize(v.size() + 1);
        for(int i = 2; i < lookup.size(); i++) lookup[i] = lookup[i >> 1] + 1;
    }

    // [l,r)
    inline T rmq(int l, int r) {
        int b = lookup[r - l];
        return F(st[b][l], st[b][r - (1 << b)]);
    }
};

#endif
