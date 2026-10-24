#include "code/probability/markov_absorb.hpp"

int main()
{
    vector<vector<double>> p(4, vector<double>(4));
    p[1][2] = p[1][3] = .5; p[3][3] = 1;
    auto ans = markov_absorb(p, {0, 0, 1, 0});
    assert(abs(ans.prob[1] - .5) < 1e-12 && isinf(ans.mean[1]));
    assert(ans.prob[2] == 1 && ans.mean[2] == 0);
    assert(ans.prob[3] == 0 && isinf(ans.mean[3]));
    mt19937 gen(1106);
    for (int t = 0; t < 150; t++)
    {
        int n = 2 + gen() % 6;
        p.assign(n + 1, vector<double>(n + 1));
        vector<int> end(n + 1); end[n] = 1;
        for (int u = 1; u < n; u++)
        {
            p[u][n] += .25;
            p[u][1 + gen() % n] += .25;
            p[u][1 + gen() % n] += .5;
        }
        ans = markov_absorb(p, end);
        for (int s = 1; s < n; s++)
        {
            vector<double> mass(n + 1); mass[s] = 1;
            double e = 0, hit = 0;
            for (int k = 0; k < 200; k++)
            {
                e += accumulate(mass.begin() + 1, mass.begin() + n, 0.0);
                vector<double> next(n + 1);
                for (int u = 1; u < n; u++)
                {
                    for (int v = 1; v <= n; v++) next[v] += mass[u] * p[u][v];
                }
                hit += next[n]; next[n] = 0; mass.swap(next);
            }
            assert(abs(ans.mean[s] - e) < 1e-10);
            assert(abs(ans.prob[s] - hit) < 1e-10);
        }
    }
    p.assign(3, vector<double>(3));
    p[1][1] = 1 - 1e-12; p[1][2] = 1e-12;
    ans = markov_absorb(p, {0, 0, 1});
    assert(abs(ans.prob[1] - 1) < 1e-12 && abs(ans.mean[1] / 1e12 - 1) < 1e-12);
    for (db rare : {1e-12, 1e-100, 1e-200})
    {
        p.assign(4, vector<db>(4));
        p[1][2] = 1; p[2][1] = 1 - rare; p[2][3] = rare;
        ans = markov_absorb(p, {0, 0, 0, 1});
        assert(abs(ans.prob[1] - 1) < 1e-12);
        assert(abs(ans.mean[1] * rare / 2 - 1) < 1e-12);
        // 状态换序不能改变答案。
        swap(p[1], p[2]);
        for (auto &row : p) swap(row[1], row[2]);
        ans = markov_absorb(p, {0, 0, 0, 1});
        assert(abs(ans.mean[2] * rare / 2 - 1) < 1e-12);
    }
    p.assign(4, vector<db>(4));
    p[1][2] = 1; p[1][3] = 1e-100; p[3][3] = 1;
    ans = markov_absorb(p, {0, 0, 1, 0});
    assert(isinf(ans.mean[1])); // 再小的陷阱概率也不能删。
    p.assign(3, vector<db>(3));
    p[1][1] = 1; p[1][2] = 1e-310;
    bool caught = false;
    try
    {
        markov_absorb(p, {0, 0, 1});
    }
    catch (const runtime_error &)
    {
        caught = true;
    }
    assert(caught); // 数值范围不足必须报错，不能伪装成数学无限。
    ans = markov_absorb(vector<vector<double>>(2, vector<double>(2)), {0, 1});
    assert(ans.mean[1] == 0 && ans.prob[1] == 1);
    cout << "PASS Markov: 150 chains vs forward tails, rare escape/cycles/permutations, traps and overflow\n";
}
