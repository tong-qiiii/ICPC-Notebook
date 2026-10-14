# H1：二维计算几何底层

> **整数点**：`PointI = Point<long long>`，方向 / 点积 / 距离平方 / 面积优先使用 `__int128`。
> **浮点点**：`PointD = Point<double>`，`EPS = 1e-9`。
> **方向约定**：`cross(a, b, c) > 0` 表示 `a -> b -> c` 为 CCW。
> **直线约定**：`Line = p + t * v`，左半平面满足 `cross(v, q - p) >= 0`。
> **凸包约定**：统一 CCW，不重复首点。
> **浮点相等**：`PointD::operator==` / `operator<` 使用机器值；几何相等使用 `same_point()`。

整数 `PointI` 的普通 `+ - *`、方向向量等仍存储为 `long long`，要求对应中间值不溢出。整数 predicate 以及面积累加同样要求最终结果可表示于 `__int128`。

```text
SegmentRelation:
DISJOINT  无交
TOUCH     唯一交点，且至少一条线段在端点处相交
PROPER    两线段内部严格相交
OVERLAP   共线且交集有正长度

PointInPolygon:
OUT / ON / IN
```
## 7.1 二维几何基础

> **用途**：二维点 / 向量基础运算，以及后续所有计算几何模板共享的方向、距离与旋转工具。
> **复杂度**：时间 `O(1)`，空间 `O(1)`。
> **注意**：`PointI` 普通坐标运算必须保证 `long long` 不溢出；整数 predicate 的结果必须可表示于 `__int128`。
> **注意**：`PointD::operator==` / `operator<` 不使用 EPS；几何意义下的相等使用 `same_point()`。
> **注意**：浮点判定统一使用绝对误差 `EPS = 1e-9`，极端坐标尺度时需按题目调整。

### 接口速查

| 接口 | 含义 |
|---|---|
| `dot(a, b)` | 点积 |
| `cross(a, b)` | 二维叉积 |
| `cross(a, b, c)` | `(b-a) × (c-a)` |
| `len2(a)` / `dist2(a,b)` | 长度平方 / 距离平方 |
| `len(a)` / `dist(a,b)` | 长度 / 距离 |
| `perp(a)` | 逆时针旋转 `90°` |
| `unit(a)` | 单位向量，要求 `a != 0` |
| `rotate(a, rad)` | 逆时针旋转 |
| `angle(a, b)` | 两非零向量夹角 `[0, π]` |
| `orient(a,b,c)` | `+1 / 0 / -1 = CCW / 共线 / CW` |
| `same_point(a,b)` | `PointD` 几何相等 |

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using i128 = __int128_t;
using db = double;

const db EPS = 1e-9;
const db PI = acos(-1.0);

int sgn(db x) {
    return (x > EPS) - (x < -EPS);
}

int sgn(i128 x) {
    return (x > 0) - (x < 0);
}

i128 abs128(i128 x) {
    return x >= 0 ? x : -x;
}

template<class T>
struct Point {
    T x{}, y{};

    Point() = default;
    Point(T x_, T y_) : x(x_), y(y_) {}

    Point operator + (const Point& p) const {
        return {x + p.x, y + p.y};
    }

    Point operator - (const Point& p) const {
        return {x - p.x, y - p.y};
    }

    Point operator * (T k) const {
        return {x * k, y * k};
    }

