#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct RollbackDSU
{
    struct Change
    {
        int x, y, sy;
    };

    vector<int> fa, siz;
    vector<Change> stk;

    RollbackDSU(int n = 0)
    {
        init(n);
    }

    void init(int n)
    {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        iota(fa.begin(), fa.end(), 0);
        stk.clear();
    }

    int find(int x) const

    {
        while (fa[x] != x)
            x = fa[x];
        return x;
    }

    bool merge(int x, int y)
    {
        x = find(x);
        y = find(y);

        if (x == y)
        {
            stk.push_back({-1, -1, 0});
            return false;
        }

        if (siz[x] > siz[y])
            swap(x, y);

        stk.push_back({x, y, siz[y]});

        fa[x] = y;
        siz[y] += siz[x];
        return true;
    }

    void undo()
    {
        assert(!stk.empty());
        auto [x, y, sy] = stk.back();
        stk.pop_back();

        if (x == -1)
            return;

        fa[x] = x;
        siz[y] = sy;
    }

    int snapshot() const

    {
        return stk.size();
    }

    void rollback(int snap)
    {
        assert(0 <= snap && snap <= (int)stk.size());
        while ((int)stk.size() > snap)
            undo();
    }

    bool same(int x, int y) const

    {
        return find(x) == find(y);
    }

    int size(int x) const

    {
        return siz[find(x)];
    }
};
