#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct FenwickKSum
{

    int n = 0;
    vector<ll> val;
    vector<ll> bit_cnt;
    vector<ll> bit_sum;

    FenwickKSum() = default;

    FenwickKSum(vector<ll> values)
    {
        init(std::move(values));
    }

    void init(vector<ll> values)
    {
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());

        val = std::move(values);
        n = (int)val.size();

        bit_cnt.assign(n + 1, 0);
        bit_sum.assign(n + 1, 0);
    }

    void modify(ll x, ll delta)
    {
        int p = get_id(x);

        for (int i = p; i <= n; i += i & -i)
        {
            bit_cnt[i] += delta;
            bit_sum[i] += delta * x;
        }
    }

    void insert(ll x)
    {
        modify(x, 1);
    }

    void erase(ll x)
    {
        modify(x, -1);
    }

    ll size() const
    {
        return prefix_cnt(n);
    }

    ll sum() const
    {
        return prefix_sum(n);
    }

    ll count_le(ll x) const
    {
        int p = upper_bound(val.begin(), val.end(), x) - val.begin();

        return prefix_cnt(p);
    }

    ll sum_le(ll x) const
    {
        int p = upper_bound(val.begin(), val.end(), x) - val.begin();

        return prefix_sum(p);
    }

    ll kth(ll k) const
    {
        int p = kth_id(k);
        return val[p - 1];
    }

    ll sum_k(ll k) const
    {
        if (k == 0)
        {
            return 0;
        }

        int p = 0;
        ll cnt = 0;
        ll res = 0;

        for (int d = highest_pow2(); d; d >>= 1)
        {
            int q = p + d;

            if (q <= n && cnt + bit_cnt[q] < k)
            {
                p = q;
                cnt += bit_cnt[q];
                res += bit_sum[q];
            }
        }

        // p 是满足 prefix_cnt(p) < k 的最大位置，
        // 第 k 小位于 p + 1。
        return res + (k - cnt) * val[p];
    }

  private:
    int get_id(ll x) const
    {
        return (int)(lower_bound(val.begin(), val.end(), x) - val.begin()) +
               1;
    }

    ll prefix_cnt(int p) const
    {
        ll res = 0;

        for (; p; p -= p & -p)
        {
            res += bit_cnt[p];
        }

        return res;
    }

    ll prefix_sum(int p) const
    {
        ll res = 0;

        for (; p; p -= p & -p)
        {
            res += bit_sum[p];
        }

        return res;
    }

    int highest_pow2() const
    {
        int d = 1;

        while (d <= n / 2)
        {
            d <<= 1;
        }

        return d;
    }

    int kth_id(ll k) const
    {
        int p = 0;

        for (int d = highest_pow2(); d; d >>= 1)
        {
            int q = p + d;

            if (q <= n && bit_cnt[q] < k)
            {
                p = q;
                k -= bit_cnt[q];
            }
        }

        return p + 1;
    }
};
