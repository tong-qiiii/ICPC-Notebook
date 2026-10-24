#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

// a[1..n] 是 <=2^60 的正二次幂；0<=target<2^61，可不用完材料。
inline ll split_powers(const vector<ll> &a, ll target)
{
    assert(0 <= target && target < (1LL << 61));
    array<ll, 63> cnt{};
    __int128 total = 0;
    for (int i = 1; i < (int)a.size(); i++)
    {
        assert(a[i] > 0 && !(a[i] & (a[i] - 1)) && a[i] <= (1LL << 60));
        cnt[__builtin_ctzll(a[i])]++;
        total += a[i];
    }
    if (total < target)
    {
        return -1;
    }
    ll ans = 0;
    for (int b = 0; b <= 60; b++)
    {
        if (target >> b & 1)
        {
            if (!cnt[b])
            {
                int j = b + 1;
                while (j < 62 && !cnt[j])
                {
                    j++;
                }
                assert(j < 62);
                cnt[j]--;
                while (j > b)
                {
                    j--;
                    cnt[j] += 2;
                    if (j > b)
                    {
                        cnt[j]--;
                    }
                    ans++;
                }
            }
            cnt[b]--;
        }
        cnt[b + 1] += cnt[b] / 2;
        cnt[b] %= 2;
    }
    return ans;
}
