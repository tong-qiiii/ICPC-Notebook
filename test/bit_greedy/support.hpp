#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline mt19937 test_rg(20261024);

inline int rnd(int l, int r)
{
    return uniform_int_distribution<int>(l, r)(test_rg);
}
