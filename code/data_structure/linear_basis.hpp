#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct Basis
{
    static constexpr int LOG = 62;

    long long basis[LOG + 1]{};
    vector<long long> vec;
    int cnt = 0;
    bool ready = false;

    Basis()
    {
        init();
    }

    void init()
    {
        fill(basis, basis + LOG + 1, 0);
        vec.clear();
        cnt = 0;
        ready = false;
    }

    bool insert(long long x)
    {
        ready = false;

        for (int i = LOG; i >= 0; --i)
        {
            if (!((x >> i) & 1))
            {
                continue;
            }

            if (!basis[i])
            {
                basis[i] = x;
                ++cnt;
                return true;
            }

            x ^= basis[i];
        }

        return false;
    }

    bool contains(long long x) const
    {
        for (int i = LOG; i >= 0; --i)
        {
            if (!((x >> i) & 1))
            {
                continue;
            }

            if (!basis[i])
            {
                return false;
            }

            x ^= basis[i];
        }

        return true;
    }

    long long max_xor(long long x = 0) const
    {
        for (int i = LOG; i >= 0; --i)
        {
            x = max(x, x ^ basis[i]);
        }

        return x;
    }

    long long min_nonzero() const
    {
        for (int i = 0; i <= LOG; ++i)
        {
            if (basis[i])
            {
                return basis[i];
            }
        }

        return 0;
    }

    void merge(const Basis &other)
    {
        for (int i = 0; i <= LOG; ++i)
        {
            if (other.basis[i])
            {
                insert(other.basis[i]);
            }
        }
    }

    void rebuild()
    {
        for (int i = LOG; i >= 0; --i)
        {
            if (!basis[i])
            {
                continue;
            }

            for (int j = i - 1; j >= 0; --j)
            {
                if ((basis[i] >> j) & 1)
                {
                    basis[i] ^= basis[j];
                }
            }
        }

        vec.clear();
        for (int i = 0; i <= LOG; ++i)
        {
            if (basis[i])
            {
                vec.push_back(basis[i]);
            }
        }

        ready = true;
    }

    unsigned long long distinct() const
    {
        return 1ULL << cnt;
    }

    long long kth(unsigned long long k) const
    {
        if (!ready || k >= (1ULL << cnt))
        {
            return -1;
        }

        long long res = 0;

        for (int i = 0; i < cnt; ++i)
        {
            if ((k >> i) & 1)
            {
                res ^= vec[i];
            }
        }

        return res;
    }

    long long rank(long long x) const
    {
        if (!ready || !contains(x))
        {
            return -1;
        }

        long long res = 0;

        for (int i = cnt - 1; i >= 0; --i)
        {
            if ((x ^ vec[i]) < x)
            {
                x ^= vec[i];
                res |= 1LL << i;
            }
        }

        return res;
    }
};
