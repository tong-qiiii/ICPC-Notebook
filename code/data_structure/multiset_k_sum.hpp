#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct KSum
{

    int k = 0;
    ll sum_small = 0;

    multiset<ll> small;
    multiset<ll> large;

    KSum() = default;

    KSum(int k)
    {
        init(k);
    }

    void init(int k_)
    {
        k = k_;
        sum_small = 0;
        small.clear();
        large.clear();
    }

    void insert(ll x)
    {
        if (!small.empty() && x <= *small.rbegin())
        {
            small.insert(x);
            sum_small += x;
        }
        else
        {
            large.insert(x);
        }

        balance();
    }

    void erase(ll x)
    {
        auto it = small.find(x);

        if (it != small.end())
        {
            sum_small -= x;
            small.erase(it);
        }
        else
        {
            it = large.find(x);

            if (it == large.end())
            {
                return;
            }

            large.erase(it);
        }

        balance();
    }

    ll sum() const
    {
        return sum_small;
    }

    bool full() const
    {
        return (int)small.size() == k;
    }

    ll kth() const
    {
        return *small.rbegin();
    }

  private:
    void balance()
    {
        while ((int)small.size() > k)
        {
            auto it = prev(small.end());

            sum_small -= *it;
            large.insert(*it);
            small.erase(it);
        }

        while ((int)small.size() < k && !large.empty())
        {
            auto it = large.begin();

            sum_small += *it;
            small.insert(*it);
            large.erase(it);
        }

        while (!small.empty() && !large.empty() &&
               *small.rbegin() > *large.begin())
        {
            auto a = prev(small.end());
            auto b = large.begin();

            ll x = *a;
            ll y = *b;

            sum_small += y - x;

            small.erase(a);
            large.erase(b);

            small.insert(y);
            large.insert(x);
        }
    }
};
