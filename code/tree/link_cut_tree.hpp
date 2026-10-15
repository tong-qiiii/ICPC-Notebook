#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

struct LCT
{
    int n = 0;
    vector<array<int, 2>> ch;
    vector<int> fa, siz, stk;
    vector<ll> val, sum, add;
    vector<char> rev;
    LCT(int n = 0)
    {
        init(n);
    }

    void init(int n_)
    {
        n = n_;
        ch.assign(n + 1, {0, 0});
        fa.assign(n + 1, 0);
        siz.assign(n + 1, 1);
        stk.assign(n + 1, 0);
        val.assign(n + 1, 0);
        sum.assign(n + 1, 0);
        add.assign(n + 1, 0);
        rev.assign(n + 1, 0);
        siz[0] = 0;
    }

    void makeroot(int x)
    {
        access(x);
        splay(x);
        apply_rev(x);
    }

    int findroot(int x)
    {
        access(x);
        splay(x);
        push_down(x);
        while (ch[x][0])
        {
            x = ch[x][0];
            push_down(x);
        }
        splay(x);
        return x;
    }

    bool connected(int x, int y)
    {
        if (x == y)
        {
            return true;
        }
        return findroot(x) == findroot(y);
    }

    void split(int x, int y)
    {
        makeroot(x);
        access(y);
        splay(y);
    }

    bool link(int x, int y)
    {
        makeroot(x);
        if (findroot(y) == x)
        {
            return false;
        }
        fa[x] = y;
        return true;
    }

    bool cut(int x, int y)
    {
        split(x, y);
        if (ch[y][0] != x || ch[x][1])
        {
            return false;
        }
        ch[y][0] = 0;
        fa[x] = 0;
        push_up(y);
        return true;
    }

    void set_val(int x, ll v)
    {
        access(x);
        splay(x);
        val[x] = v;
        push_up(x);
    }

    void path_add(int x, int y, ll v)
    {
        split(x, y);
        apply_add(y, v);
    }

    ll path_sum(int x, int y)
    {
        split(x, y);
        return sum[y];
    }
private:
    bool is_root(int x) const
    {
        int f = fa[x];
        return ch[f][0] != x && ch[f][1] != x;
    }

    void push_up(int x)
    {
        siz[x] = siz[ch[x][0]] + siz[ch[x][1]] + 1;
        sum[x] = sum[ch[x][0]] + sum[ch[x][1]] + val[x];
    }

    void apply_rev(int x)
    {
        if (!x)
        {
            return;
        }
        swap(ch[x][0], ch[x][1]);
        rev[x] ^= 1;
    }

    void apply_add(int x, ll v)
    {
        if (!x)
        {
            return;
        }
        val[x] += v;
        sum[x] += v * siz[x];
        add[x] += v;
    }

    void push_down(int x)
    {
        if (rev[x])
        {
            apply_rev(ch[x][0]);
            apply_rev(ch[x][1]);
            rev[x] = 0;
        }
        if (add[x])
        {
            apply_add(ch[x][0], add[x]);
            apply_add(ch[x][1], add[x]);
            add[x] = 0;
        }
    }

    void rotate(int x)
    {
        int y = fa[x];
        int z = fa[y];
        int k = (ch[y][1] == x);
        int w = ch[x][k ^ 1];
        if (!is_root(y))
        {
            ch[z][ch[z][1] == y] = x;
        }
        fa[x] = z;
        ch[x][k ^ 1] = y;
        fa[y] = x;
        ch[y][k] = w;
        if (w)
        {
            fa[w] = y;
        }
        push_up(y);
        push_up(x);
    }

    void splay(int x)
    {
        int top = 0;
        int y = x;
        stk[++top] = y;
        while (!is_root(y))
        {
            stk[++top] = y = fa[y];
        }
        while (top)
        {
            push_down(stk[top--]);
        }
        while (!is_root(x))
        {
            int y = fa[x];
            int z = fa[y];
            if (!is_root(y))
            {
                if ((ch[y][1] == x) == (ch[z][1] == y))
                {
                    rotate(y);
                }
                else
                {
                    rotate(x);
                }
            }
            rotate(x);
        }
    }

    void access(int x)
    {
        for (int y = 0; x; x = fa[y = x])
        {
            splay(x);
            ch[x][1] = y;
            push_up(x);
        }
    }
};
