#pragma once
#include <bits/stdc++.h>
using namespace std;

// B >= 变量数 + 1；增广列位于 m，不是固定 B - 1。
template<int B>
int gauss_xor(vector<bitset<B>> a, int m, bitset<B> &ans)
{
    assert(m >= 0 && m < B);
    int n = a.size(), row = 0;
    vector<int> pos(m, -1);
    for (int col = 0; col < m && row < n; col++)
    {
        int sel = row;
        while (sel < n && !a[sel][col]) sel++;
        if (sel == n) continue;
        swap(a[sel], a[row]);
        for (int i = row + 1; i < n; i++)
        {
            if (a[i][col]) a[i] ^= a[row];
        }
        pos[col] = row++;
    }
    ans.reset();
    for (int i = row; i < n; i++)
    {
        if (a[i][m]) return -1;
    }
    // 自由变量取 0；按主元逆序回代。
    for (int col = m - 1; col >= 0; col--)
    {
        if (pos[col] != -1)
        {
            ans[col] = a[pos[col]][m] ^ ((a[pos[col]] & ans).count() & 1);
        }
    }
    return row;
}
