#include "code/probability/markov_conditional.hpp"

int main()
{
    vector<vector<db>> p(4, vector<db>(4));
    p[1][2] = p[1][3] = .5; p[3][3] = 1;
    auto ans = markov_conditional(p, {0, 0, 1, 0});
    assert(abs(ans.prob[1] - .5) < 1e-12 && abs(ans.mean[1] - 1) < 1e-12);
    assert(ans.mean[2] == 0 && isnan(ans.mean[3]));
    p[1][1] = .5; p[1][2] = p[1][3] = .25;
    ans = markov_conditional(p, {0, 0, 1, 0});
    assert(abs(ans.mean[1] - 2) < 1e-12);
    p[1][1] = 1 - 2e-12; p[1][2] = p[1][3] = 1e-12;
    ans = markov_conditional(p, {0, 0, 1, 0});
    assert(abs(ans.prob[1] - .5) < 1e-12);
    assert(abs(ans.mean[1] / 5e11 - 1) < 1e-12);
    mt19937 gen(1117);
    for (int t = 0; t < 100; t++)
    {
        int n = 3 + gen() % 5;
        p.assign(n + 1, vector<db>(n + 1));
        vector<int> end(n + 1); end[n] = 1;
        p[n - 1][n - 1] = 1;
        for (int u = 1; u < n - 1; u++)
        {
            p[u][n] += .25; p[u][n - 1] += .25;
            p[u][1 + gen() % (n - 2)] += .5;
        }
        ans = markov_conditional(p, end);
        for (int s = 1; s < n - 1; s++)
        {
            // 独立前向枚举首次成功的质量和时间加权质量，不构造条件链。
            vector<db> mass(n + 1); mass[s] = 1;
            db hit = 0, total = 0;
            for (int k = 1; k <= 160; k++)
            {
                vector<db> next(n + 1);
                for (int u = 1; u < n - 1; u++)
                {
                    for (int v = 1; v <= n; v++) next[v] += mass[u] * p[u][v];
                }
                hit += next[n]; total += k * next[n];
                next[n] = next[n - 1] = 0; mass.swap(next);
            }
            assert(abs(ans.prob[s] - hit) < 1e-10);
            assert(abs(ans.mean[s] - total / hit) < 1e-10);
        }
    }
    ans = markov_conditional(vector<vector<db>>(2, vector<db>(2)), {0, 1});
    assert(ans.mean[1] == 0 && ans.prob[1] == 1);
    cout << "PASS conditional expectation: traps, rare escape, 100 chains vs weighted hit times\n";
}