    Point& operator += (const Point& p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point& operator -= (const Point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    bool operator == (const Point& p) const {
        return x == p.x && y == p.y;
    }

    bool operator != (const Point& p) const {
        return !(*this == p);
    }

    bool operator < (const Point& p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
};

template<class T>
Point<T> operator * (T k, const Point<T>& p) {
    return p * k;
}

using PointI = Point<i64>;
using PointD = Point<db>;

PointD operator / (PointD p, db k) {
    return {p.x / k, p.y / k};
}

PointD toD(PointI p) {
    return {(db)p.x, (db)p.y};
}

i128 dot(PointI a, PointI b) {
    return (i128)a.x * b.x + (i128)a.y * b.y;
}

db dot(PointD a, PointD b) {
    return a.x * b.x + a.y * b.y;
}

i128 cross(PointI a, PointI b) {
    return (i128)a.x * b.y - (i128)a.y * b.x;
}

db cross(PointD a, PointD b) {
    return a.x * b.y - a.y * b.x;
}

// Avoid long long subtraction before promotion.
i128 cross(PointI a, PointI b, PointI c) {
    return ((i128)b.x - a.x) * ((i128)c.y - a.y)
         - ((i128)b.y - a.y) * ((i128)c.x - a.x);
}

db cross(PointD a, PointD b, PointD c) {
    return cross(b - a, c - a);
}

i128 len2(PointI a) {
    return dot(a, a);
}

db len2(PointD a) {
    return dot(a, a);
}

i128 norm(PointI a) {
    return len2(a);
}

db norm(PointD a) {
    return len2(a);
}

i128 dist2(PointI a, PointI b) {
    i128 dx = (i128)a.x - b.x;
    i128 dy = (i128)a.y - b.y;
    return dx * dx + dy * dy;
}

db dist2(PointD a, PointD b) {
    return len2(a - b);
}

long double len(PointI a) {
    return sqrt((long double)len2(a));
}

db len(PointD a) {
    return sqrt(len2(a));
}

long double dist(PointI a, PointI b) {
    return sqrt((long double)dist2(a, b));
}

db dist(PointD a, PointD b) {
    return sqrt(dist2(a, b));
}

template<class T>
Point<T> perp(Point<T> a) {
    return {-a.y, a.x};
}

bool same_point(PointD a, PointD b) {
    return sgn(a.x - b.x) == 0 &&
           sgn(a.y - b.y) == 0;
}

// pre: |a| > 0
PointD unit(PointD a) {
    return a / len(a);
}

PointD rotate(PointD a, db rad) {
    db c = cos(rad), s = sin(rad);
    return {
        a.x * c - a.y * s,
        a.x * s + a.y * c
    };
}

// pre: a,b != 0
db angle(PointD a, PointD b) {
    return atan2(fabs(cross(a, b)), dot(a, b));
}

int orient(PointI a, PointI b, PointI c) {
    return sgn(cross(a, b, c));
}

int orient(PointD a, PointD b, PointD c) {
    return sgn(cross(a, b, c));
}
```

### 现场记忆

```text
cross(a,b,c) = cross(b-a,c-a)
cross > 0：CCW / left；cross < 0：CW / right

dot(a,b)   = |a||b| cos(theta)
cross(a,b) = |a||b| sin(theta)

S_triangle = |cross(A,B,C)| / 2
rad = degree * PI / 180
degree = rad * 180 / PI

PointD 几何相等用 same_point()，排序不要掺 EPS
```

## 7.2 直线与线段

> **用途**：直线 / 线段的位置关系、相交分类、交点、投影、对称与最近点。
> **复杂度**：所有接口时间 `O(1)`，空间 `O(1)`。
> **依赖**：7.1 二维几何基础。
> **限制**：`Line` 使用 `p + t * v`，所有直线运算要求 `v != 0`；退化线段 `a == b` 正常支持。
> **注意**：整数关系判断保持精确；最终构造非整数交点仍是浮点近似。

### 调用

```cpp
SegmentI s{{x1, y1}, {x2, y2}};
SegmentI t{{x3, y3}, {x4, y4}};

auto rel = segment_relation(s, t);

LineD a = LineD::through(p1, p2);
LineD b = LineD::through(p3, p4);

if (line_relation(a, b) == LineRelation::INTERSECT) {
    PointD p = line_intersection(a, b);
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `Line::through(a,b)` | 过两点构造直线 |
| `on_line(p,l)` / `side(l,p)` | 点在线上 / 点在线的哪侧 |
| `on_segment(p,s)` | 在线段上，含端点 |
| `on_segment_strict(p,s)` | 在线段内部 |
| `line_relation(a,b)` | 平行 / 重合 / 相交 |
| `line_intersection(a,b)` | 唯一交点 |
| `segment_relation(a,b)` | 四态线段相交分类 |
| `segment_intersect(a,b)` | 非严格相交 |
| `proper_segment_intersect(a,b)` | 两线段内部严格交叉 |
| `projection(q,l)` / `reflection(q,l)` | 投影 / 对称点 |
| `closest_point_on_segment(q,s)` | 线段上最近点 |
| `distance_to_line/segment()` | 点到直线 / 线段距离 |

### 模板

```cpp
template<class T>
struct Line {
    Point<T> p, v;  // p + t * v

    Line() = default;
    Line(Point<T> p_, Point<T> v_) : p(p_), v(v_) {}

    static Line through(Point<T> a, Point<T> b) {
        return {a, b - a};
    }
};

template<class T>
struct Segment {
    Point<T> a, b;
};

using LineI = Line<i64>;
using LineD = Line<db>;
using SegmentI = Segment<i64>;
using SegmentD = Segment<db>;

LineD toD(const LineI& l) {
    return {toD(l.p), toD(l.v)};
}

SegmentD toD(const SegmentI& s) {
    return {toD(s.a), toD(s.b)};
}

bool line_degenerate(const LineI& l) {
    return l.v == PointI{0, 0};
}

bool line_degenerate(const LineD& l) {
    return len2(l.v) <= EPS * EPS;
}

bool segment_degenerate(const SegmentI& s) {
    return s.a == s.b;
}

bool segment_degenerate(const SegmentD& s) {
    return same_point(s.a, s.b);
}

// cross(v, q-p)
i128 line_cross(const LineI& l, PointI q) {
    return (i128)l.v.x * ((i128)q.y - l.p.y)
         - (i128)l.v.y * ((i128)q.x - l.p.x);
}

db line_cross(const LineD& l, PointD q) {
    return cross(l.v, q - l.p);
}

bool collinear(PointI a, PointI b, PointI c) {
    return orient(a, b, c) == 0;
}

bool collinear(PointD a, PointD b, PointD c) {
    return orient(a, b, c) == 0;
}

bool on_line(PointI q, const LineI& l) {
    return !line_degenerate(l) &&
           sgn(line_cross(l, q)) == 0;
}

bool on_line(PointD q, const LineD& l) {
    return !line_degenerate(l) &&
           sgn(line_cross(l, q)) == 0;
}

// pre: l 非退化
int side(const LineI& l, PointI q) {
    return sgn(line_cross(l, q));
}

int side(const LineD& l, PointD q) {
    return sgn(line_cross(l, q));
}

bool same_side_strict(const LineI& l, PointI a, PointI b) {
    int x = side(l, a), y = side(l, b);
    return x != 0 && x == y;
}

bool same_side_strict(const LineD& l, PointD a, PointD b) {
    int x = side(l, a), y = side(l, b);
    return x != 0 && x == y;
}

bool opposite_side_strict(const LineI& l, PointI a, PointI b) {
    return side(l, a) * side(l, b) < 0;
}

bool opposite_side_strict(const LineD& l, PointD a, PointD b) {
    return side(l, a) * side(l, b) < 0;
}

// 含端点；退化线段也支持
bool on_segment(PointI p, const SegmentI& s) {
    if (orient(s.a, s.b, p) != 0) return false;

    return min(s.a.x, s.b.x) <= p.x &&
           p.x <= max(s.a.x, s.b.x) &&
           min(s.a.y, s.b.y) <= p.y &&
           p.y <= max(s.a.y, s.b.y);
}

bool on_segment(PointD p, const SegmentD& s) {
    if (orient(s.a, s.b, p) != 0) return false;

    return p.x >= min(s.a.x, s.b.x) - EPS &&
           p.x <= max(s.a.x, s.b.x) + EPS &&
           p.y >= min(s.a.y, s.b.y) - EPS &&
           p.y <= max(s.a.y, s.b.y) + EPS;
}

bool on_segment_strict(PointI p, const SegmentI& s) {
    return on_segment(p, s) &&
           p != s.a && p != s.b;
}

bool on_segment_strict(PointD p, const SegmentD& s) {
    return on_segment(p, s) &&
           !same_point(p, s.a) &&
           !same_point(p, s.b);
}

enum class LineRelation {
    PARALLEL,
    COINCIDENT,
    INTERSECT
};

// pre: 两条直线均非退化
LineRelation line_relation(const LineI& a, const LineI& b) {
    if (sgn(cross(a.v, b.v)) != 0)
        return LineRelation::INTERSECT;

    return sgn(line_cross(a, b.p)) == 0
         ? LineRelation::COINCIDENT
         : LineRelation::PARALLEL;
}

LineRelation line_relation(const LineD& a, const LineD& b) {
    if (sgn(cross(a.v, b.v)) != 0)
        return LineRelation::INTERSECT;

    return sgn(line_cross(a, b.p)) == 0
         ? LineRelation::COINCIDENT
         : LineRelation::PARALLEL;
}

// pre: line_relation(a,b) == INTERSECT
PointD line_intersection(const LineD& a, const LineD& b) {
    db t = cross(b.p - a.p, b.v) / cross(a.v, b.v);
    return a.p + a.v * t;
}

// 整数关系先精确判断，交点最终转浮点
// pre: line_relation(a,b) == INTERSECT
PointD line_intersection(const LineI& a, const LineI& b) {
    long double den =
        (long double)a.v.x * b.v.y -
        (long double)a.v.y * b.v.x;

    long double num =
        ((long double)b.p.x - a.p.x) * b.v.y -
        ((long double)b.p.y - a.p.y) * b.v.x;

    long double t = num / den;

    return {
        (db)((long double)a.p.x + (long double)a.v.x * t),
        (db)((long double)a.p.y + (long double)a.v.y * t)
    };
}

enum class SegmentRelation {
    DISJOINT,
    TOUCH,
    PROPER,
    OVERLAP
};

SegmentRelation segment_relation(
    const SegmentI& s,
    const SegmentI& t
) {
    if (segment_degenerate(s) && segment_degenerate(t))
        return s.a == t.a
             ? SegmentRelation::TOUCH
             : SegmentRelation::DISJOINT;

    if (segment_degenerate(s))
        return on_segment(s.a, t)
             ? SegmentRelation::TOUCH
             : SegmentRelation::DISJOINT;

    if (segment_degenerate(t))
        return on_segment(t.a, s)
             ? SegmentRelation::TOUCH
             : SegmentRelation::DISJOINT;

    int a = orient(s.a, s.b, t.a);
    int b = orient(s.a, s.b, t.b);
    int c = orient(t.a, t.b, s.a);
    int d = orient(t.a, t.b, s.b);

    if (a * b < 0 && c * d < 0)
        return SegmentRelation::PROPER;

    if (a == 0 && b == 0 && c == 0 && d == 0) {
        bool use_x =
            abs128((i128)s.a.x - s.b.x) >=
            abs128((i128)s.a.y - s.b.y);

        i64 s1 = use_x ? s.a.x : s.a.y;
        i64 s2 = use_x ? s.b.x : s.b.y;
        i64 t1 = use_x ? t.a.x : t.a.y;
        i64 t2 = use_x ? t.b.x : t.b.y;

        i64 L = max(min(s1, s2), min(t1, t2));
        i64 R = min(max(s1, s2), max(t1, t2));

        if (L < R) return SegmentRelation::OVERLAP;
        if (L == R) return SegmentRelation::TOUCH;
        return SegmentRelation::DISJOINT;
    }

    if ((a == 0 && on_segment(t.a, s)) ||
        (b == 0 && on_segment(t.b, s)) ||
        (c == 0 && on_segment(s.a, t)) ||
        (d == 0 && on_segment(s.b, t)))
        return SegmentRelation::TOUCH;

    return SegmentRelation::DISJOINT;
}

SegmentRelation segment_relation(
    const SegmentD& s,
    const SegmentD& t
) {
    if (segment_degenerate(s) && segment_degenerate(t))
        return same_point(s.a, t.a)
             ? SegmentRelation::TOUCH
             : SegmentRelation::DISJOINT;

    if (segment_degenerate(s))
        return on_segment(s.a, t)
             ? SegmentRelation::TOUCH
             : SegmentRelation::DISJOINT;

    if (segment_degenerate(t))
        return on_segment(t.a, s)
             ? SegmentRelation::TOUCH
             : SegmentRelation::DISJOINT;

    int a = orient(s.a, s.b, t.a);
    int b = orient(s.a, s.b, t.b);
    int c = orient(t.a, t.b, s.a);
    int d = orient(t.a, t.b, s.b);

    if (a * b < 0 && c * d < 0)
        return SegmentRelation::PROPER;

    if (a == 0 && b == 0 && c == 0 && d == 0) {
        bool use_x =
            fabs(s.a.x - s.b.x) >=
            fabs(s.a.y - s.b.y);

        db s1 = use_x ? s.a.x : s.a.y;
        db s2 = use_x ? s.b.x : s.b.y;
        db t1 = use_x ? t.a.x : t.a.y;
        db t2 = use_x ? t.b.x : t.b.y;

        db L = max(min(s1, s2), min(t1, t2));
        db R = min(max(s1, s2), max(t1, t2));

        int z = sgn(R - L);

        if (z > 0) return SegmentRelation::OVERLAP;
        if (z == 0) return SegmentRelation::TOUCH;
        return SegmentRelation::DISJOINT;
    }

    if ((a == 0 && on_segment(t.a, s)) ||
        (b == 0 && on_segment(t.b, s)) ||
        (c == 0 && on_segment(s.a, t)) ||
        (d == 0 && on_segment(s.b, t)))
        return SegmentRelation::TOUCH;

    return SegmentRelation::DISJOINT;
}

bool segment_intersect(const SegmentI& a, const SegmentI& b) {
    return segment_relation(a, b) != SegmentRelation::DISJOINT;
}

bool segment_intersect(const SegmentD& a, const SegmentD& b) {
    return segment_relation(a, b) != SegmentRelation::DISJOINT;
}

bool proper_segment_intersect(
    const SegmentI& a,
    const SegmentI& b
) {
    return segment_relation(a, b) == SegmentRelation::PROPER;
}

bool proper_segment_intersect(
    const SegmentD& a,
    const SegmentD& b
) {
    return segment_relation(a, b) == SegmentRelation::PROPER;
}

// 以下均为浮点构造；直线要求非退化

PointD projection(PointD q, const LineD& l) {
    return l.p + l.v *
        (dot(q - l.p, l.v) / len2(l.v));
}

PointD reflection(PointD q, const LineD& l) {
    return projection(q, l) * 2.0 - q;
}

db distance_to_line(PointD q, const LineD& l) {
    return fabs(cross(l.v, q - l.p)) / len(l.v);
}

PointD closest_point_on_line(PointD q, const LineD& l) {
    return projection(q, l);
}

PointD closest_point_on_segment(
    PointD q,
    const SegmentD& s
) {
    PointD v = s.b - s.a;

    if (len2(v) <= EPS * EPS)
        return s.a;

    db t = dot(q - s.a, v) / len2(v);
    t = clamp(t, 0.0, 1.0);

    return s.a + v * t;
}

db distance_to_segment(
    PointD q,
    const SegmentD& s
) {
    return dist(q, closest_point_on_segment(q, s));
}
```

### 现场记忆

```text
Line = p + t*v
side(l,q) = sgn(cross(v, q-p))
左侧 > 0，右侧 < 0

proper intersection：
orient(A,B,C) * orient(A,B,D) < 0
且反向也严格异侧

共线线段：投影到变化更大的坐标轴，区间交为
L < R -> OVERLAP；L == R -> TOUCH

projection 参数 t = dot(q-p,v) / |v|^2
线段最近点：把 t clamp 到 [0,1]
```

## 7.3 三角形常用工具

> **用途**：三角形面积、外心、内心、垂心、外接圆半径与内切圆半径。
> **复杂度**：时间 `O(1)`，空间 `O(1)`。
> **依赖**：7.1 二维几何基础。
> **注意**：圆心 / 半径接口遇到退化三角形返回 `nullopt`。

### 接口速查

| 接口 | 含义 |
|---|---|
| `triangle_area2(a,b,c)` | 有向两倍面积 |
| `triangle_area(a,b,c)` | 非负面积 |
| `circumcenter(a,b,c)` | 外心 |
| `incenter(a,b,c)` | 内心 |
| `orthocenter(a,b,c)` | 垂心 |
| `circumradius(a,b,c)` | 外接圆半径 |
| `inradius(a,b,c)` | 内切圆半径 |

### 模板

```cpp
i128 triangle_area2(PointI a, PointI b, PointI c) {
    return cross(a, b, c);
}

db triangle_area2(PointD a, PointD b, PointD c) {
    return cross(a, b, c);
}

long double triangle_area(PointI a, PointI b, PointI c) {
    return (long double)abs128(cross(a, b, c)) / 2;
}

db triangle_area(PointD a, PointD b, PointD c) {
    return fabs(cross(a, b, c)) / 2;
}

optional<PointD> circumcenter(
    PointD a,
    PointD b,
    PointD c
) {
    PointD u = b - a;
    PointD v = c - a;

    db d = 2 * cross(u, v);
    if (sgn(d) == 0)
        return nullopt;

    return a +
        (perp(u) * len2(v) -
         perp(v) * len2(u)) / d;
}

optional<PointD> incenter(
    PointD a,
    PointD b,
    PointD c
) {
    if (orient(a, b, c) == 0)
        return nullopt;

    db A = dist(b, c);
    db B = dist(c, a);
    db C = dist(a, b);

    return (a * A + b * B + c * C) / (A + B + C);
}

optional<PointD> orthocenter(
    PointD a,
    PointD b,
    PointD c
) {
    auto o = circumcenter(a, b, c);

    if (!o)
        return nullopt;

    return a + b + c - (*o) * 2.0;
}

optional<db> circumradius(
    PointD a,
    PointD b,
    PointD c
) {
    auto o = circumcenter(a, b, c);

    if (!o)
        return nullopt;

    return dist(*o, a);
}

optional<db> inradius(
    PointD a,
    PointD b,
    PointD c
) {
    db S = triangle_area(a, b, c);

    if (sgn(S) == 0)
        return nullopt;

    db s = (
        dist(a, b) +
        dist(b, c) +
        dist(c, a)
    ) / 2;

    return S / s;
}
```

### 现场记忆

```text
S = |cross(A,B,C)| / 2
s = (a+b+c)/2

R = abc / (4S)
r = S / s

内心 = (a*A + b*B + c*C) / (A+B+C)
其中 A,B,C 分别是对应顶点的对边长度

垂心 H = A + B + C - 2O
```

## 7.4 多边形基础

> **用途**：多边形面积、方向、点在简单多边形内判定以及多边形重心。
> **复杂度**：所有主要接口时间 `O(n)`，额外空间 `O(1)`。
> **依赖**：7.1 二维几何基础；`point_in_polygon()` 依赖 7.2 的 `on_segment()`。
> **限制**：`point_in_polygon()` 针对简单多边形；`polygon_centroid()` 仅对非退化简单多边形有意义。
> **注意**：`polygon_area2() > 0` 表示 CCW，后续凸包统一保持 CCW 且不重复首点。

### 调用

```cpp
vector<PointI> p(n);
// read p...

if (polygon_direction(p) == PolygonDir::CW)
    reverse(p.begin(), p.end());

PointInPolygon pos = point_in_polygon(p, q);

auto c = polygon_centroid(p);
if (c) {
    cout << c->x << ' ' << c->y << '\n';
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `polygon_area2(p)` | 有向两倍面积 |
| `polygon_area(p)` | 非负面积 |
| `polygon_direction(p)` | `CW / CCW / DEGENERATE` |
| `point_in_polygon(p,q)` | `OUT / ON / IN` |
| `polygon_centroid(p)` | 重心，退化返回 `nullopt` |

### 模板

```cpp
i128 polygon_area2(const vector<PointI>& p) {
    i128 s = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; ++i)
        s += cross(p[i], p[(i + 1) % n]);

    return s;
}

db polygon_area2(const vector<PointD>& p) {
    db s = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; ++i)
        s += cross(p[i], p[(i + 1) % n]);

    return s;
}

long double polygon_area(const vector<PointI>& p) {
    return (long double)abs128(polygon_area2(p)) / 2;
}

db polygon_area(const vector<PointD>& p) {
    return fabs(polygon_area2(p)) / 2;
}

enum class PolygonDir {
    CW = -1,
    DEGENERATE = 0,
    CCW = 1
};

PolygonDir polygon_direction(const vector<PointI>& p) {
    return static_cast<PolygonDir>(sgn(polygon_area2(p)));
}

PolygonDir polygon_direction(const vector<PointD>& p) {
    return static_cast<PolygonDir>(sgn(polygon_area2(p)));
}

enum class PointInPolygon {
    OUT,
    ON,
    IN
};

PointInPolygon point_in_polygon(
    const vector<PointI>& p,
    PointI q
) {
    int n = (int)p.size();

    if (n == 0)
        return PointInPolygon::OUT;

    if (n == 1)
        return q == p[0]
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    if (n == 2)
        return on_segment(q, SegmentI{p[0], p[1]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    int winding = 0;

    for (int i = 0; i < n; ++i) {
        PointI a = p[i];
        PointI b = p[(i + 1) % n];

        if (on_segment(q, SegmentI{a, b}))
            return PointInPolygon::ON;

        // Half-open y interval avoids counting vertices twice.
        if (a.y <= q.y && q.y < b.y &&
            orient(a, b, q) > 0)
            ++winding;

        if (b.y <= q.y && q.y < a.y &&
            orient(a, b, q) < 0)
            --winding;
    }

    return winding
         ? PointInPolygon::IN
         : PointInPolygon::OUT;
}

PointInPolygon point_in_polygon(
    const vector<PointD>& p,
    PointD q
) {
    int n = (int)p.size();

    if (n == 0)
        return PointInPolygon::OUT;

    if (n == 1)
        return same_point(q, p[0])
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    if (n == 2)
        return on_segment(q, SegmentD{p[0], p[1]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    int winding = 0;

    for (int i = 0; i < n; ++i) {
        PointD a = p[i];
        PointD b = p[(i + 1) % n];

        if (on_segment(q, SegmentD{a, b}))
            return PointInPolygon::ON;

        int ya = sgn(a.y - q.y);
        int yb = sgn(b.y - q.y);

        if (ya <= 0 && yb > 0 &&
            orient(a, b, q) > 0)
            ++winding;

        if (yb <= 0 && ya > 0 &&
            orient(a, b, q) < 0)
            --winding;
    }

    return winding
         ? PointInPolygon::IN
         : PointInPolygon::OUT;
}

optional<PointD> polygon_centroid(
    const vector<PointD>& p
) {
    int n = (int)p.size();

    db s = 0;
    db sx = 0, sy = 0;

    for (int i = 0; i < n; ++i) {
        PointD a = p[i];
        PointD b = p[(i + 1) % n];

        db c = cross(a, b);

        s += c;
        sx += (a.x + b.x) * c;
        sy += (a.y + b.y) * c;
    }

    if (sgn(s) == 0)
        return nullopt;

    return PointD{
        sx / (3 * s),
        sy / (3 * s)
    };
}

optional<PointD> polygon_centroid(
    const vector<PointI>& p
) {
    int n = (int)p.size();

    i128 s = 0;
    long double sx = 0, sy = 0;

    for (int i = 0; i < n; ++i) {
        PointI a = p[i];
        PointI b = p[(i + 1) % n];

        i128 c = cross(a, b);

        s += c;
        sx += ((long double)a.x + b.x) * (long double)c;
        sy += ((long double)a.y + b.y) * (long double)c;
    }

    if (s == 0)
        return nullopt;

    return PointD{
        (db)(sx / (3 * (long double)s)),
        (db)(sy / (3 * (long double)s))
    };
}
```

### 现场记忆

```text
polygon_area2 = sum cross(p[i], p[i+1])
area2 > 0 -> CCW；area2 < 0 -> CW

点在边上必须先判 ON

winding：
向上穿过 q.y 且 q 在边左侧 -> +1
向下穿过 q.y 且 q 在边右侧 -> -1
非零 -> IN

重心分母 = 3 * sum cross(p[i],p[i+1])
```

## 7.5 格点多边形：Pick 定理

> **用途**：计算整点线段格点数、简单格点多边形边界格点数与内部格点数。
> **复杂度**：单条线段 `O(log C)`；`n` 边多边形 `O(n log C)`，其中 `C` 为坐标差范围。
> **依赖**：7.4 的 `polygon_area2()`。
> **限制**：Pick 定理仅适用于非退化简单格点多边形。

```cpp
i128 gcd128(i128 a, i128 b) {
    a = abs128(a);
    b = abs128(b);

    while (b) {
        i128 t = a % b;
        a = b;
        b = t;
    }

    return a;
}

// 闭线段 [a,b]，包含两个端点
i128 lattice_points_on_segment(PointI a, PointI b) {
    i128 dx = (i128)b.x - a.x;
    i128 dy = (i128)b.y - a.y;

    return gcd128(dx, dy) + 1;
}

i128 boundary_lattice_points(
    const vector<PointI>& p
) {
    i128 B = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; ++i) {
        PointI a = p[i];
        PointI b = p[(i + 1) % n];

        B += gcd128(
            (i128)b.x - a.x,
            (i128)b.y - a.y
        );
    }

    return B;
}

i128 interior_lattice_points(
    const vector<PointI>& p
) {
    i128 A2 = abs128(polygon_area2(p));
    i128 B = boundary_lattice_points(p);

    return (A2 - B + 2) / 2;
}
```

### 现场记忆

```text
整点线段：
g = gcd(|dx|, |dy|)
闭线段格点数 = g + 1

多边形边界：
B = sum gcd(|dx|, |dy|)
不能对每条边累加 g+1，否则顶点重复

Pick：
A = I + B/2 - 1
A2 = 2I + B - 2
I = (A2 - B + 2) / 2
```


# H2：凸几何

> **默认整数几何**：`PointI / i128 / cross / orient / dist2`。
> **凸包约定**：`convex_hull(raw)` 默认去重、删除边界中间共线点、CCW、不重复首点。
> **卡壳约定**：7.9 / 7.10 默认直接使用 `convex_hull(raw)`。
> **距离比较**：优先比较 `dist2`，不做无意义的 `sqrt()`。
> **点包含**：继续使用 H1 的 `PointInPolygon::OUT / ON / IN`。

## 7.6 二维凸包 Andrew

> **用途**：求二维点集凸包。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **下标**：返回凸包顶点 `0-indexed`。
> **注意**：默认删除边界中间共线点；7.9 / 7.10 推荐直接使用默认输出。

### 调用

```cpp
auto hull = convex_hull(p);        // 推荐：无冗余共线点
auto hull2 = convex_hull(p, true); // 保留所有边界点
```

### 模板

```cpp
vector<PointI> convex_hull(
    vector<PointI> p,
    bool keep_collinear = false
) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    int n = (int)p.size();
    if (n <= 2)
        return p;

    bool all_collinear = true;

    for (int i = 2; i < n; ++i) {
        if (orient(p[0], p[1], p[i]) != 0) {
            all_collinear = false;
            break;
        }
    }

    if (all_collinear) {
        if (keep_collinear)
            return p;

        return {p.front(), p.back()};
    }

    vector<PointI> lo, up;

    for (PointI x : p) {
        while ((int)lo.size() >= 2) {
            int o = orient(
                lo[(int)lo.size() - 2],
                lo.back(),
                x
            );

            if (keep_collinear ? o < 0 : o <= 0)
                lo.pop_back();
            else
                break;
        }

        lo.push_back(x);
    }

    for (int i = n - 1; i >= 0; --i) {
        PointI x = p[i];

        while ((int)up.size() >= 2) {
            int o = orient(
                up[(int)up.size() - 2],
                up.back(),
                x
            );

            if (keep_collinear ? o < 0 : o <= 0)
                up.pop_back();
            else
                break;
        }

        up.push_back(x);
    }

    lo.pop_back();
    up.pop_back();

    lo.insert(lo.end(), up.begin(), up.end());

    return lo;
}
```

### 现场记忆

```text
Andrew：
先按 (x,y) 排序 + 去重

默认无共线：
    while cross <= 0 -> pop

保留边界共线：
    while cross < 0 -> pop

非退化结果：
    CCW
    不重复首点

全共线：
    false -> 两端点
    true  -> 所有不同点
```


## 7.7 凸多边形点包含 O(log n)

> **用途**：判断点位于凸多边形外部、边界或内部。
> **复杂度**：时间 `O(log n)`，空间 `O(1)`。
> **下标**：凸多边形顶点 `0-indexed`。
> **限制**：输入按 CCW 排列，不重复首点。
> **注意**：兼容 `convex_hull(raw)`，也兼容 `convex_hull(raw, true)` 保留边界共线点的输出。

### 调用

```cpp
auto hull = convex_hull(raw);

PointInPolygon pos =
    point_in_convex_polygon(hull, q);
```

### 模板

```cpp
PointInPolygon point_in_convex_polygon(
    const vector<PointI>& p,
    PointI q
) {
    int n = (int)p.size();

    if (n == 0)
        return PointInPolygon::OUT;

    if (n == 1)
        return q == p[0]
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    if (n == 2)
        return on_segment(q, SegmentI{p[0], p[1]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    // convex_hull(..., true) 的全共线输出
    if (sgn(polygon_area2(p)) == 0) {
        return on_segment(
            q,
            SegmentI{p.front(), p.back()}
        )
        ? PointInPolygon::ON
        : PointInPolygon::OUT;
    }

    // 找首射线上离 p[0] 最远的边界点
    int lo = 1, hi = n - 1;

    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;

        if (orient(p[0], p[1], p[mid]) == 0)
            lo = mid;
        else
            hi = mid - 1;
    }

    int L = lo;

    // 找尾射线上离 p[0] 最远的边界点
    lo = 1;
    hi = n - 1;

    while (lo < hi) {
        int mid = (lo + hi) / 2;

        if (orient(p[0], p[n - 1], p[mid]) == 0)
            hi = mid;
        else
            lo = mid + 1;
    }

    int R = lo;

    int a = orient(p[0], p[L], q);
    int b = orient(p[0], p[R], q);

    if (a < 0 || b > 0)
        return PointInPolygon::OUT;

    if (a == 0) {
        return on_segment(q, SegmentI{p[0], p[L]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;
    }

    if (b == 0) {
        return on_segment(q, SegmentI{p[0], p[R]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;
    }

    int l = L, r = R;

    while (r - l > 1) {
        int mid = (l + r) / 2;

        if (orient(p[0], p[mid], q) >= 0)
            l = mid;
        else
            r = mid;
    }

    int o = orient(p[l], p[l + 1], q);

    if (o < 0)
        return PointInPolygon::OUT;

    if (o == 0) {
        return on_segment(
            q,
            SegmentI{p[l], p[l + 1]}
        )
        ? PointInPolygon::ON
        : PointInPolygon::OUT;
    }

    return PointInPolygon::IN;
}
```

### 现场记忆

```text
以 p[0] 为扇形原点。

先判：
    q 是否位于首射线与尾射线之间

落在首 / 尾射线：
    直接 on_segment 判 ON

否则二分找到：
    angle(p[l]) <= angle(q) < angle(p[l+1])

最后看：
    orient(p[l], p[l+1], q)

> 0 -> IN
= 0 -> ON
< 0 -> OUT
```


## 7.8 凸多边形点包含 O(log n)

> **用途**：判断点位于凸多边形外部、边界或内部。
> **复杂度**：时间 `O(log n)`，空间 `O(1)`。
> **下标**：凸多边形顶点 `0-indexed`。
> **限制**：输入按 CCW 排列，不重复首点。
> **注意**：兼容 `convex_hull(raw)`，也兼容 `convex_hull(raw, true)` 保留边界共线点的输出。

### 调用

```cpp
auto hull = convex_hull(raw);

PointInPolygon pos =
    point_in_convex_polygon(hull, q);
```

### 模板

```cpp
PointInPolygon point_in_convex_polygon(
    const vector<PointI>& p,
    PointI q
) {
    int n = (int)p.size();

    if (n == 0)
        return PointInPolygon::OUT;

    if (n == 1)
        return q == p[0]
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    if (n == 2)
        return on_segment(q, SegmentI{p[0], p[1]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;

    // convex_hull(..., true) 的全共线输出
    if (sgn(polygon_area2(p)) == 0) {
        return on_segment(
            q,
            SegmentI{p.front(), p.back()}
        )
        ? PointInPolygon::ON
        : PointInPolygon::OUT;
    }

    // 找首射线上离 p[0] 最远的边界点
    int lo = 1, hi = n - 1;

    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;

        if (orient(p[0], p[1], p[mid]) == 0)
            lo = mid;
        else
            hi = mid - 1;
    }

    int L = lo;

    // 找尾射线上离 p[0] 最远的边界点
    lo = 1;
    hi = n - 1;

    while (lo < hi) {
        int mid = (lo + hi) / 2;

        if (orient(p[0], p[n - 1], p[mid]) == 0)
            hi = mid;
        else
            lo = mid + 1;
    }

    int R = lo;

    int a = orient(p[0], p[L], q);
    int b = orient(p[0], p[R], q);

    if (a < 0 || b > 0)
        return PointInPolygon::OUT;

    if (a == 0) {
        return on_segment(q, SegmentI{p[0], p[L]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;
    }

    if (b == 0) {
        return on_segment(q, SegmentI{p[0], p[R]})
             ? PointInPolygon::ON
             : PointInPolygon::OUT;
    }

    int l = L, r = R;

    while (r - l > 1) {
        int mid = (l + r) / 2;

        if (orient(p[0], p[mid], q) >= 0)
            l = mid;
        else
            r = mid;
    }

    int o = orient(p[l], p[l + 1], q);

    if (o < 0)
        return PointInPolygon::OUT;

    if (o == 0) {
        return on_segment(
            q,
            SegmentI{p[l], p[l + 1]}
        )
        ? PointInPolygon::ON
        : PointInPolygon::OUT;
    }

    return PointInPolygon::IN;
}
```

### 现场记忆

```text
以 p[0] 为扇形原点。

先判：
    q 是否位于首射线与尾射线之间

落在首 / 尾射线：
    直接 on_segment 判 ON

否则二分找到：
    angle(p[l]) <= angle(q) < angle(p[l+1])

最后看：
    orient(p[l], p[l+1], q)

> 0 -> IN
= 0 -> ON
< 0 -> OUT
```

## 7.9 旋转卡壳：凸包直径

> **用途**：求凸包最远点对及最大距离平方。
> **复杂度**：时间 `O(n)`，空间 `O(1)`。
> **下标**：凸包顶点 `0-indexed`。
> **限制**：推荐输入 `convex_hull(raw)` 的 CCW、无冗余共线点凸包。
> **注意**：返回距离平方 `d2`，不要为了比较距离调用 `sqrt()`。

### 调用

```cpp
auto hull = convex_hull(raw);
auto res = convex_diameter(hull);

if (res) {
    PointI a = res->a;
    PointI b = res->b;
    i128 d2 = res->d2;
}
```

### 模板

```cpp
struct DiameterResult {
    PointI a, b;
    i128 d2;
};

optional<DiameterResult> convex_diameter(
    const vector<PointI>& p
) {
    int n = (int)p.size();

    if (n == 0)
        return nullopt;

    if (n == 1)
        return DiameterResult{p[0], p[0], 0};

    if (n == 2) {
        return DiameterResult{
            p[0],
            p[1],
            dist2(p[0], p[1])
        };
    }

    // 额外容错：直接传入多个共线点
    if (sgn(polygon_area2(p)) == 0) {
        auto [mn, mx] =
            minmax_element(p.begin(), p.end());

        return DiameterResult{
            *mn,
            *mx,
            dist2(*mn, *mx)
        };
    }

    DiameterResult ans{p[0], p[0], 0};

    auto update = [&](int i, int j) {
        i128 d = dist2(p[i], p[j]);

        if (d > ans.d2)
            ans = {p[i], p[j], d};
    };

    auto area = [&](int i, int j) -> i128 {
        return abs128(
            cross(
                p[i],
                p[(i + 1) % n],
                p[j]
            )
        );
    };

    int j = 1;

    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;

        while (
            area(i, (j + 1) % n) >
            area(i, j)
        ) {
            j = (j + 1) % n;
        }

        update(i, j);
        update(ni, j);

        // 平行支撑边：检查面积平台的另一个端点
        int nj = (j + 1) % n;

        if (area(i, nj) == area(i, j)) {
            update(i, nj);
            update(ni, nj);
        }
    }

    return ans;
}
```

### 现场记忆

```text
固定凸包边：
    i -> i+1

只要：
    area(i, j+1) > area(i, j)
就继续推进 j

area =
    |cross(p[i], p[i+1], p[j])|

每条边检查：
    (i, j)
    (i+1, j)

面积出现平台时顺手检查 j+1。

j 只向前走 -> O(n)
最终答案比较 dist2，不 sqrt。
```

## 7.10 旋转卡壳：最小面积外接矩形

> **用途**：求整数凸包的最小面积外接矩形面积。
> **复杂度**：时间 `O(n)`，空间 `O(1)`。
> **下标**：凸包顶点 `0-indexed`。
> **限制**：推荐输入 `convex_hull(raw)` 的 CCW、无冗余共线点凸包。
> **注意**：三个支撑点的比较全部使用 `i128`；只有最终面积转为浮点。
> **注意**：第一条边必须先 `O(n)` 找到三个真实支撑点，不能简单把指针设为 `0` 后只向前试探。

### 调用

```cpp
auto hull = convex_hull(raw);

db ans =
    min_area_bounding_rectangle(hull);
```

### 模板

```cpp
db min_area_bounding_rectangle(
    const vector<PointI>& p
) {
    int n = (int)p.size();

    if (n <= 2)
        return 0;

    if (sgn(polygon_area2(p)) == 0)
        return 0;

    // j: cross 最大
    // k: dot 最大
    // l: dot 最小
    int j = 0;
    int k = 0;
    int l = 0;

    // 第一条边先找到三个真实极值
    {
        int i = 0;
        int ni = 1;

        i128 ex = (i128)p[ni].x - p[i].x;
        i128 ey = (i128)p[ni].y - p[i].y;

        auto cr = [&](int t) -> i128 {
            return
                ex * ((i128)p[t].y - p[i].y) -
                ey * ((i128)p[t].x - p[i].x);
        };

        auto pr = [&](int t) -> i128 {
            return
                ex * ((i128)p[t].x - p[i].x) +
                ey * ((i128)p[t].y - p[i].y);
        };

        for (int t = 1; t < n; ++t) {
            if (cr(t) > cr(j))
                j = t;

            if (pr(t) > pr(k))
                k = t;

            if (pr(t) < pr(l))
                l = t;
        }
    }

    db ans = numeric_limits<db>::infinity();

    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;

        i128 ex = (i128)p[ni].x - p[i].x;
        i128 ey = (i128)p[ni].y - p[i].y;

        auto cr = [&](int t) -> i128 {
            return
                ex * ((i128)p[t].y - p[i].y) -
                ey * ((i128)p[t].x - p[i].x);
        };

        auto pr = [&](int t) -> i128 {
            return
                ex * ((i128)p[t].x - p[i].x) +
                ey * ((i128)p[t].y - p[i].y);
        };

        while (cr((j + 1) % n) > cr(j))
            j = (j + 1) % n;

        while (pr((k + 1) % n) > pr(k))
            k = (k + 1) % n;

        while (pr((l + 1) % n) < pr(l))
            l = (l + 1) % n;

        i128 height_num = cr(j);
        i128 width_num = pr(k) - pr(l);
        i128 e2 = ex * ex + ey * ey;

        db cur = (db)(
            (long double)height_num *
            (long double)width_num /
            (long double)e2
        );

        ans = min(ans, cur);
    }

    return ans;
}
```

### 现场记忆

```text
当前边方向 e 固定：

j：cross(e, p) 最大
k：dot(e, p) 最大
l：dot(e, p) 最小

height = cross_max / |e|
width  = (dot_max - dot_min) / |e|

area =
    cross_max
    * (dot_max - dot_min)
    / |e|^2

第一条边先 O(n) 找真实 j / k / l，
之后三个指针只向前移动。

卡壳比较全程 i128，无需 EPS。
```

## 7.11 最近点对

> **用途**：求整数点集中最近的两个点及最小距离平方。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **注意**：距离判断全部使用 `i128 dist2`，不调用 `sqrt()`；重复点会得到 `d2 = 0`。

### 调用

```cpp
auto res = closest_pair(p);

if (res) {
    PointI a = res->a;
    PointI b = res->b;
    i128 d2 = res->d2;
}
```

### 模板

```cpp
struct ClosestPairResult {
    PointI a, b;
    i128 d2;
};

optional<ClosestPairResult> closest_pair(
    vector<PointI> a
) {
    int n = (int)a.size();

    if (n < 2)
        return nullopt;

    sort(a.begin(), a.end());

    ClosestPairResult best{
        a[0],
        a[1],
        dist2(a[0], a[1])
    };

    vector<PointI> tmp(n);
    vector<PointI> strip;
    strip.reserve(n);

    auto cmp_y = [](PointI a, PointI b) {
        return a.y != b.y
             ? a.y < b.y
             : a.x < b.x;
    };

    auto update = [&](PointI x, PointI y) {
        i128 d = dist2(x, y);

        if (d < best.d2)
            best = {x, y, d};
    };

    function<void(int, int)> solve =
        [&](int l, int r) {
            // [l, r)
            if (r - l <= 3) {
                for (int i = l; i < r; ++i) {
                    for (int j = i + 1; j < r; ++j)
                        update(a[i], a[j]);
                }

                sort(
                    a.begin() + l,
                    a.begin() + r,
                    cmp_y
                );

                return;
            }

            int m = (l + r) / 2;

            // 递归会把区间改成按 y 排序，先保存分割线
            i64 mid_x = a[m].x;

            solve(l, m);
            solve(m, r);

            merge(
                a.begin() + l,
                a.begin() + m,
                a.begin() + m,
                a.begin() + r,
                tmp.begin() + l,
                cmp_y
            );

            copy(
                tmp.begin() + l,
                tmp.begin() + r,
                a.begin() + l
            );

            strip.clear();

            for (int i = l; i < r; ++i) {
                i128 dx = (i128)a[i].x - mid_x;

                if (dx * dx < best.d2)
                    strip.push_back(a[i]);
            }

            // strip 已按 y 排序
            for (int i = 0; i < (int)strip.size(); ++i) {
                for (int j = i - 1; j >= 0; --j) {
                    i128 dy =
                        (i128)strip[i].y -
                        strip[j].y;

                    if (dy * dy >= best.d2)
                        break;

                    update(strip[i], strip[j]);
                }
            }
        };

    solve(0, n);

    return best;
}
```

### 现场记忆

```text
开始：
    全局按 x 排序

solve(l,r)：
    先保存 mid_x
    递归左右
    返回后把当前区间 merge 成按 y 排序

跨中线只考虑：
    (x - mid_x)^2 < best

strip 已按 y 排序，
向前检查直到：
    dy^2 >= best
即可停止。

所有比较用 dist2 / i128，不 sqrt。
```

# H3：半平面与圆

> **浮点几何**：`PointD / LineD / db`，统一使用 `EPS = 1e-9`。
> **直线**：`p + t * v`。
> **半平面**：有向直线保留左侧，即 `cross(v, q-p) >= 0`，包含边界。
> **圆**：`Circle{o, r}`，默认要求 `r >= 0`。

## 7.12 半平面交

> **用途**：求若干左半平面的有界正面积交集多边形。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **依赖**：7.1 / 7.2 / 7.4。
> **限制**：若非空交集可能无界，必须根据题意先加入合法外框；本函数不负责无界判定。
> **注意**：`HalfPlane(a,b)` 保留 `a -> b` 左侧；返回 CCW、不重复首点。
> **注意**：空返回表示在“交集应有界”的前提下，没有正面积二维区域；空集与退化成点 / 线段均不区分。

### 调用

```cpp
vector<HalfPlane> h;

h.emplace_back(a, b); // 保留 a -> b 左侧

auto poly = halfplane_intersection(h);

if (!poly.empty()) {
    db area = polygon_area(poly);
}
```

### 模板

```cpp
struct HalfPlane {
    LineD l;

    HalfPlane() = default;

    // 保留 a -> b 左侧
    HalfPlane(PointD a, PointD b)
        : l(LineD::through(a, b)) {}

    explicit HalfPlane(LineD l_)
        : l(l_) {}

    // true <=> q 严格位于被删除的一侧
    bool outside(PointD q) const {
        return sgn(cross(l.v, q - l.p)) < 0;
    }
};

bool halfplane_parallel(
    const HalfPlane& a,
    const HalfPlane& b
) {
    return sgn(cross(a.l.v, b.l.v)) == 0;
}

bool halfplane_same_direction(
    const HalfPlane& a,
    const HalfPlane& b
) {
    return halfplane_parallel(a, b) &&
           sgn(dot(a.l.v, b.l.v)) > 0;
}

// comparator 内不用 EPS，保证 strict weak ordering
int halfplane_half(PointD v) {
    return v.y < 0 ||
           (v.y == 0 && v.x < 0);
}

bool halfplane_angle_less(
    const HalfPlane& a,
    const HalfPlane& b
) {
    int A = halfplane_half(a.l.v);
    int B = halfplane_half(b.l.v);

    if (A != B)
        return A < B;

    db c = cross(a.l.v, b.l.v);

    if (c != 0)
        return c > 0;

    return false;
}

// pre:
//   所有直线非退化
//   若交集非空，则交集应有界
vector<PointD> halfplane_intersection(
    vector<HalfPlane> h
) {
    if (h.empty())
        return {};

    sort(
        h.begin(),
        h.end(),
        halfplane_angle_less
    );

    // 同方向只保留更严格的一条
    vector<HalfPlane> a;

    for (const HalfPlane& x : h) {
        if (!a.empty() &&
            halfplane_same_direction(a.back(), x)) {

            if (x.outside(a.back().l.p))
                a.back() = x;
        } else {
            a.push_back(x);
        }
    }

    deque<HalfPlane> q;

    for (const HalfPlane& x : a) {
        while (q.size() >= 2) {
            if (halfplane_parallel(
                q[q.size() - 2],
                q.back()
            ))
                return {};

            PointD p = line_intersection(
                q[q.size() - 2].l,
                q.back().l
            );

            if (x.outside(p))
                q.pop_back();
            else
                break;
        }

        while (q.size() >= 2) {
            if (halfplane_parallel(q[0], q[1]))
                return {};

            PointD p =
                line_intersection(q[0].l, q[1].l);

            if (x.outside(p))
                q.pop_front();
            else
                break;
        }

        if (!q.empty() &&
            halfplane_parallel(q.back(), x))
            return {};

        q.push_back(x);
    }

    while (q.size() >= 3) {
        PointD p = line_intersection(
            q[q.size() - 2].l,
            q.back().l
        );

        if (q.front().outside(p))
            q.pop_back();
        else
            break;
    }

    while (q.size() >= 3) {
        PointD p =
            line_intersection(q[0].l, q[1].l);

        if (q.back().outside(p))
            q.pop_front();
        else
            break;
    }

    if (q.size() < 3)
        return {};

    vector<PointD> poly;

    for (int i = 0; i < (int)q.size(); ++i) {
        const HalfPlane& A = q[i];
        const HalfPlane& B =
            q[(i + 1) % q.size()];

        if (halfplane_parallel(A, B))
            return {};

        poly.push_back(
            line_intersection(A.l, B.l)
        );
    }

    db area2 = polygon_area2(poly);

    if (sgn(area2) == 0)
        return {};

    if (sgn(area2) < 0)
        reverse(poly.begin(), poly.end());

    return poly;
}
```

### 现场记忆

```text
HalfPlane(a,b)：
    保留 a -> b 左侧
    cross(b-a, q-a) >= 0

极角排序后：
    同方向平行线只留更严格的一条

新半平面 x：
    尾部交点在 x 外 -> pop_back
    首部交点在 x 外 -> pop_front

最后首约束尾、尾约束首，再取相邻直线交点。

这版不检测无界：
    可能无界 -> 题目层面先加合法外框
```

## 7.13 圆基础

> **用途**：点 / 直线 / 两圆关系，圆交点、两圆公共面积与三点外接圆。
> **复杂度**：所有接口时间 `O(1)`，空间 `O(1)`。
> **依赖**：7.1 / 7.2 / 7.3。
> **限制**：`Circle.r >= 0`；`r = 0` 允许表示退化点圆。

### 接口速查

| 接口 | 含义 |
|---|---|
| `point_circle_relation(p,c)` | 点与圆关系 |
| `in_circle(p,c)` | 是否在圆盘内或边界 |
| `line_circle_relation(l,c)` | 直线与圆关系 |
| `line_circle_intersection(l,c)` | 直线圆交点 |
| `circle_relation(a,b)` | 两圆位置关系 |
| `circle_circle_intersection(a,b)` | 两圆交点 |
| `circle_intersection_area(a,b)` | 两圆盘公共面积 |
| `circumcircle(a,b,c)` | 三点外接圆 |

### 模板

```cpp
struct Circle {
    PointD o;
    db r;
};

enum class PointCircleRelation {
    INSIDE,
    ON,
    OUTSIDE
};

PointCircleRelation point_circle_relation(
    PointD p,
    const Circle& c
) {
    int s = sgn(dist(p, c.o) - c.r);

    if (s < 0)
        return PointCircleRelation::INSIDE;

    if (s == 0)
        return PointCircleRelation::ON;

    return PointCircleRelation::OUTSIDE;
}

bool in_circle(
    PointD p,
    const Circle& c
) {
    return point_circle_relation(p, c) !=
           PointCircleRelation::OUTSIDE;
}

enum class LineCircleRelation {
    DISJOINT,
    TANGENT,
    SECANT
};

// pre: l 非退化
LineCircleRelation line_circle_relation(
    const LineD& l,
    const Circle& c
) {
    int s = sgn(
        distance_to_line(c.o, l) - c.r
    );

    if (s > 0)
        return LineCircleRelation::DISJOINT;

    if (s == 0)
        return LineCircleRelation::TANGENT;

    return LineCircleRelation::SECANT;
}

// 0 / 1 / 2 个交点
// pre: l 非退化
vector<PointD> line_circle_intersection(
    const LineD& l,
    const Circle& c
) {
    PointD h = projection(c.o, l);

    db h2 =
        c.r * c.r -
        dist2(h, c.o);

    int s = sgn(h2);

    if (s < 0)
        return {};

    if (s == 0)
        return {h};

    db d = sqrt(max<db>(0, h2));
    PointD e = unit(l.v);

    return {
        h - e * d,
        h + e * d
    };
}

enum class CircleRelation {
    DISJOINT,
    EXTERNALLY_TANGENT,
    INTERSECT,
    INTERNALLY_TANGENT,
    CONTAINED,
    COINCIDENT
};

CircleRelation circle_relation(
    const Circle& a,
    const Circle& b
) {
    db d = dist(a.o, b.o);

    if (sgn(d) == 0 &&
        sgn(a.r - b.r) == 0)
        return CircleRelation::COINCIDENT;

    db sum = a.r + b.r;
    db dif = fabs(a.r - b.r);

    if (sgn(d - sum) > 0)
        return CircleRelation::DISJOINT;

    if (sgn(d - sum) == 0)
        return CircleRelation::EXTERNALLY_TANGENT;

    if (sgn(d - dif) < 0)
        return CircleRelation::CONTAINED;

    if (sgn(d - dif) == 0)
        return CircleRelation::INTERNALLY_TANGENT;

    return CircleRelation::INTERSECT;
}

enum class CircleIntersectionType {
    NONE,
    ONE,
    TWO,
    INFINITE
};

struct CircleIntersectionResult {
    CircleIntersectionType type;
    vector<PointD> points;
};

CircleIntersectionResult circle_circle_intersection(
    const Circle& a,
    const Circle& b
) {
    CircleRelation rel = circle_relation(a, b);

    if (rel == CircleRelation::DISJOINT ||
        rel == CircleRelation::CONTAINED) {
        return {
            CircleIntersectionType::NONE,
            {}
        };
    }

    if (rel == CircleRelation::COINCIDENT) {
        if (sgn(a.r) == 0) {
            return {
                CircleIntersectionType::ONE,
                {a.o}
            };
        }

        return {
            CircleIntersectionType::INFINITE,
            {}
        };
    }

    PointD dvec = b.o - a.o;
    db d = len(dvec);

    db x = (
        d * d +
        a.r * a.r -
        b.r * b.r
    ) / (2 * d);

    db h2 =
        a.r * a.r -
        x * x;

    PointD e = dvec / d;
    PointD base = a.o + e * x;

    if (sgn(h2) <= 0) {
        return {
            CircleIntersectionType::ONE,
            {base}
        };
    }

    PointD v =
        perp(e) * sqrt(max<db>(0, h2));

    return {
        CircleIntersectionType::TWO,
        {
            base + v,
            base - v
        }
    };
}

db circle_intersection_area(
    const Circle& a,
    const Circle& b
) {
    db r1 = a.r;
    db r2 = b.r;
    db d = dist(a.o, b.o);

    if (sgn(r1) == 0 ||
        sgn(r2) == 0)
        return 0;

    if (sgn(d) == 0) {
        db r = min(r1, r2);
        return PI * r * r;
    }

    if (sgn(d - r1 - r2) >= 0)
        return 0;

    if (sgn(d - fabs(r1 - r2)) <= 0) {
        db r = min(r1, r2);
        return PI * r * r;
    }

    db ca = (
        r1 * r1 +
        d * d -
        r2 * r2
    ) / (2 * r1 * d);

    db cb = (
        r2 * r2 +
        d * d -
        r1 * r1
    ) / (2 * r2 * d);

    ca = clamp(ca, -1.0, 1.0);
    cb = clamp(cb, -1.0, 1.0);

    db A = 2 * acos(ca);
    db B = 2 * acos(cb);

    return
        0.5 * r1 * r1 * (A - sin(A)) +
        0.5 * r2 * r2 * (B - sin(B));
}

optional<Circle> circumcircle(
    PointD a,
    PointD b,
    PointD c
) {
    auto o = circumcenter(a, b, c);

    if (!o)
        return nullopt;

    return Circle{
        *o,
        dist(*o, a)
    };
}
```

### 现场记忆

```text
直线圆：
    先投影圆心 H
    h2 = r^2 - |OH|^2
    <0 / =0 / >0 -> 0 / 1 / 2 个交点

两圆：
    d 与 r1+r2 比较 -> 外离 / 外切
    d 与 |r1-r2| 比较 -> 内含 / 内切
    中间 -> 两交点

两圆交点：
    x = (d^2+r1^2-r2^2)/(2d)
    h^2 = r1^2-x^2

两圆公共面积：
    外离 0
    内含 = 小圆面积
    其余 = 两个圆弓面积
```

## 7.14 圆的切线

> **用途**：求点到圆的切点 / 切线，以及两个圆的所有公切线。
> **复杂度**：时间 `O(1)`，空间 `O(1)`。
> **依赖**：7.13 圆基础。
> **限制**：本模板切线部分要求圆半径 `r > 0`。
> **注意**：两圆相切时切点可能重合，因此公切线直接用法向量构造，不能 `LineD::through(p,p)`。

### 接口速查

| 接口 | 含义 |
|---|---|
| `tangent_points_from_point(p,c)` | 点到圆的切点 |
| `tangents_from_point(p,c)` | 点到圆的切线 |
| `common_tangents(a,b)` | 两圆全部公切线 |

### 模板

```cpp
// pre: c.r > 0
vector<PointD> tangent_points_from_point(
    PointD p,
    const Circle& c
) {
    PointD v = p - c.o;

    db d2 = len2(v);
    db r2 = c.r * c.r;

    int s = sgn(d2 - r2);

    if (s < 0)
        return {};

    if (s == 0)
        return {p};

    PointD base =
        c.o + v * (r2 / d2);

    db h =
        c.r *
        sqrt(max<db>(0, d2 - r2)) /
        d2;

    PointD off = perp(v) * h;

    return {
        base + off,
        base - off
    };
}

// pre: c.r > 0
vector<LineD> tangents_from_point(
    PointD p,
    const Circle& c
) {
    auto tp = tangent_points_from_point(p, c);

    if (tp.empty())
        return {};

    if (tp.size() == 1) {
        return {
            LineD{
                p,
                perp(p - c.o)
            }
        };
    }

    return {
        LineD::through(p, tp[0]),
        LineD::through(p, tp[1])
    };
}

struct CommonTangent {
    LineD line;
    PointD a;
    PointD b;
};

struct CommonTangentsResult {
    bool infinite = false;
    vector<CommonTangent> tangents;
};

// pre: a.r > 0 && b.r > 0
CommonTangentsResult common_tangents(
    const Circle& a,
    const Circle& b
) {
    CommonTangentsResult res;

    PointD d = b.o - a.o;
    db z = len2(d);

    if (sgn(z) == 0) {
        if (sgn(a.r - b.r) == 0)
            res.infinite = true;

        return res;
    }

    // s = +1: 外公切线
    // s = -1: 内公切线
    for (int s : {-1, 1}) {
        db r = a.r - s * b.r;
        db h2 = z - r * r;

        if (sgn(h2) < 0)
            continue;

        db h = sqrt(max<db>(0, h2));

        for (int t : {-1, 1}) {
            // h = 0 时两种符号是同一条线
            if (t == -1 && sgn(h) == 0)
                continue;

            PointD n = (
                d * r +
                perp(d) * (h * t)
            ) / z;

            PointD p1 =
                a.o + n * a.r;

            PointD p2 =
                b.o + n * (s * b.r);

            res.tangents.push_back({
                LineD{
                    p1,
                    perp(n)
                },
                p1,
                p2
            });
        }
    }

    return res;
}
```

### 现场记忆

```text
点到圆：
    圆内 0 条
    圆上 1 条
    圆外 2 条

切点：
    base = O + OP * r^2 / |OP|^2

两圆公切线：
    s = +1 -> 外公切线
    s = -1 -> 内公切线

r = r1 - s*r2
h^2 = |O1O2|^2 - r^2

单位法向量：
    n = (d*r ± perp(d)*h) / |d|^2

切线方向 = perp(n)
```

## 7.15 最小圆覆盖

> **用途**：求覆盖全部二维点的最小圆。
> **复杂度**：随机增量期望时间 `O(n)`，空间 `O(1)`（除输入副本）。
> **依赖**：7.13 圆基础。
> **注意**：必须随机打乱输入；三点最小覆盖圆不一定是外接圆，钝角三角形由最长边直径圆决定。

### 调用

```cpp
Circle c = minimum_enclosing_circle(p);

cout << c.o.x << ' '
     << c.o.y << ' '
     << c.r << '\n';
```

### 模板

```cpp
Circle circle_from_diameter(
    PointD a,
    PointD b
) {
    PointD o = (a + b) / 2.0;

    return {
        o,
        dist(a, b) / 2
    };
}

// 三个点的真正最小覆盖圆
Circle circle_from_three_mec(
    PointD a,
    PointD b,
    PointD c
) {
    array<Circle, 3> cand{
        circle_from_diameter(a, b),
        circle_from_diameter(a, c),
        circle_from_diameter(b, c)
    };

    bool found = false;
    Circle ans{};

    for (const Circle& x : cand) {
        if (!in_circle(a, x) ||
            !in_circle(b, x) ||
            !in_circle(c, x))
            continue;

        if (!found || x.r < ans.r) {
            ans = x;
            found = true;
        }
    }

    if (found)
        return ans;

    auto cc = circumcircle(a, b, c);

    if (cc)
        return *cc;

    // EPS 下近似共线时的兜底
    return *max_element(
        cand.begin(),
        cand.end(),
        [](const Circle& x, const Circle& y) {
            return x.r < y.r;
        }
    );
}

Circle minimum_enclosing_circle(
    vector<PointD> p
) {
    if (p.empty())
        return {{0, 0}, 0};

    static mt19937 rng(
        chrono::steady_clock::now()
            .time_since_epoch()
            .count()
    );

    shuffle(p.begin(), p.end(), rng);

    Circle c{p[0], 0};

    for (int i = 0; i < (int)p.size(); ++i) {
        if (in_circle(p[i], c))
            continue;

        c = {p[i], 0};

        for (int j = 0; j < i; ++j) {
            if (in_circle(p[j], c))
                continue;

            c = circle_from_diameter(
                p[i],
                p[j]
            );

            for (int k = 0; k < j; ++k) {
                if (in_circle(p[k], c))
                    continue;

                c = circle_from_three_mec(
                    p[i],
                    p[j],
                    p[k]
                );
            }
        }
    }

    return c;
}
```

### 现场记忆

```text
先 shuffle。

i 在圆外：
    C = 单点 i

j 在圆外：
    C = ij 直径圆

k 在圆外：
    C = i,j,k 三点 MEC

三点 MEC：
    若某条边的直径圆已覆盖三点 -> 取最小者
    否则 -> 三点外接圆

随机增量期望 O(n)。
```

# H4：凸集与优化

> **整数凸几何**：继续使用 `PointI / i128`，普通坐标加减必须满足 H1 的 `i64` 范围约定。
> **凸多边形**：默认使用 `convex_hull(raw)` 得到 CCW、无重复首点、无冗余边界共线点。

## 7.16 Minkowski Sum

> **用途**：求两个凸多边形的 Minkowski 和。
> **复杂度**：标准凸包输入下时间 `O(n+m)`，空间 `O(n+m)`。
> **依赖**：7.6 二维凸包。
> **限制**：输入应为 CCW、不重复首点、无冗余共线点的标准凸包；否则先调用 `convex_hull()`。
> **注意**：方向判断使用 `i128`，但结果坐标及普通向量运算仍必须能放入 `i64`。

### 调用

```cpp
auto a = convex_hull(A);
auto b = convex_hull(B);

auto c = minkowski_sum(a, b);
```

### 模板

```cpp
int polar_half(PointI v) {
    return v.y < 0 ||
           (v.y == 0 && v.x < 0);
}

// -1: a 极角更小
//  0: 同方向
// +1: b 极角更小
int polar_compare(
    PointI a,
    PointI b
) {
    int A = polar_half(a);
    int B = polar_half(b);

    if (A != B)
        return A < B ? -1 : 1;

    i128 c = cross(a, b);

    if (c > 0)
        return -1;

    if (c < 0)
        return 1;

    return 0;
}

void minkowski_rotate_start(
    vector<PointI>& p
) {
    if (p.empty())
        return;

    int s = 0;

    for (int i = 1; i < (int)p.size(); ++i) {
        if (p[i].y < p[s].y ||
            (p[i].y == p[s].y &&
             p[i].x < p[s].x))
            s = i;
    }

    rotate(
        p.begin(),
        p.begin() + s,
        p.end()
    );
}

vector<PointI> minkowski_sum(
    vector<PointI> a,
    vector<PointI> b
) {
    if (a.empty() || b.empty())
        return {};

    if (a.size() == 1) {
        for (PointI& p : b)
            p += a[0];

        return b;
    }

    if (b.size() == 1) {
        for (PointI& p : a)
            p += b[0];

        return a;
    }

    minkowski_rotate_start(a);
    minkowski_rotate_start(b);

    int n = (int)a.size();
    int m = (int)b.size();

    vector<PointI> ea(n), eb(m);

    for (int i = 0; i < n; ++i)
        ea[i] =
            a[(i + 1) % n] - a[i];

    for (int i = 0; i < m; ++i)
        eb[i] =
            b[(i + 1) % m] - b[i];

    vector<PointI> res;
    res.push_back(a[0] + b[0]);

    int i = 0, j = 0;

    while (i < n || j < m) {
        PointI e;

        if (i == n) {
            e = eb[j++];
        } else if (j == m) {
            e = ea[i++];
        } else {
            int cmp =
                polar_compare(ea[i], eb[j]);

            if (cmp < 0) {
                e = ea[i++];
            } else if (cmp > 0) {
                e = eb[j++];
            } else {
                e = ea[i++] + eb[j++];
            }
        }

        res.push_back(res.back() + e);
    }

    // 最后一条边回到起点
    res.pop_back();

    return res;
}
```

### 现场记忆

```text
标准凸多边形：
    起点 + 一圈按极角有序的边向量

先 rotate 到：
    y 最小，若并列 x 最小

Minkowski：
    起点 = a[0] + b[0]
    两组边向量按极角归并

同方向边：
    直接 ea + eb，一起推进

最后一步回到起点，要 pop_back()。
```

## 7.17 Convex Hull Trick：单调直线凸包

> **用途**：维护 `y = kx + b`，在单调 DP 中快速查询最大值。
> **复杂度**：`add_line()` 摊还 `O(1)`，`query()` 摊还 `O(1)`，空间 `O(n)`。
> **限制**：只支持 **MAX、斜率 `k` 非递减加入、查询 `x` 非递减**。
> **注意**：若加入或查询没有单调性，直接换 Li Chao Tree，不要魔改这份模板。

### 调用

```cpp
MonotoneCHTMax cht;

cht.add_line(k, b);

i128 ans = cht.query(x);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `add_line(k,b)` | 加入 `y=kx+b`，要求 `k` 非递减 |
| `query(x)` | 查询最大值，要求 `x` 非递减 |
| `clear()` | 清空 |
| `empty()` | 是否为空 |

### 模板

```cpp
struct CHTLine {
    i64 k, b;

    i128 eval(i64 x) const {
        return (i128)k * x + b;
    }
};

struct MonotoneCHTMax {
    deque<CHTLine> q;

    // pre: a.k < b.k < c.k
    static bool bad(
        const CHTLine& a,
        const CHTLine& b,
        const CHTLine& c
    ) {
        return
            ((i128)a.b - b.b) *
            ((i128)c.k - b.k)
            >=
            ((i128)b.b - c.b) *
            ((i128)b.k - a.k);
    }

    // pre: k 非递减
    void add_line(i64 k, i64 b) {
        CHTLine cur{k, b};

        // MAX：同斜率只留最大截距
        if (!q.empty() &&
            q.back().k == k) {

            if (q.back().b >= b)
                return;

            q.pop_back();
        }

        while (q.size() >= 2 &&
               bad(
                   q[q.size() - 2],
                   q.back(),
                   cur
               ))
            q.pop_back();

        q.push_back(cur);
    }

    // pre: q 非空，x 非递减
    i128 query(i64 x) {
        while (q.size() >= 2 &&
               q[1].eval(x) >=
               q[0].eval(x))
            q.pop_front();

        return q.front().eval(x);
    }

    bool empty() const {
        return q.empty();
    }

    void clear() {
        q.clear();
    }
};
```

### 现场记忆

```text
只记这一版：

    MAX
    k ↑ 加入
    x ↑ 查询

同斜率：
    MAX 只留 b 最大

三条线 a,b,c：
若交点(a,b) >= 交点(b,c)
    -> b 永远没用

全部交叉乘用 i128，不算浮点交点。

query：
    q[1](x) >= q[0](x)
    -> pop_front
```


# H5：三维几何基础

> **浮点几何**：继续使用 `db / EPS / sgn()`。
> **命名**：`dot / cross / len2 / len / dist / unit / angle / projection` 与二维通过参数类型自然重载。

## 7.18 三维几何基础

> **用途**：三维向量、直线、平面的基础运算，以及投影、距离、夹角和绕轴旋转。
> **复杂度**：所有接口时间 `O(1)`，空间 `O(1)`。
> **限制**：直线方向向量、平面法向量以及 `unit()` 输入向量必须非零。

### 接口速查

| 接口 | 含义 |
|---|---|
| `dot(a,b)` / `cross(a,b)` | 三维点积 / 叉积 |
| `len2/len/dist` | 长度平方 / 长度 / 距离 |
| `unit(a)` / `angle(a,b)` | 单位向量 / 向量夹角 |
| `Line3::through(a,b)` | 两点构造三维直线 |
| `projection(q,l)` | 点到直线投影 |
| `distance_to_line(q,l)` | 点到直线距离 |
| `projection(q,pl)` | 点到平面投影 |
| `distance_to_plane(q,pl)` | 点到平面距离 |
| `line_plane_relation(l,pl)` | 直线与平面关系 |
| `line_plane_intersection(l,pl)` | 唯一直线平面交点 |
| `line_angle(a,b)` | 两直线较小夹角 |
| `plane_angle(a,b)` | 两平面较小夹角 |
| `rotate_around_axis(v,axis,rad)` | Rodrigues 绕原点轴旋转 |

### 模板

```cpp
struct Point3 {
    db x{}, y{}, z{};

    Point3() = default;

    Point3(db x_, db y_, db z_)
        : x(x_), y(y_), z(z_) {}

    Point3 operator + (const Point3& p) const {
        return {
            x + p.x,
            y + p.y,
            z + p.z
        };
    }

    Point3 operator - (const Point3& p) const {
        return {
            x - p.x,
            y - p.y,
            z - p.z
        };
    }

    Point3 operator * (db k) const {
        return {
            x * k,
            y * k,
            z * k
        };
    }

    Point3 operator / (db k) const {
        return {
            x / k,
            y / k,
            z / k
        };
    }

    Point3& operator += (const Point3& p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    Point3& operator -= (const Point3& p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }
};

Point3 operator * (db k, Point3 p) {
    return p * k;
}

db dot(Point3 a, Point3 b) {
    return
        a.x * b.x +
        a.y * b.y +
        a.z * b.z;
}

Point3 cross(Point3 a, Point3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

db len2(Point3 a) {
    return dot(a, a);
}

db len(Point3 a) {
    return sqrt(len2(a));
}

db dist(Point3 a, Point3 b) {
    return len(a - b);
}

// pre: a 非零
Point3 unit(Point3 a) {
    return a / len(a);
}

// [0, pi]
// pre: a,b 非零
db angle(Point3 a, Point3 b) {
    return atan2(
        len(cross(a, b)),
        dot(a, b)
    );
}

struct Line3 {
    Point3 p;
    Point3 v; // p + t*v

    Line3() = default;

    Line3(Point3 p_, Point3 v_)
        : p(p_), v(v_) {}

    static Line3 through(
        Point3 a,
        Point3 b
    ) {
        return {a, b - a};
    }
};

struct Plane {
    Point3 p;
    Point3 n;

    Plane() = default;

    Plane(Point3 p_, Point3 n_)
        : p(p_), n(n_) {}
};

// pre: l.v 非零
Point3 projection(
    Point3 q,
    const Line3& l
) {
    return l.p +
           l.v *
           (dot(q - l.p, l.v) /
            len2(l.v));
}

Point3 closest_point_on_line(
    Point3 q,
    const Line3& l
) {
    return projection(q, l);
}

// pre: l.v 非零
db distance_to_line(
    Point3 q,
    const Line3& l
) {
    return
        len(cross(q - l.p, l.v)) /
        len(l.v);
}

// pre: pl.n 非零
Point3 projection(
    Point3 q,
    const Plane& pl
) {
    return q -
           pl.n *
           (dot(q - pl.p, pl.n) /
            len2(pl.n));
}

// pre: pl.n 非零
db distance_to_plane(
    Point3 q,
    const Plane& pl
) {
    return
        fabs(dot(q - pl.p, pl.n)) /
        len(pl.n);
}

enum class LinePlaneRelation {
    PARALLEL,
    CONTAINED,
    INTERSECT
};

LinePlaneRelation line_plane_relation(
    const Line3& l,
    const Plane& pl
) {
    db d = dot(l.v, pl.n);

    if (sgn(d) != 0)
        return LinePlaneRelation::INTERSECT;

    if (sgn(
        dot(l.p - pl.p, pl.n)
    ) == 0)
        return LinePlaneRelation::CONTAINED;

    return LinePlaneRelation::PARALLEL;
}

// 唯一交点；PARALLEL / CONTAINED -> nullopt
optional<Point3> line_plane_intersection(
    const Line3& l,
    const Plane& pl
) {
    db d = dot(l.v, pl.n);

    if (sgn(d) == 0)
        return nullopt;

    db t =
        dot(pl.p - l.p, pl.n) / d;

    return l.p + l.v * t;
}

// [0, pi/2]
// pre: 两方向向量非零
db line_angle(
    const Line3& a,
    const Line3& b
) {
    return atan2(
        len(cross(a.v, b.v)),
        fabs(dot(a.v, b.v))
    );
}

// [0, pi/2]
// pre: 两法向量非零
db plane_angle(
    const Plane& a,
    const Plane& b
) {
    return atan2(
        len(cross(a.n, b.n)),
        fabs(dot(a.n, b.n))
    );
}

// 向量 v 绕过原点、方向 axis 的轴按右手定则旋转
// pre: axis 非零
Point3 rotate_around_axis(
    Point3 v,
    Point3 axis,
    db rad
) {
    Point3 u = unit(axis);

    db c = cos(rad);
    db s = sin(rad);

    return
        v * c +
        cross(u, v) * s +
        u * (
            dot(u, v) *
            (1 - c)
        );
}
```

### 现场记忆

```text
点到直线投影：
    p + v * dot(q-p,v) / |v|^2

点到直线距离：
    |(q-p) x v| / |v|

点到平面投影：
    q - n * dot(q-p,n) / |n|^2

点到平面距离：
    |dot(q-p,n)| / |n|

直线平面交：
    t = dot(Pplane-Pline,n) / dot(v,n)

Rodrigues：
    v cosθ
    + (u×v) sinθ
    + u(u·v)(1-cosθ)
```
