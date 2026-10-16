#include "code/poly/ogf.hpp"
int main()
{
    mt19937 gen(20261016);
    using namespace poly998;
    assert(ogf_from_sequence({1, 2, 3}) == Poly({1, 2, 3}));
    {
        using namespace poly998;
        assert(ogf_from_sequence({}).empty());
    }
    cout << "PASS ogf\n";
}
