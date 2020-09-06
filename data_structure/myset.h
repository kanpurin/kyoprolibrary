#ifndef _MYSET_H_
#define _MYSET_H_

#include <cassert>
#include <iostream>

// 平衡二分木
template<typename T>
struct Myset {
private:
    struct node_t {
        node_t *ch[2]; int sz;
        T x;
        node_t(T x) {
            ch[0] = ch[1] = nullptr;
            sz = 1;
            this->x = x;
        }
    };

    node_t *tr; // 本体

    int sz(node_t *n) { return (n==nullptr) ? 0 : n->sz; }
    int wei(node_t *n) { return sz(n)+1; }
    void update(node_t *n) {
        assert(n != nullptr);
        n->sz = 1 + sz(n->ch[0]) + sz(n->ch[1]);
    }
    // 回転
    node_t *rotate(node_t *n, int ty) {
        // ty = 0: ((a, b), c) -> (a, (b, c))
        node_t *m = n->ch[ty];
        n->ch[ty] = m->ch[1-ty];
        m->ch[1-ty] = n;
        update(n); update(m);
        return m;
    }
    // バランスをとってそう
    node_t *balance(node_t *n) {
        for (int f = 0; f <= 1; f++) {
            if (wei(n->ch[f])*2 > wei(n->ch[1-f])*5) {
                if (wei(n->ch[f]->ch[f])*3 < wei(n->ch[f]->ch[1-f])*2) {
                    n->ch[f] = rotate(n->ch[f], 1-f);
                }
                return rotate(n, f);
            }
        }
        return n;
    }
    node_t *_insert(node_t *n, T x) {
        if (n == nullptr) return new node_t(x);
        if (x <= n->x) {
            n->ch[0] = _insert(n->ch[0], x);
        } else {
            n->ch[1] = _insert(n->ch[1], x);
        }
        update(n);
        return balance(n);
    }
    T _at(node_t *n, int k) {
        assert(n != nullptr);
        assert(0 <= k && k < sz(n));
        if (k < sz(n->ch[0])) {
            return _at(n->ch[0], k);
        }
        if (k == sz(n->ch[0])) return n->x;
        return _at(n->ch[1], k - sz(n->ch[0]) - 1);
    }
    int _lower_bound(node_t *n, T x) {
        if (n == nullptr) return 0;
        if (x <= n->x) return _lower_bound(n->ch[0], x);
        return sz(n->ch[0]) + 1 + _lower_bound(n->ch[1], x);
    }
    int _upper_bound(node_t *n, T x) {
        if (n == nullptr) return 0;
        if (x < n->x) return _upper_bound(n->ch[0], x);
        return sz(n->ch[0]) + 1 + _upper_bound(n->ch[1], x);
    }
    node_t *_erase(node_t *n, int k) {
        if (k < sz(n->ch[0])) {
            n->ch[0] = _erase(n->ch[0], k);
        } else if (k > sz(n->ch[0])) {
            n->ch[1] = _erase(n->ch[1], k - sz(n->ch[0]) - 1);
        } else {
            if (n->ch[0] == nullptr) return n->ch[1];
            n->x = _at(n->ch[0], sz(n->ch[0])-1);
            n->ch[0] = _erase(n->ch[0], sz(n->ch[0]) - 1);
        }
        update(n);
        return balance(n);
    }
public:
    Myset() { this->tr = nullptr; }
    int size() { return sz(this->tr); }
    
    // xを挿入
    void insert(T x) { this->tr = _insert(this->tr, x); }

    // xを挿入(既にある場合は挿入しない)
    void insert_unique(T x) { 
        int t = _lower_bound(this->tr, x);
        if (t < sz(this->tr)) this->tr = _insert(this->tr, x); 
    }
    
    // 昇順k番目の要素を取得
    T get_kth(int k) { return _at(this->tr,k); }

    // 最大の要素を取得
    T get_max() { return _at(this->tr,sz(this->tr)-1); }

    // 最小の要素を取得
    T get_min() { return _at(this->tr,0); }

    // x以上で最小が昇順何番目か 
    // x以上がないならsize番目
    int lower_bound(T x) { return _lower_bound(this->tr,x); }

    // x以上で最小が昇順何番目か 
    // x以上がないならsize番目
    int upper_bound(T x) { return _upper_bound(this->tr,x); }

    // 昇順k番目の削除
    void erase_kth(int k) { this->tr = _erase(this->tr, k); }

    // xを削除
    void erase(T x) {
        int t = _lower_bound(this->tr, x);
        if (t < sz(this->tr)) this->tr = _erase(this->tr, t); 
    }

    // xを全て削除
    void erase_all(T x) {
        int t = _lower_bound(this->tr, x);
        while(t < sz(this->tr)) this->tr = _erase(this->tr, t); 
    }

    // x以下の数
    int count_less(T x) {
        return _upper_bound(this->tr, x);
    }    
    
    // x以上の数
    int count_more(T x) {
        return sz(this->tr) - _lower_bound(this->tr, x);
    }

    // デバッグ用
    void print() {
        std::cout << "[ ";
        for (int i = 0; i < sz(this->tr); i++) {
            std::cout << _at(this->tr,i) << " ";
        }
        std::cout << "]" << std::endl;
    }
};

#endif
