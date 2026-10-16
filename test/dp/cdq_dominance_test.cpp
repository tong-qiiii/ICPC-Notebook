#include "code/dp/cdq_dominance.hpp"
int main()
{
    mt19937 gen(20261016);
    CDQDominance<> s;
    assert(s.work(vector<Node>{{}, {1, 1, 1, 3}, {2, 2, 2, 4}}) == 7);
    {
        {
            CDQDominance<0, 0, 0> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((0 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (0 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (0 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<0, 0, 1> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((0 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (0 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (1 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<0, 1, 0> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((0 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (1 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (0 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<0, 1, 1> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((0 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (1 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (1 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<1, 0, 0> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((1 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (0 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (0 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<1, 0, 1> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((1 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (0 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (1 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<1, 1, 0> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((1 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (1 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (0 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
        {
            CDQDominance<1, 1, 1> solver;
            auto test = [&](auto &solver)
            {
                for (int t = 0; t < 150; t++)
                {
                    vector<Node> a(1);
                    int n = gen() % 50;
                    for (int i = 1; i <= n; i++)
                    {
                        a.push_back({(ll)(gen() % 6), (ll)(gen() % 6),
                                     (ll)(gen() % 6), (ll)(gen() % 21) - 10, 0,
                                     i, 0});
                    }
                    auto b = a;
                    sort(b.begin() + 1, b.end(),
                         [](const Node &a, const Node &b) {
                             return tie(a.x, a.y, a.z, a.id) <
                                    tie(b.x, b.y, b.z, b.id);
                         });
                    ll ref = n ? LLONG_MIN : 0;
                    for (int i = 1; i <= n; i++)
                    {
                        b[i].dp = b[i].w;
                        for (int j = 1; j < i; j++)
                        {
                            if ((1 ? b[j].x < b[i].x : b[j].x <= b[i].x) &&
                                (1 ? b[j].y < b[i].y : b[j].y <= b[i].y) &&
                                (1 ? b[j].z < b[i].z : b[j].z <= b[i].z))
                            {
                                b[i].dp = max(b[i].dp, b[j].dp + b[i].w);
                            }
                        }
                        ref = max(ref, b[i].dp);
                    }
                    assert(solver.work(a) == ref);
                }
            };
            test(solver);
        }
    }
    cout << "PASS cdq_dominance\n";
}
