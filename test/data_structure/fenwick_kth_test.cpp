#include "code/data_structure/fenwick_kth.hpp"
int main()
{
    mt19937 gen(20261016);
    BITKth fw(5);
    fw.add(2, 2);
    fw.add(5, 1);
    assert(fw.kth(2) == 2 && fw.kth(3) == 5);
    {
        BITKth fw;
        for (int t = 0; t < 20; t++)
        {
            fw.init(30);
            vector<int> a(31);
            for (int j = 0; j < 300; j++)
            {
                int v = gen() % 30 + 1;
                if (a[v] && gen() % 2)
                {
                    a[v]--;
                    fw.add(v, -1);
                }
                else
                {
                    a[v]++;
                    fw.add(v, 1);
                }
                int c = 0;
                for (int i = 1; i <= 30; i++)
                {
                    for (int k = 0; k < a[i]; k++)
                    {
                        assert(fw.kth(++c) == i);
                    }
                    assert(fw.sum(i) == c);
                }
                assert(fw.total() == c);
            }
        }
    }
    cout << "PASS fenwick_kth\n";
}
