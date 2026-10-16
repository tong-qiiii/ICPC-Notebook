#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

template <class Small, class Large, class Update>
void two_ends(int n, Small too_small, Large too_large, Update update)
{
    int l = 1, r = n;

    while (l < r)
    {
        if (too_small(l, r))
        {
            l++;
        }
        else if (too_large(l, r))
        {
            r--;
        }
        else
        {
            int step = update(l, r);
            if (!step)
            {
                return;
            }
            if (step > 0)
            {
                ++l;
            }
            else
            {
                --r;
            }

            // 找到答案后如何移动按题意决定，不一定同时移动两端
        }
    }
}
