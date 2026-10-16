#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "modint.hpp"
template <int MOD> struct Matrix
{
    using Z = Mint<MOD>;

    int n = 0;
    vector<vector<Z>> a;

    Matrix() = default;

    Matrix(int n_, bool identity = false)
    {
        init(n_, identity);
    }

    void init(int n_, bool identity = false)
    {
        n = n_;
        a.assign(n, vector<Z>(n));

        if (identity)
        {
            for (int i = 0; i < n; ++i)
            {
                a[i][i] = 1;
            }
        }
    }

    vector<Z> &operator[](int i)
    {
        return a[i];
    }

    const vector<Z> &operator[](int i) const
    {
        return a[i];
    }

    Matrix operator*(const Matrix &o) const
    {
        Matrix res(n);

        for (int i = 0; i < n; ++i)
        {
            for (int k = 0; k < n; ++k)
            {
                if (a[i][k] == Z(0))
                {
                    continue;
                }

                for (int j = 0; j < n; ++j)
                {
                    res[i][j] += a[i][k] * o[k][j];
                }
            }
        }

        return res;
    }

    Matrix pow(long long b) const
    {
        Matrix base = *this;
        Matrix res(n, true);

        while (b)
        {
            if (b & 1)
            {
                res = res * base;
            }

            base = base * base;
            b >>= 1;
        }

        return res;
    }
};
