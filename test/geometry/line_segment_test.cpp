#include "code/geometry/line_segment.hpp"

// Independent oracle for small integer coordinates: parametric fractions.
SegmentRelation oracle(SegmentI s, SegmentI t)
{
    using R = SegmentRelation;
    auto cr = [](PointI a, PointI b) { return a.x * b.y - a.y * b.x; };
    auto u = s.b - s.a, v = t.b - t.a, w = t.a - s.a;
    ll d = cr(u, v), a = cr(w, v), b = cr(w, u);
    if (d)
    {
        if (d < 0) d = -d, a = -a, b = -b;
        if (a < 0 || a > d || b < 0 || b > d) return R::DISJOINT;
        return a && a < d && b && b < d ? R::PROPER : R::TOUCH;
    }
    auto has = [&](PointI p, SegmentI z)
    {
        return cr(p - z.a, z.b - z.a) == 0 &&
               min(z.a.x, z.b.x) <= p.x && p.x <= max(z.a.x, z.b.x) &&
               min(z.a.y, z.b.y) <= p.y && p.y <= max(z.a.y, z.b.y);
    };
    set<PointI> both;
    for (auto p : {s.a, s.b, t.a, t.b})
        if (has(p, s) && has(p, t)) both.insert(p);
    return both.empty() ? R::DISJOINT : both.size() == 1 ? R::TOUCH : R::OVERLAP;
}

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
            assert(segment_relation(a, b) == oracle(a, b));
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
    vector<PointI> grid;
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++) grid.push_back({x, y});
    for (auto a : grid) for (auto b : grid)
        for (auto c : grid) for (auto d : grid)
        {
            SegmentI s{a, b}, t{c, d};
            auto rel = oracle(s, t);
            assert(segment_relation(s, t) == rel);
            assert(segment_relation(toD(s), toD(t)) == rel);
            assert(segment_intersect(s, t) == (rel != SegmentRelation::DISJOINT));
            assert(proper_segment_intersect(s, t) == (rel == SegmentRelation::PROPER));
        }
    // Subtracting these endpoints in i64 would overflow; cross products fit i128.
    ll lo = LLONG_MIN, hi = LLONG_MAX;
    SegmentI wide{{lo, 0}, {hi, 0}};
    assert(on_segment(PointI{0, 0}, wide));
    assert(on_segment_strict(PointI{0, 0}, wide));
    assert(!on_segment_strict(PointI{lo, 0}, wide));
    assert(segment_relation(wide, SegmentI{{-1, -1}, {1, 1}}) == SegmentRelation::PROPER);
    assert(segment_relation(wide, SegmentI{{-1, 0}, {1, 0}}) == SegmentRelation::OVERLAP);
    LineI vertical{{lo, 0}, {0, 1}};
    assert(side(vertical, PointI{hi, 0}) == -1);
    assert(collinear(PointI{lo, 0}, PointI{hi, 0}, PointI{0, 0}));
    assert(line_relation(vertical, LineI{{hi, 1}, {0, 1}}) == LineRelation::PARALLEL);
    SegmentD s{{0, 0}, {1, 0}};
    assert(on_segment(PointD{1 + EPS / 2, 0}, s));
    assert(!on_segment(PointD{1 + 2 * EPS, 0}, s));
    assert(segment_relation(s, SegmentD{{1 + EPS / 2, 0}, {2, 0}}) == SegmentRelation::TOUCH);
    assert(segment_relation(s, SegmentD{{1 + 2 * EPS, 0}, {2, 0}}) == SegmentRelation::DISJOINT);
    assert(segment_degenerate(SegmentD{{0, 0}, {0.8 * EPS, 0.8 * EPS}}));
    assert(!line_degenerate(LineD{{0, 0}, {0.8 * EPS, 0.8 * EPS}}));
    auto l = LineD::through({0, 0}, {1, 0});
    assert(same_side_strict(l, PointD{1, 2}, PointD{4, 3}));
    assert(opposite_side_strict(l, PointD{1, 2}, PointD{4, -3}));
    assert(on_line(PointD{8, 0}, l));
    assert(same_point(reflection({1, 2}, l), {1, -2}));
    assert(same_point(closest_point_on_segment({3, 2}, s), {1, 0}));
    assert(fabs(distance_to_segment({3, 0}, s) - 2) < EPS);
    assert(same_point(line_intersection(l, LineD{{1, 1}, {0, 1}}), {1, 0}));
    assert(same_point(line_intersection(LineI{{0, 0}, {2, 1}},
                                        LineI{{0, 1}, {2, -1}}), {1, 0.5}));
    cout << "PASS line_segment\n";
}
