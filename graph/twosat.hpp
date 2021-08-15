#ifndef _TWO_SAT_HPP_
#define _TWO_SAT_HPP_

#include <vector>
#include <cassert>

#include "stronglyconnectedcomponents.hpp"

// 2-SAT O(N+M) N:変数, M:節
struct TwoSAT {
private:
    int _n;
    std::vector<bool> _answer;
    StronglyConnectedComponents scc;

    void at_most_one_naive(const std::vector<int> &x, const std::vector<bool> &t) {
        assert(x.size() == t.size());
        int _m = x.size();
        for (int i = 0; i < _m-1; i++) {
            for (int j = i+1; j < _m; j++) {
                add_clause(x[i],!t[i],x[j],!t[j]);
            }
        }
    }
public:
    TwoSAT():_n(0),scc(0){}
    explicit TwoSAT(int n) : _n(n), _answer(n), scc(2*n) {}
    void add_clause(int i, bool f, int j, bool g) {
        assert(0 <= i && i < _n);
        assert(0 <= j && j < _n);
        scc.add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
        scc.add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
    }
    // 高々一つが成り立つ
    void at_most_one(const std::vector<int> &x, const std::vector<bool> &t) {
        assert(x.size() == t.size());
        int _m = x.size();
        if (_m <= 6) {
            at_most_one_naive(x,t);
            return;
        }
        int _nn = _n;
        _n += _m;
        scc.add_vertex(_m*2);
        _answer.resize(_n);
        for (int i = 0; i < _m; i++) {
            add_clause(_nn+i,false,x[i],!t[i]);
            if (i) {
                add_clause(_nn+i,false,_nn+i-1,true);
                add_clause(x[i],!t[i],_nn+i-1,true);
            }
        }
    }
    bool satisfiable() {
        scc.build();
        auto id = scc.comp;
        for (int i = 0; i < _n; i++) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            _answer[i] = id[2 * i] < id[2 * i + 1];
        }
        return true;
    }
    std::vector<bool> answer() { return _answer; }
};

#endif
