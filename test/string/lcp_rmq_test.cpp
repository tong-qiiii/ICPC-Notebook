#include "code/string/lcp_rmq.hpp"
int main()
{
    mt19937 gen(20261016);
    LCPRMQ l("banana");
    assert(l.query(1, 3) == 3 && l.query(0, 0) == 6);
    {
        for (int run = 0; run < 500; run++)
        {
            int n = gen() % 40;
            string s;
            for (int i = 0; i < n; i++)
            {
                s += char('a' + gen() % 4);
            }
            LCPRMQ l(s);
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    int k = 0;
                    while (i + k < n && j + k < n && s[i + k] == s[j + k])
                    {
                        k++;
                    }
                    assert(l.query(i, j) == k);
                }
            }
        }
    }
    cout << "PASS lcp_rmq\n";
}
