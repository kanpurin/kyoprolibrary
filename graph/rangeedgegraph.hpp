#ifndef _RANGE_EDGE_GRAPH_H_
#define _RANGE_EDGE_GRAPH_H_

#include <vector>
#include <cassert>

// 区間に辺を張れる最短路問題 辺の数O(V+ElogV)
// G:最短路問題アルゴリズム T:値の型
// verify:https://atcoder.jp/contests/nikkei2019-2-qual/tasks/nikkei2019_2_qual_d
template< typename G , typename T >
struct RangeEdgeGraph {
private:
    int V;
    int n;
public:
    G graph;
    
    RangeEdgeGraph(int v) : V(v) {
        n = 1;
        while (n < v) n <<= 1;
        graph = G(n * 3 - 2);
        for (int i = 0; i < n-1; i++) {
            graph.add_edge(i+n,(2*i+1+n)%(2*n-1),0,true);
            graph.add_edge(i+n,(2*i+2+n)%(2*n-1),0,true);
        }
        for (int i = 0; i < n-1; i++) {
            graph.add_edge((2*i+1+2*n-1)%(3*n-2),i+2*n-1,0,true);
            graph.add_edge((2*i+2+2*n-1)%(3*n-2),i+2*n-1,0,true);
        }
    }

    // [from_l,from_r)から[to_l,to_r)への有向辺の追加
    void range_add_edge(int from_l, int from_r, int to_l, int to_r, T weight) {
        assert(from_l < from_r);
        assert(to_l < to_r);
        from_l = std::max(0,from_l);
        from_r = std::min(V,from_r);
        to_l = std::max(0,from_l);
        to_r = std::min(V,to_r);
        int v1 = graph.add_vertex();
        int v2 = graph.add_vertex();
        for (from_l += n, from_r += n; from_l < from_r; from_l >>= 1, from_r >>= 1) {
            if (from_l & 1) graph.add_edge((from_l++-1+2*n-1)%(3*n-2),v1,0,true);
            if (from_r & 1) graph.add_edge((--from_r-1+2*n-1)%(3*n-2),v1,0,true);
        }
        for (to_l += n, to_r += n; to_l < to_r; to_l >>= 1, to_r >>= 1) {
            if (to_l & 1) graph.add_edge(v2,(to_l++-1+n)%(2*n-1),0,true);
            if (to_r & 1) graph.add_edge(v2,(--to_r-1+n)%(2*n-1),0,true);
        }
        graph.add_edge(v1,v2,weight,true);
    }

    // 辺の追加
    // 有向の場合 directed = true
    void add_edge(int from, int to, T weight, bool directed = false) {
        graph.add_edge(from, to, weight, directed);
    }
};

#endif