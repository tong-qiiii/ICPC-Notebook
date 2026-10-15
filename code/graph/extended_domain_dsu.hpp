#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "dsu.hpp"

inline bool merge_domain(DSU& dsu, int n, int u, int v, bool same)
{
    if (same)
    {
        if (dsu.same(u, v + n)) return false;
        dsu.merge(u, v);
        dsu.merge(u + n, v + n);
    }
    else
    {
        if (dsu.same(u, v)) return false;
        dsu.merge(u, v + n);
        dsu.merge(u + n, v);
    }
    return true;
}
