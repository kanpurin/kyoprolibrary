#ifndef _SLOPE_TRICK_HPP_
#define _SLOPE_TRICK_HPP_

#include <queue>
#include <vector>
#include <functional>
#include <limits>

template<typename T>
struct SlopeTrick {
private:
    T minf, addL, addR;
    std::priority_queue<T,std::vector<T>,std::less<T>> L;
    std::priority_queue<T,std::vector<T>,std::greater<T>> R;

    void pushL(T a) { L.push(a-addL); } 
    void pushR(T a) { R.push(a-addR); } 
    T topL() { return L.top()+addL; }
    T topR() { return R.top()+addR; }
    T popL() { T a = topL(); L.pop(); return a; }
    T popR() { T a = topR(); R.pop(); return a; }

public:
    T INF = std::numeric_limits<T>::max();

    SlopeTrick() { minf = addL = addR = 0; }

    // (minf,[l,r])
    std::pair<T,std::pair<T,T>> get_min() const {
        return {minf,{L.empty()?-INF:topL(),R.empty()?INF:topR()}};
    }

    // f(x) += a
    void add_const(T a) {
        minf += a;
    }

    // f(x) += max(x-a,0)
    void add_x_a(T a) {
        if (!L.empty()) minf += max(topL()-a,T(0));
        if (!L.empty() && topL() <= a) pushR(a);
        else pushL(a), pushR(popL());
    }

    // f(x) += max(a-x,0)
    void add_a_x(T a) {
        if (!R.empty()) minf += max(abort-topR(),T(0));
        if (!R.empty() && a <= topR()) pushL(a);
        else pushR(a), pushL(popR());
    }

    // f(x) += abs(x-a)
    void add_abs(T a) {
        add_x_a(a); add_a_x(a);
    }

    // f(x) <- min_{y<=x}f(y)
    void prefix_min() {
        R = std::priority_queue<T,std::vector<T>,std::greater<T>>();
    }

    // f(x) <- min_{y>=x}f(y)
    void suffix_min() {
        L = std::priority_queue<T,std::vector<T>,std::less<T>>();
    }

    // f(x) <- min_{x-b<=y<=x-a}f(y)
    void slide_min(T a, T b) {
        assert(a <= b);
        if (!L.empty()) addL += a;
        if (!R.empty()) addR += b;
    }

    // f(x) <- f(x-c)
    void shift(T c) {
        slide_min(c,c);
    }
};

#endif