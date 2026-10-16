#include "code/math/adaptive_simpson.hpp"
int main()
{
    mt19937 gen(20261016);
    assert(fabs(adaptive_simpson([](double x)
                                 { return sin(x); }, 0.0,
                                 acos(-1.0)) -
                2) < 1e-8);
    {
        for (int i = 0; i < 100; i++)
        {
            double a = int(gen() % 100) - 50, b = int(gen() % 100) - 50;
            auto f = [](double x)
            { return x * x * x * x; };
            assert(fabs(adaptive_simpson(f, a, b, 1e-7) -
                        (pow(b, 5) - pow(a, 5)) / 5) < 1e-5);
        }
        assert(adaptive_simpson([](double)
                                { return 1.; }, 2., 2.) == 0);
    }
    cout << "PASS adaptive_simpson\n";
}
