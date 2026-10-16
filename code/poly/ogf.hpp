#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

#include "poly_core.hpp"
namespace poly998
{
inline Poly ogf_from_sequence(const vector<int> &seq)
{
    return Poly(seq);
}
} // namespace poly998
