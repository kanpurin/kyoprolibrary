#ifndef _DYNAMIC_SEGMENT_TREE_HPP_
#define _DYNAMIC_SEGMENT_TREE_HPP_

#include <functional>
#include <vector>
#include <iostream>
#include <memory>
#include <cassert>

template< typename Monoid >
struct DynamicSegmentTree {
private:
    struct Node;
    using Func = std::function<Monoid(Monoid,Monoid)>;
    using node_ptr = std::unique_ptr<Node>;
    Func F;
    Monoid e;
    size_t n;
    struct Node {
        size_t index;
        Monoid value,prod;
        node_ptr left, right;
        Node(size_t index, Monoid value) : index(index),
                                           value(value),
                                           prod(value),
                                           left(nullptr),
                                           right(nullptr) {}
    };
    
    void node_update(node_ptr &np) const {
        np->prod = F(F(np->left?np->left->prod:e,np->value),
                       np->right?np->right->prod:e);
    }

    void _update(node_ptr& t, size_t a, size_t b, size_t p, Monoid &val) const {
        if (!t) {
            t = std::make_unique<Node>(p, val);
            return;
        }
        if (t->index == p) {
            t->value = val;
            node_update(t);
            return;
        }
        size_t c = (a+b)/2;
        if (p < c) {
            if (t->index < p) swap(t->index,p), swap(t->value,val);
            _update(t->left,a,c,p,val);
        }
        else {
            if (p < t->index) swap(p,t->index), swap(val,t->value);
            _update(t->right,c,b,p,val);
        }
        node_update(t);
    }

    Monoid _get(const node_ptr& t, size_t a, size_t b, size_t l, size_t r) const {
        if (!t || b <= l || r <= a) return e;
        if (l <= a && b <= r) return t->prod;
        size_t c = (a + b) / 2;
        Monoid result = _get(t->left, a, c, l, r);
        if (l <= t->index && t->index < r) result = F(result, t->value);
        return F(result, _get(t->right, c, b, l, r));
    }

    size_t _binary_search(const node_ptr& t, 
                       size_t a, size_t b, 
                       const std::function<bool(Monoid)> &f, 
                       Monoid &s, 
                       size_t l, size_t r) {
        if (!t || r <= a || b <= l) return n;
        if (a <= l && r <= b && !f(F(s,t->prod))) {
            s = F(s,t->prod);
            return n;
        }
        size_t ret = _binary_search(t->left,a,b,f,s,l,(r-l)/2+l);
        if (ret != n) return ret;
        if (a <= t->index) {
            s = F(s,t->value);
            if (f(s)) return t->index;
        }
        return _binary_search(t->right,a,b,f,s,(r-l)/2+l,r);
    }

    void _print(node_ptr& t) {
        if (!t) return;
        _print(t->left);
        _print(t->right);
        std::cout << "[" << t->index << "] : " << t->value << std::endl;
    }

    node_ptr root;
public:
    DynamicSegmentTree(size_t n,
                       const Func f,
                       const Monoid &e) : n(n),F(f),e(e),root(nullptr) {}

    void update_query(size_t x, Monoid val) {
        assert(x < n);
        _update(root, 0, n, x, val);
    }

    // [l,r)
    Monoid get_query(size_t l, size_t r) const {
        assert(l <= r && r <= n);
        return _get(root, 0, n, l, r);
    }

    // [l,r)を満たすkでf(get_query(l,k+1))=trueとなる最小のk
    // 無ければ-1
    size_t binary_search(size_t l, size_t r, const std::function<bool(Monoid)> &f) {
        Monoid s = e;
        size_t ret = _binary_search(root,l,r,f,s,0,n);
        return ret != n ? ret : -1;
    }

    Monoid operator[](int x) const {
        return get_query(x,x+1);
    }
    
    size_t size() {
        return n;
    }

    void print() {
        _print(root);
    }
};

#endif