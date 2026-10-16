#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Add, class Del, class Ok, class Update>
void window_shortest(int n, Add add, Del del, Ok ok, Update update)
{
    int l = 1;

    for (int r = 1; r <= n; r++)
    {
        add(r);

        while (l <= r && ok())
        {
            update(l, r);
            del(l);
            l++;
        }
    }
}
