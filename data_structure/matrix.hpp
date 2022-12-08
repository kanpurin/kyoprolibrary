#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <vector>
#include <iostream>

// 半環上で動く
// (+,min),(min,+),(max,min),(AND,OR)など
template< class T >
struct Matrix {
    std::vector< std::vector< T > > A;

    Matrix() {}

    Matrix(size_t n, size_t m) : A(n, std::vector< T >(m, 0)) {}

    Matrix(size_t n) : A(n, std::vector< T >(n, 0)) {};

    size_t height() const {
        return (A.size());
    }

    size_t width() const {
        return (A[0].size());
    }

    inline const std::vector< T > &operator[](int k) const {
        return (A.at(k));
    }

    inline std::vector< T > &operator[](int k) {
        return (A.at(k));
    }

    static Matrix I(size_t n) {
        Matrix mat(n);
        for (int i = 0; i < n; i++) mat[i][i] = 1;
        return (mat);
    }

    Matrix &operator+=(const Matrix &B) {
        size_t n = height(), m = width();
        assert(n == B.height() && m == B.width());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                (*this)[i][j] += B[i][j];
        return (*this);
    }

    Matrix &operator-=(const Matrix &B) {
        size_t n = height(), m = width();
        assert(n == B.height() && m == B.width());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                (*this)[i][j] -= B[i][j];
        return (*this);
    }

    Matrix &operator*=(const Matrix &B) {
        size_t n = height(), m = B.width(), p = width();
        assert(p == B.height());
        std::vector< std::vector< T > > C(n, std::vector< T >(m, 0));
        for (int i = 0; i < n; i++)
            for (int k = 0; k < p; k++)
                for (int j = 0; j < m; j++)
                    C[i][j] = (C[i][j] + (*this)[i][k] * B[k][j]);
        A.swap(C);
        return (*this);
    }

    Matrix operator+(const Matrix &B) const {
        return (Matrix(*this) += B);
    }

    Matrix operator-(const Matrix &B) const {
        return (Matrix(*this) -= B);
    }

    Matrix operator*(const Matrix &B) const {
        return (Matrix(*this) *= B);
    }
    
    bool operator==(const Matrix &B) const {
        assert(this->A.size() == B.A.size() && this->A[0].size() == B.A[0].size());
        int n = this->A.size();
        int m = this->A[0].size();
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                if (this->A[i][j] != B.A[i][j]) return false;
        return true;
    }

    bool operator!=(const Matrix &B) const {
        return !(*this == B);
    }

    friend std::ostream &operator<<(std::ostream &os, Matrix &p) {
        size_t n = p.height(), m = p.width();
        for (int i = 0; i < n; i++) {
            os << "[";
            for (int j = 0; j < m; j++) {
                os << p[i][j] << (j + 1 == m ? "]\n" : ",");
            }
        }
        return (os);
    }

    // 行列式
    T determinant() {
        Matrix B(*this);
        assert(width() == height());
        T ret = 1;
        for (int i = 0; i < width(); i++) {
            int idx = -1;
            for (int j = i; j < width(); j++) {
                if (B[j][i] != 0) idx = j;
            }
            if (idx == -1) return (0);
            if (i != idx) {
                ret *= -1;
                swap(B[i], B[idx]);
            }
            ret *= B[i][i];
            T vv = B[i][i];
            for (int j = 0; j < width(); j++) {
                B[i][j] /= vv;
            }
            for (int j = i + 1; j < width(); j++) {
                T a = B[j][i];
                for (int k = 0; k < width(); k++) {
                    B[j][k] -= B[i][k] * a;
                }
            }
        }
        return (ret);
    }

    // k乗
    // 元の数は変更されない
    Matrix pow(int64_t k) const {
        auto res = I(A.size());
        auto M = *this;
        while (k > 0) {
            if (k & 1) {
                res *= M;
            }
            M *= M;
            k >>= 1;
        }
        return res;
    }
};

#endif
