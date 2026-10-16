#include "code/probability/walk_line.hpp"
#include "code/probability/markov_absorb.hpp"

int main()
{
    for (int n = 1; n <= 25; n++)
    {
        auto [prob, mean] = walk_line(n, .5);
        for (int i = 0; i <= n; i++)
        {
            assert(abs(prob[i] - (db)i / n) < 1e-11);
            assert(abs(mean[i] - (db)i * (n - i)) < 1e-9);
        }
        for (db bias : {0.0, .1, .3, .7, .9, 1.0})
        {
            tie(prob, mean) = walk_line(n, bias);
            vector<vector<db>> p(n + 2, vector<db>(n + 2));
            vector<int> end(n + 2); end[1] = end[n + 1] = 1;
            for (int i = 1; i < n; i++) p[i + 1][i] = 1 - bias, p[i + 1][i + 2] = bias;
            auto ref = markov_absorb(p, end);
            for (int i = 1; i < n; i++)
            {
                assert(abs(mean[i] - ref.mean[i + 1]) < 1e-8);
                assert(abs(prob[i] - bias * (i == n - 1 ? 1 : prob[i + 1]) - (1 - bias) * prob[i - 1]) < 1e-11);
            }
        }
    }
    cout << "PASS line walk: fair-walk formulas, biased systems and deterministic boundaries\n";
}
