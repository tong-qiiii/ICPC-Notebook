#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include <bits/stdc++.h>
using namespace std;

inline void sosSubset(vector<ll> &f, int n)
{
    int S = 1 << n;

    for (int i = 0; i < n; ++i)
    {
        for (int mask = 0; mask < S; ++mask)
        {
            if (mask >> i & 1)
            {
                f[mask] += f[mask ^ (1 << i)];
            }
        }
    }
}

inline void mobiusSubset(vector<ll> &f, int n)
{
    int S = 1 << n;

    for (int i = 0; i < n; ++i)
    {
        for (int mask = 0; mask < S; ++mask)
        {
            if (mask >> i & 1)
            {
                f[mask] -= f[mask ^ (1 << i)];
            }
        }
    }
}

inline void sosSuperset(vector<ll> &f, int n)
{
    int S = 1 << n;

    for (int i = 0; i < n; ++i)
    {
        for (int mask = 0; mask < S; ++mask)
        {
            if (!(mask >> i & 1))
            {
                f[mask] += f[mask | (1 << i)];
            }
        }
    }
}

inline void mobiusSuperset(vector<ll> &f, int n)
{
    int S = 1 << n;

    for (int i = 0; i < n; ++i)
    {
        for (int mask = 0; mask < S; ++mask)
        {
            if (!(mask >> i & 1))
            {
                f[mask] -= f[mask | (1 << i)];
            }
        }
    }
}
