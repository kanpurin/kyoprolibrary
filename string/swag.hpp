#ifndef _SWAG_HPP_
#define _SWAG_HPP_

#include <stack>
#include <functional>
#include <cassert>

// deque内部の半群の総積を求める
// 全クエリO(1)
template<typename T>
struct SWAG {
private:
    std::stack<T> sta1,sta2;
    std::stack<T> prod1, prod2;
    std::function<T(T,T)> func;
    T _pop(std::stack<T> &sta_, std::stack<T> &prod_) {
        T v = sta_.top();
        sta_.pop();
        prod_.pop();
        return v;
    }
public:
    SWAG(std::function<T(T,T)> func) : func(func) {}
    bool empty() const {
        return sta1.empty() && sta2.empty();
    }
    void push_front(T v) {
        if (sta1.empty()) prod1.push(v);
        else prod1.push(func(v,prod1.top()));
        sta1.push(v);
    }
    void push_back(T v) {
        if (sta2.empty()) prod2.push(v);
        else prod2.push(func(prod2.top(),v));
        sta2.push(v);
    }
    void pop_front() {
        assert(!empty());
        if (sta1.empty()) {
            std::stack<T> tmpsta;
            while(sta2.size() > tmpsta.size() + 1) {
                T v = _pop(sta2,prod2);
                tmpsta.push(v);
            }
            while(!sta2.empty()) {
                T v = _pop(sta2,prod2);
                push_front(v);
            }
            while(!tmpsta.empty()) {
                T v = tmpsta.top(); tmpsta.pop();
                push_back(v);
            }
        }
        sta1.pop();
        prod1.pop();
    }
    void pop_back() {
        assert(!empty());
        if (sta2.empty()) {
            std::stack<T> tmpsta;
            while(sta1.size() > tmpsta.size() + 1) {
                T v = _pop(sta1,prod1);
                tmpsta.push(v);
            }
            while(!sta1.empty()) {
                T v = _pop(sta1,prod1);
                push_back(v);
            }
            while(!tmpsta.empty()) {
                T v = tmpsta.top(); 
                tmpsta.pop();
                push_front(v);
            }
        }
        sta2.pop();
        prod2.pop();
    }
    T prod() {
        assert(!empty());
        if (!sta1.empty() && !sta2.empty()) {
            return func(prod1.top(),prod2.top());
        }
        else if (!sta1.empty()) {
            return prod1.top();
        }
        else {
            return prod2.top();
        }
    }
};

#endif