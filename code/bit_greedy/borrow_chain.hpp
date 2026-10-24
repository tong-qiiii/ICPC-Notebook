#pragma once
#include <bits/stdc++.h>
using namespace std;

// 两数各有 a 个 0、b 个 1，无前导 0，差的 popcount=k。
// 要求 a>=0,b>=1,k>=0,a+b<=200000；无解返回两个空串。
inline pair<string, string> borrow_chain(int a, int b, int k)
{
    assert(a >= 0 && b >= 1 && k >= 0 && a <= 200000 - b);
    int n = a + b;
    if (!k)
    {
        string s = string(b, '1') + string(a, '0');
        return {s, s};
    }
    if (!a || b < 2 || k > n - 2)
    {
        return {"", ""};
    }
    string u(n, '0'), v(n, '0');
    u[0] = v[0] = '1';
    u[1] = v[1 + k] = '1';
    int left = b - 2;
    for (int i = 1; i < n && left; i++)
    {
        if (i == 1 || i == 1 + k)
        {
            continue;
        }
        u[i] = v[i] = '1';
        left--;
    }
    return {u, v};
}
