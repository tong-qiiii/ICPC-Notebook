#include "code/geometry/line_segment.hpp"
int main()
{
    mt19937 gen(20261016);
    SegmentI a{{0, 0}, {2, 2}}, b{{0, 2}, {2, 0}};
    assert(segment_relation(a, b) == SegmentRelation::PROPER);
    assert(segment_relation(SegmentI{{0, 0}, {0, 0}}, a) ==
           SegmentRelation::TOUCH);
    {
        for (int t = 0; t < 3000; t++)
        {
            SegmentI a{{int(gen() % 21) - 10, int(gen() % 21) - 10},
                       {int(gen() % 21) - 10, int(gen() % 21) - 10}},
                b{{int(gen() % 21) - 10, int(gen() % 21) - 10},
                  {int(gen() % 21) - 10, int(gen() % 21) - 10}};
            assert(segment_relation(a, b) == segment_relation(b, a));
            assert(segment_relation(a, b) ==
                   segment_relation(toD(a), toD(b)));
        }
        assert(segment_relation(SegmentI{{0, 0}, {3, 0}},
                                SegmentI{{1, 0}, {4, 0}}) ==
               SegmentRelation::OVERLAP);
        assert(segment_relation(SegmentI{{0, 0}, {1, 0}},
                                SegmentI{{2, 0}, {3, 0}}) ==
               SegmentRelation::DISJOINT);
        LineD l = LineD::through({0, 0}, {2, 0});
        assert(same_point(projection({1, 2}, l), {1, 0}));
    }
    cout << "PASS line_segment\n";
}
