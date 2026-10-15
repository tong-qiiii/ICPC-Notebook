#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif
#include "weighted_dsu.hpp"

inline optional<ll> get_value(WeightedDSU& dsu, int S, int u)
{
    if (!dsu.same(S, u)) return nullopt;
    return dsu.diff(S, u);
}
