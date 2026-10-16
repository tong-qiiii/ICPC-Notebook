#include "code/data_structure/sparse_table.hpp"
int main()
{
    mt19937 gen(20261016);
    STMax st(vector<int>{0, -3, -1, -2});
    assert(st.query(1, 3) == -1);
    {
        STMax st;
        for (int n = 1; n < 60; n++)
        {
            vector<int> a(n + 1);
            for (int i = 1; i <= n; i++)
            {
                a[i] = int(gen() % 31) - 15;
            }
            st.build(a);
            for (int l = 1; l <= n; l++)
            {
                for (int r = l; r <= n; r++)
                {
                    assert(st.query(l, r) ==
                           *max_element(a.begin() + l, a.begin() + r + 1));
                }
            }
        }
    }
    cout << "PASS sparse_table\n";
}
