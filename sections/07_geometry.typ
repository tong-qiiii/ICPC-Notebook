= 计算几何 <chapter-geometry>

#text("完整保留 raw/07_geometry.md 的 18 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#import "../styles/source.typ": source_code

=== #text("H1：二维计算几何底层")

#strong[#text("整数点")]#text("：")#raw("PointI = Point<long long>")#text("，方向 / 点积 / 距离平方 / 面积优先使用 ")#raw("__int128")#text("。")
#strong[#text("浮点点")]#text("：")#raw("PointD = Point<double>")#text("，")#raw("EPS = 1e-9")#text("。")
#strong[#text("方向约定")]#text("：")#raw("cross(a, b, c) > 0")#text(" 表示 ")#raw("a -> b -> c")#text(" 为 CCW。")
#strong[#text("直线约定")]#text("：")#raw("Line = p + t * v")#text("，左半平面满足 ")#raw("cross(v, q - p) >= 0")#text("。")
#strong[#text("凸包约定")]#text("：统一 CCW，不重复首点。")
#strong[#text("浮点相等")]#text("：")#raw("PointD::operator==")#text(" / ")#raw("operator<")#text(" 使用机器值；几何相等使用 ")#raw("same_point()")#text("。")

#text("整数 ")#raw("PointI")#text(" 的普通 ")#raw("+ - *")#text("、方向向量等仍存储为 ")#raw("long long")#text("，要求对应中间值不溢出。整数 predicate 以及面积累加同样要求最终结果可表示于 ")#raw("__int128")#text("。")

#raw("SegmentRelation:\nDISJOINT  无交\nTOUCH     唯一交点，且至少一条线段在端点处相交\nPROPER    两线段内部严格相交\nOVERLAP   共线且交集有正长度\n\nPointInPolygon:\nOUT / ON / IN",block:true)



=== 公共代码与复制顺序

本章源码块仅展示算法自身实现；公共骨架只打印一次。按依赖链接复制所需的公共代码，再复制本算法与调用示例，比赛时不需要仓库头文件。已有基础算法被复用时，直接复制其原条目，不在每个代码块里重贴。

*依赖索引：* #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)、#ref(<algo-geometry-convex-hull>, supplement: none)、#ref(<algo-geometry-triangle>, supplement: none)、#ref(<algo-geometry-circle>, supplement: none)。

=== 公共骨架：二维点与向量骨架 <shared-geometry-base>

完整代码只在此处展示一次；用途、接口和调用仍见 #ref(<algo-geometry-geometry-base>, supplement: none)。

#source_code(("geometry/geometry_base.hpp",))

== #text("二维几何基础") <algo-geometry-geometry-base>

#text("原稿来源：7.1 二维几何基础")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：二维点 / 向量基础运算，以及后续所有计算几何模板共享的方向、距离与旋转工具。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(1)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("注意")]#text("：")#raw("PointI")#text(" 普通坐标运算必须保证 ")#raw("long long")#text(" 不溢出；整数 predicate 的结果必须可表示于 ")#raw("__int128")#text("。")
#strong[#text("注意")]#text("：")#raw("PointD::operator==")#text(" / ")#raw("operator<")#text(" 不使用 EPS；几何意义下的相等使用 ")#raw("same_point()")#text("。")
#strong[#text("注意")]#text("：浮点判定统一使用绝对误差 ")#raw("EPS = 1e-9")#text("，极端坐标尺度时需按题目调整。")

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("dot(a, b)")],[#text("点积")],
[#raw("cross(a, b)")],[#text("二维叉积")],
[#raw("cross(a, b, c)")],[#raw("(b-a) × (c-a)")],
[#raw("len2(a)")#text(" / ")#raw("dist2(a,b)")],[#text("长度平方 / 距离平方")],
[#raw("len(a)")#text(" / ")#raw("dist(a,b)")],[#text("长度 / 距离")],
[#raw("perp(a)")],[#text("逆时针旋转 ")#raw("90°")],
[#raw("unit(a)")],[#text("单位向量，要求 ")#raw("a != 0")],
[#raw("rotate(a, rad)")],[#text("逆时针旋转")],
[#raw("angle(a, b)")],[#text("两非零向量夹角 ")#raw("[0, π]")],
[#raw("orient(a,b,c)")],[#raw("+1 / 0 / -1 = CCW / 共线 / CW")],
[#raw("same_point(a,b)")],[#text("整数精确；浮点按 EPS 相等")],
)])



=== #text("现场记忆")

#raw("cross(a,b,c) = cross(b-a,c-a)\ncross > 0：CCW / left；cross < 0：CW / right\n\ndot(a,b)   = |a||b| cos(theta)\ncross(a,b) = |a||b| sin(theta)\n\nS_triangle = |cross(A,B,C)| / 2\nrad = degree * PI / 180\ndegree = rad * 180 / PI\n\nPointD 几何相等用 same_point()，排序不要掺 EPS",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
PointI a{0, 0}, b{3, 0}, c{0, 4};
assert(cross(a, b, c) == 12 && dist2(b, c) == 25);
```

=== 模板引用

#source_code(("geometry/geometry_base.hpp",), shared: <shared-geometry-base>)


== #text("直线与线段") <algo-geometry-line-segment>

#text("原稿来源：7.2 直线与线段")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：直线 / 线段的位置关系、相交分类、交点、投影、对称与最近点。")
#strong[#text("复杂度")]#text("：所有接口时间 ")#raw("O(1)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-geometry-base>, supplement: none)#text(" 二维几何基础。")
#strong[#text("限制")]#text("：")#raw("Line")#text(" 使用 ")#raw("p + t * v")#text("，所有直线运算要求 ")#raw("v != 0")#text("；退化线段 ")#raw("a == b")#text(" 正常支持。")
#strong[#text("注意")]#text("：整数关系判断保持精确；最终构造非整数交点仍是浮点近似。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("Line::through(a,b)")],[#text("过两点构造直线")],
[#raw("on_line(p,l)")#text(" / ")#raw("side(l,p)")],[#text("点在线上 / 点在线的哪侧")],
[#raw("on_segment(p,s)")],[#text("在线段上，含端点")],
[#raw("on_segment_strict(p,s)")],[#text("在线段内部")],
[#raw("line_relation(a,b)")],[#text("平行 / 重合 / 相交")],
[#raw("line_intersection(a,b)")],[#text("唯一交点")],
[#raw("segment_relation(a,b)")],[#text("四态线段相交分类")],
[#raw("segment_intersect(a,b)")],[#text("非严格相交")],
[#raw("proper_segment_intersect(a,b)")],[#text("两线段内部严格交叉")],
[#raw("projection(q,l)")#text(" / ")#raw("reflection(q,l)")],[#text("投影 / 对称点")],
[#raw("closest_point_on_segment(q,s)")],[#text("线段上最近点")],
[#raw("distance_to_line/segment()")],[#text("点到直线 / 线段距离")],
)])



=== #text("现场记忆")

#raw("Line = p + t*v\nside(l,q) = sgn(cross(v, q-p))\n左侧 > 0，右侧 < 0\n\nproper intersection：\norient(A,B,C) * orient(A,B,D) < 0\n且反向也严格异侧\n\n共线线段：投影到变化更大的坐标轴，区间交为\nL < R -> OVERLAP；L == R -> TOUCH\n\nprojection 参数 t = dot(q-p,v) / |v|^2\n线段最近点：把 t clamp 到 [0,1]",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 精简后的接口约定

整数与浮点版本共用同名函数模板，调用写法不变，只需选择 `PointI / LineI / SegmentI` 或 `PointD / LineD / SegmentD`。整数方向判定仍用 `i128`，差值先提升；共线区间比较不做可能溢出的整数减法。浮点保留原 EPS 和端点判定，退化线段按坐标逐维 EPS 相等，退化直线按方向向量长度判定。两者阈值含义不同，不相互替代。直线 `through(a,b)` 的方向向量仍存为 `i64`，要求 b−a 不溢出；叉积及平方距离仍须落在 i128 范围内。浮点交点、投影等只在非退化且相应分母非零时使用。

=== 最小调用示例

```cpp
SegmentI a{{0, 0}, {2, 2}}, b{{0, 2}, {2, 0}};
assert(segment_relation(a, b) == SegmentRelation::PROPER);
assert(segment_relation(SegmentI{{0, 0}, {0, 0}}, a) ==
       SegmentRelation::TOUCH);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/line_segment.hpp",))


== #text("三角形常用工具") <algo-geometry-triangle>

#text("原稿来源：7.3 三角形常用工具")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：三角形面积、外心、内心、垂心、外接圆半径与内切圆半径。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(1)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-geometry-base>, supplement: none)#text(" 二维几何基础。")
#strong[#text("注意")]#text("：圆心 / 半径接口遇到退化三角形返回 ")#raw("nullopt")#text("。")

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("triangle_area2(a,b,c)")],[#text("有向两倍面积")],
[#raw("triangle_area(a,b,c)")],[#text("非负面积")],
[#raw("circumcenter(a,b,c)")],[#text("外心")],
[#raw("incenter(a,b,c)")],[#text("内心")],
[#raw("orthocenter(a,b,c)")],[#text("垂心")],
[#raw("circumradius(a,b,c)")],[#text("外接圆半径")],
[#raw("inradius(a,b,c)")],[#text("内切圆半径")],
)])



=== #text("现场记忆")

#raw("S = |cross(A,B,C)| / 2\ns = (a+b+c)/2\n\nR = abc / (4S)\nr = S / s\n\n内心 = (a*A + b*B + c*C) / (A+B+C)\n其中 A,B,C 分别是对应顶点的对边长度\n\n垂心 H = A + B + C - 2O",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto o = circumcenter({0, 0}, {4, 0}, {0, 2});
assert(o && same_point(*o, {2, 1}));
assert(!circumcenter({0, 0}, {1, 0}, {2, 0}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/triangle.hpp",))


== #text("多边形基础") <algo-geometry-polygon>

#text("原稿来源：7.4 多边形基础")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：多边形面积、方向、点在简单多边形内判定以及多边形重心。")
#strong[#text("复杂度")]#text("：所有主要接口时间 ")#raw("O(n)")#text("，额外空间 ")#raw("O(1)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-geometry-base>, supplement: none)#text(" 二维几何基础；")#raw("point_in_polygon()")#text(" 依赖 ")#ref(<algo-geometry-line-segment>, supplement: none)#text(" 的 ")#raw("on_segment()")#text("。")
#strong[#text("限制")]#text("：")#raw("point_in_polygon()")#text(" 针对简单多边形；")#raw("polygon_centroid()")#text(" 仅对非退化简单多边形有意义。")
#strong[#text("注意")]#text("：")#raw("polygon_area2() > 0")#text(" 表示 CCW，后续凸包统一保持 CCW 且不重复首点。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("polygon_area2(p)")],[#text("有向两倍面积")],
[#raw("polygon_area(p)")],[#text("非负面积")],
[#raw("polygon_direction(p)")],[#raw("CW / CCW / DEGENERATE")],
[#raw("point_in_polygon(p,q)")],[#raw("OUT / ON / IN")],
[#raw("polygon_centroid(p)")],[#text("重心，退化返回 ")#raw("nullopt")],
)])



=== #text("现场记忆")

#raw("polygon_area2 = sum cross(p[i], p[i+1])\narea2 > 0 -> CCW；area2 < 0 -> CW\n\n点在边上必须先判 ON\n\nwinding：\n向上穿过 q.y 且 q 在边左侧 -> +1\n向下穿过 q.y 且 q 在边右侧 -> -1\n非零 -> IN\n\n重心分母 = 3 * sum cross(p[i],p[i+1])",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
vector<PointI> p{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
assert(polygon_area2(p) == 32 &&
       point_in_polygon(p, PointI{2, 2}) == PointInPolygon::IN);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/polygon.hpp",))


== #text("格点多边形：Pick 定理") <algo-geometry-pick>

#text("原稿来源：7.5 格点多边形：Pick 定理")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：计算整点线段格点数、简单格点多边形边界格点数与内部格点数。")
#strong[#text("复杂度")]#text("：单条线段 ")#raw("O(log C)")#text("；")#raw("n")#text(" 边多边形 ")#raw("O(n log C)")#text("，其中 ")#raw("C")#text(" 为坐标差范围。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-polygon>, supplement: none)#text(" 的 ")#raw("polygon_area2()")#text("。")
#strong[#text("限制")]#text("：Pick 定理仅适用于非退化简单格点多边形。")


=== #text("现场记忆")

#raw("整点线段：\ng = gcd(|dx|, |dy|)\n闭线段格点数 = g + 1\n\n多边形边界：\nB = sum gcd(|dx|, |dy|)\n不能对每条边累加 g+1，否则顶点重复\n\nPick：\nA = I + B/2 - 1\nA2 = 2I + B - 2\nI = (A2 - B + 2) / 2",block:true)


=== #text("H2：凸几何")

#strong[#text("默认整数几何")]#text("：")#raw("PointI / i128 / cross / orient / dist2")#text("。")
#strong[#text("凸包约定")]#text("：")#raw("convex_hull(raw)")#text(" 默认去重、删除边界中间共线点、CCW、不重复首点。")
#strong[#text("卡壳约定")]#text("：")#ref(<algo-geometry-convex-diameter>, supplement: none)#text(" / ")#ref(<algo-geometry-bounding-rectangle>, supplement: none)#text(" 默认直接使用 ")#raw("convex_hull(raw)")#text("。")
#strong[#text("距离比较")]#text("：优先比较 ")#raw("dist2")#text("，不做无意义的 ")#raw("sqrt()")#text("。")
#strong[#text("点包含")]#text("：继续使用 H1 的 ")#raw("PointInPolygon::OUT / ON / IN")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(interior_lattice_points(
           vector<PointI>{{0, 0}, {3, 0}, {3, 3}, {0, 3}}) == 4);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/pick.hpp",))


== #text("二维凸包 Andrew") <algo-geometry-convex-hull>

#text("原稿来源：7.6 二维凸包 Andrew")


#strong[#text("用途")]#text("：求二维点集凸包。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：返回凸包顶点 ")#raw("0-indexed")#text("。")
#strong[#text("注意")]#text("：默认删除边界中间共线点；")#ref(<algo-geometry-convex-diameter>, supplement: none)#text(" / ")#ref(<algo-geometry-bounding-rectangle>, supplement: none)#text(" 推荐直接使用默认输出。")





=== #text("现场记忆")

#raw("Andrew：\n先按 (x,y) 排序 + 去重\n\n默认无共线：\n    while cross <= 0 -> pop\n\n保留边界共线：\n    while cross < 0 -> pop\n\n非退化结果：\n    CCW\n    不重复首点\n\n全共线：\n    false -> 两端点\n    true  -> 所有不同点",block:true)



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto h =
    convex_hull(vector<PointI>{{0, 0}, {2, 0}, {0, 2}, {1, 0}, {0, 0}});
assert(h.size() == 3);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/convex_hull.hpp",))


== #text("凸多边形点包含 O(n)，含退化检查") <algo-geometry-convex-contains>

#text("原稿来源：7.7 凸多边形点包含 O(log n)")


#strong[#text("用途")]#text("：判断点位于凸多边形外部、边界或内部。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("（面积判退化为线性，扇形二分为 O(log n)），空间 ")#raw("O(1)")#text("。")
#strong[#text("下标")]#text("：凸多边形顶点 ")#raw("0-indexed")#text("。")
#strong[#text("限制")]#text("：输入按 CCW 排列，不重复首点。")
#strong[#text("注意")]#text("：兼容 ")#raw("convex_hull(raw)")#text("，也兼容 ")#raw("convex_hull(raw, true)")#text(" 保留边界共线点的输出。")





=== #text("现场记忆")

#raw("以 p[0] 为扇形原点。\n\n先判：\n    q 是否位于首射线与尾射线之间\n\n落在首 / 尾射线：\n    直接 on_segment 判 ON\n\n否则二分找到：\n    angle(p[l]) <= angle(q) < angle(p[l+1])\n\n最后看：\n    orient(p[l], p[l+1], q)\n\n> 0 -> IN\n= 0 -> ON\n< 0 -> OUT",block:true)



=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto h = convex_hull(vector<PointI>{{0, 0}, {4, 0}, {4, 4}, {0, 4}});
assert(point_in_convex_polygon(h, {2, 2}) == PointInPolygon::IN);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)、#ref(<algo-geometry-convex-hull>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/convex_contains.hpp",))


== #text("凸多边形点包含 O(n)，含退化检查") <algo-geometry-convex-contains-variant>

#text("原稿来源：7.8 凸多边形点包含 O(log n)")


#strong[#text("用途")]#text("：判断点位于凸多边形外部、边界或内部。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("（面积判退化为线性，扇形二分为 O(log n)），空间 ")#raw("O(1)")#text("。")
#strong[#text("下标")]#text("：凸多边形顶点 ")#raw("0-indexed")#text("。")
#strong[#text("限制")]#text("：输入按 CCW 排列，不重复首点。")
#strong[#text("注意")]#text("：兼容 ")#raw("convex_hull(raw)")#text("，也兼容 ")#raw("convex_hull(raw, true)")#text(" 保留边界共线点的输出。")





=== #text("现场记忆")

#raw("以 p[0] 为扇形原点。\n\n先判：\n    q 是否位于首射线与尾射线之间\n\n落在首 / 尾射线：\n    直接 on_segment 判 ON\n\n否则二分找到：\n    angle(p[l]) <= angle(q) < angle(p[l+1])\n\n最后看：\n    orient(p[l], p[l+1], q)\n\n> 0 -> IN\n= 0 -> ON\n< 0 -> OUT",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto h = convex_hull(vector<PointI>{{0, 0}, {4, 0}, {4, 4}, {0, 4}});
assert(point_in_convex_polygon(h, {2, 2}) == PointInPolygon::IN);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)、#ref(<algo-geometry-convex-hull>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/convex_contains_variant.hpp",))


== #text("旋转卡壳：凸包直径") <algo-geometry-convex-diameter>

#text("原稿来源：7.9 旋转卡壳：凸包直径")


#strong[#text("用途")]#text("：求凸包最远点对及最大距离平方。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("下标")]#text("：凸包顶点 ")#raw("0-indexed")#text("。")
#strong[#text("限制")]#text("：推荐输入 ")#raw("convex_hull(raw)")#text(" 的 CCW、无冗余共线点凸包。")
#strong[#text("注意")]#text("：返回距离平方 ")#raw("d2")#text("，不要为了比较距离调用 ")#raw("sqrt()")#text("。")





=== #text("现场记忆")

#raw("固定凸包边：\n    i -> i+1\n\n只要：\n    area(i, j+1) > area(i, j)\n就继续推进 j\n\narea =\n    |cross(p[i], p[i+1], p[j])|\n\n每条边检查：\n    (i, j)\n    (i+1, j)\n\n面积出现平台时顺手检查 j+1。\n\nj 只向前走 -> O(n)\n最终答案比较 dist2，不 sqrt。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
auto h = convex_hull(vector<PointI>{{0, 0}, {3, 0}, {0, 4}});
assert(convex_diameter(h)->d2 == 25);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)、#ref(<algo-geometry-convex-hull>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/convex_diameter.hpp",))


== #text("旋转卡壳：最小面积外接矩形") <algo-geometry-bounding-rectangle>

#text("原稿来源：7.10 旋转卡壳：最小面积外接矩形")


#strong[#text("用途")]#text("：求整数凸包的最小面积外接矩形面积。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("下标")]#text("：凸包顶点 ")#raw("0-indexed")#text("。")
#strong[#text("限制")]#text("：推荐输入 ")#raw("convex_hull(raw)")#text(" 的 CCW、无冗余共线点凸包。")
#strong[#text("注意")]#text("：三个支撑点的比较全部使用 ")#raw("i128")#text("；只有最终面积转为浮点。")
#strong[#text("注意")]#text("：第一条边必须先 ")#raw("O(n)")#text(" 找到三个真实支撑点，不能简单把指针设为 ")#raw("0")#text(" 后只向前试探。")





=== #text("现场记忆")

#raw("当前边方向 e 固定：\n\nj：cross(e, p) 最大\nk：dot(e, p) 最大\nl：dot(e, p) 最小\n\nheight = cross_max / |e|\nwidth  = (dot_max - dot_min) / |e|\n\narea =\n    cross_max\n    * (dot_max - dot_min)\n    / |e|^2\n\n第一条边先 O(n) 找真实 j / k / l，\n之后三个指针只向前移动。\n\n卡壳比较全程 i128，无需 EPS。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto h = convex_hull(vector<PointI>{{0, 0}, {3, 0}, {3, 2}, {0, 2}});
assert(fabs(min_area_bounding_rectangle(h) - 6) < 1e-8);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)、#ref(<algo-geometry-convex-hull>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/bounding_rectangle.hpp",))


== #text("最近点对") <algo-geometry-closest-pair>

#text("原稿来源：7.11 最近点对")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：求整数点集中最近的两个点及最小距离平方。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("注意")]#text("：距离判断全部使用 ")#raw("i128 dist2")#text("，不调用 ")#raw("sqrt()")#text("；重复点会得到 ")#raw("d2 = 0")#text("。")





=== #text("现场记忆")

#raw("开始：\n    全局按 x 排序\n\nsolve(l,r)：\n    先保存 mid_x\n    递归左右\n    返回后把当前区间 merge 成按 y 排序\n\n跨中线只考虑：\n    (x - mid_x)^2 < best\n\nstrip 已按 y 排序，\n向前检查直到：\n    dy^2 >= best\n即可停止。\n\n所有比较用 dist2 / i128，不 sqrt。",block:true)

=== #text("H3：半平面与圆")

#strong[#text("浮点几何")]#text("：")#raw("PointD / LineD / db")#text("，统一使用 ")#raw("EPS = 1e-9")#text("。")
#strong[#text("直线")]#text("：")#raw("p + t * v")#text("。")
#strong[#text("半平面")]#text("：有向直线保留左侧，即 ")#raw("cross(v, q-p) >= 0")#text("，包含边界。")
#strong[#text("圆")]#text("：")#raw("Circle{o, r}")#text("，默认要求 ")#raw("r >= 0")#text("。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
assert(closest_pair(vector<PointI>{{0, 0}, {3, 4}, {0, 1}})->d2 == 1);
assert(!closest_pair({}));
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/closest_pair.hpp",))


== #text("半平面交") <algo-geometry-halfplane-intersection>

#text("原稿来源：7.12 半平面交")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：求若干左半平面的有界正面积交集多边形。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-geometry-base>, supplement: none)#text(" / ")#ref(<algo-geometry-line-segment>, supplement: none)#text(" / ")#ref(<algo-geometry-polygon>, supplement: none)#text("。")
#strong[#text("限制")]#text("：若非空交集可能无界，必须根据题意先加入合法外框；本函数不负责无界判定。")
#strong[#text("注意")]#text("：")#raw("HalfPlane(a,b)")#text(" 保留 ")#raw("a -> b")#text(" 左侧；返回 CCW、不重复首点。")
#strong[#text("注意")]#text("：空返回表示在“交集应有界”的前提下，没有正面积二维区域；空集与退化成点 / 线段均不区分。")





=== #text("现场记忆")

#raw("HalfPlane(a,b)：\n    保留 a -> b 左侧\n    cross(b-a, q-a) >= 0\n\n极角排序后：\n    同方向平行线只留更严格的一条\n\n新半平面 x：\n    尾部交点在 x 外 -> pop_back\n    首部交点在 x 外 -> pop_front\n\n最后首约束尾、尾约束首，再取相邻直线交点。\n\n这版不检测无界：\n    可能无界 -> 题目层面先加合法外框",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
vector<HalfPlane> h{{{0, 0}, {2, 0}},
                    {{2, 0}, {2, 3}},
                    {{2, 3}, {0, 3}},
                    {{0, 3}, {0, 0}}};
auto p = halfplane_intersection(h);
assert(fabs(polygon_area(p) - 6) < 1e-8);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-polygon>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/halfplane_intersection.hpp",))


== #text("圆基础") <algo-geometry-circle>

#text("原稿来源：7.13 圆基础")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：点 / 直线 / 两圆关系，圆交点、两圆公共面积与三点外接圆。")
#strong[#text("复杂度")]#text("：所有接口时间 ")#raw("O(1)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-geometry-base>, supplement: none)#text(" / ")#ref(<algo-geometry-line-segment>, supplement: none)#text(" / ")#ref(<algo-geometry-triangle>, supplement: none)#text("。")
#strong[#text("限制")]#text("：")#raw("Circle.r >= 0")#text("；")#raw("r = 0")#text(" 允许表示退化点圆。")

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("point_circle_relation(p,c)")],[#text("点与圆关系")],
[#raw("in_circle(p,c)")],[#text("是否在圆盘内或边界")],
[#raw("line_circle_relation(l,c)")],[#text("直线与圆关系")],
[#raw("line_circle_intersection(l,c)")],[#text("直线圆交点")],
[#raw("circle_relation(a,b)")],[#text("两圆位置关系")],
[#raw("circle_circle_intersection(a,b)")],[#text("两圆交点")],
[#raw("circle_intersection_area(a,b)")],[#text("两圆盘公共面积")],
[#raw("circumcircle(a,b,c)")],[#text("三点外接圆")],
)])



=== #text("现场记忆")

#raw("直线圆：\n    先投影圆心 H\n    h2 = r^2 - |OH|^2\n    <0 / =0 / >0 -> 0 / 1 / 2 个交点\n\n两圆：\n    d 与 r1+r2 比较 -> 外离 / 外切\n    d 与 |r1-r2| 比较 -> 内含 / 内切\n    中间 -> 两交点\n\n两圆交点：\n    x = (d^2+r1^2-r2^2)/(2d)\n    h^2 = r1^2-x^2\n\n两圆公共面积：\n    外离 0\n    内含 = 小圆面积\n    其余 = 两个圆弓面积",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Circle a{{0, 0}, 1}, b{{2, 0}, 1};
assert(circle_relation(a, b) == CircleRelation::EXTERNALLY_TANGENT);
assert(circle_circle_intersection(a, b).points.size() == 1);
assert(fabs(circle_intersection_area(a, a) - PI) < 1e-8);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-triangle>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/circle.hpp",))


== #text("圆的切线") <algo-geometry-circle-tangents>

#text("原稿来源：7.14 圆的切线")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：求点到圆的切点 / 切线，以及两个圆的所有公切线。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(1)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-circle>, supplement: none)#text(" 圆基础。")
#strong[#text("限制")]#text("：本模板切线部分要求圆半径 ")#raw("r > 0")#text("。")
#strong[#text("注意")]#text("：两圆相切时切点可能重合，因此公切线直接用法向量构造，不能 ")#raw("LineD::through(p,p)")#text("。")

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("tangent_points_from_point(p,c)")],[#text("点到圆的切点")],
[#raw("tangents_from_point(p,c)")],[#text("点到圆的切线")],
[#raw("common_tangents(a,b)")],[#text("两圆全部公切线")],
)])



=== #text("现场记忆")

#raw("点到圆：\n    圆内 0 条\n    圆上 1 条\n    圆外 2 条\n\n切点：\n    base = O + OP * r^2 / |OP|^2\n\n两圆公切线：\n    s = +1 -> 外公切线\n    s = -1 -> 内公切线\n\nr = r1 - s*r2\nh^2 = |O1O2|^2 - r^2\n\n单位法向量：\n    n = (d*r ± perp(d)*h) / |d|^2\n\n切线方向 = perp(n)",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
auto r = common_tangents(Circle{{0, 0}, 1}, Circle{{2, 0}, 1});
assert(r.tangents.size() == 3);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-triangle>, supplement: none)、#ref(<algo-geometry-circle>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/circle_tangents.hpp",))


== #text("最小圆覆盖") <algo-geometry-minimum-enclosing-circle>

#text("原稿来源：7.15 最小圆覆盖")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：求覆盖全部二维点的最小圆。")
#strong[#text("复杂度")]#text("：随机增量期望时间 ")#raw("O(n)")#text("，空间 ")#raw("O(1)")#text("（除输入副本）。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-circle>, supplement: none)#text(" 圆基础。")
#strong[#text("注意")]#text("：必须随机打乱输入；三点最小覆盖圆不一定是外接圆，钝角三角形由最长边直径圆决定。")





=== #text("现场记忆")

#raw("先 shuffle。\n\ni 在圆外：\n    C = 单点 i\n\nj 在圆外：\n    C = ij 直径圆\n\nk 在圆外：\n    C = i,j,k 三点 MEC\n\n三点 MEC：\n    若某条边的直径圆已覆盖三点 -> 取最小者\n    否则 -> 三点外接圆\n\n随机增量期望 O(n)。",block:true)

=== #text("H4：凸集与优化")

#strong[#text("整数凸几何")]#text("：继续使用 ")#raw("PointI / i128")#text("，普通坐标加减必须满足 H1 的 ")#raw("i64")#text(" 范围约定。")
#strong[#text("凸多边形")]#text("：默认使用 ")#raw("convex_hull(raw)")#text(" 得到 CCW、无重复首点、无冗余边界共线点。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto c =
    minimum_enclosing_circle(vector<PointD>{{0, 0}, {4, 0}, {1, 1}});
assert(same_point(c.o, {2, 0}) && fabs(c.r - 2) < 1e-8);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-line-segment>, supplement: none)、#ref(<algo-geometry-triangle>, supplement: none)、#ref(<algo-geometry-circle>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/minimum_enclosing_circle.hpp",))


== #text("Minkowski Sum") <algo-geometry-minkowski-sum>

#text("原稿来源：7.16 Minkowski Sum")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：求两个凸多边形的 Minkowski 和。")
#strong[#text("复杂度")]#text("：标准凸包输入下时间 ")#raw("O(n+m)")#text("，空间 ")#raw("O(n+m)")#text("。")
#strong[#text("依赖")]#text("：")#ref(<algo-geometry-convex-hull>, supplement: none)#text(" 二维凸包。")
#strong[#text("限制")]#text("：输入应为 CCW、不重复首点、无冗余共线点的标准凸包；否则先调用 ")#raw("convex_hull()")#text("。")
#strong[#text("注意")]#text("：方向判断使用 ")#raw("i128")#text("，但结果坐标及普通向量运算仍必须能放入 ")#raw("i64")#text("。")





=== #text("现场记忆")

#raw("标准凸多边形：\n    起点 + 一圈按极角有序的边向量\n\n先 rotate 到：\n    y 最小，若并列 x 最小\n\nMinkowski：\n    起点 = a[0] + b[0]\n    两组边向量按极角归并\n\n同方向边：\n    直接 ea + eb，一起推进\n\n最后一步回到起点，要 pop_back()。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto p =
    minkowski_sum(convex_hull(vector<PointI>{{0, 0}, {1, 0}, {0, 1}}),
                  vector<PointI>{{1, 2}});
assert(p.size() == 3);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]、#ref(<algo-geometry-convex-hull>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/minkowski_sum.hpp",))


== #text("Convex Hull Trick：单调直线凸包") <algo-geometry-monotone-cht>

#text("原稿来源：7.17 Convex Hull Trick：单调直线凸包")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：维护 ")#raw("y = kx + b")#text("，在单调 DP 中快速查询最大值。")
#strong[#text("复杂度")]#text("：")#raw("add_line()")#text(" 摊还 ")#raw("O(1)")#text("，")#raw("query()")#text(" 摊还 ")#raw("O(1)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("限制")]#text("：只支持 ")#strong[#text("MAX、斜率 `k` 非递减加入、查询 `x` 非递减")]#text("。")
#strong[#text("注意")]#text("：若加入或查询没有单调性，直接换 Li Chao Tree，不要魔改这份模板。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("add_line(k,b)")],[#text("加入 ")#raw("y=kx+b")#text("，要求 ")#raw("k")#text(" 非递减")],
[#raw("query(x)")],[#text("查询最大值，要求 ")#raw("x")#text(" 非递减")],
[#raw("clear()")],[#text("清空")],
[#raw("empty()")],[#text("是否为空")],
)])



=== #text("现场记忆")

#raw("只记这一版：\n\n    MAX\n    k ↑ 加入\n    x ↑ 查询\n\n同斜率：\n    MAX 只留 b 最大\n\n三条线 a,b,c：\n若交点(a,b) >= 交点(b,c)\n    -> b 永远没用\n\n全部交叉乘用 i128，不算浮点交点。\n\nquery：\n    q[1](x) >= q[0](x)\n    -> pop_front",block:true)


=== #text("H5：三维几何基础")

#strong[#text("浮点几何")]#text("：继续使用 ")#raw("db / EPS / sgn()")#text("。")
#strong[#text("命名")]#text("：")#raw("dot / cross / len2 / len / dist / unit / angle / projection")#text(" 与二维通过参数类型自然重载。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
MonotoneCHTMax h;
h.add_line(-1, 2);
h.add_line(1, 0);
assert(h.query(3) == 3);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/monotone_cht.hpp",))


== #text("三维几何基础") <algo-geometry-geometry-3d>

#text("原稿来源：7.18 三维几何基础")

#strong[#text("下标")]#text("：点、向量和标量接口无数组下标；输入 / 返回点容器 0-indexed。")


#strong[#text("用途")]#text("：三维向量、直线、平面的基础运算，以及投影、距离、夹角和绕轴旋转。")
#strong[#text("复杂度")]#text("：所有接口时间 ")#raw("O(1)")#text("，空间 ")#raw("O(1)")#text("。")
#strong[#text("限制")]#text("：直线方向向量、平面法向量以及 ")#raw("unit()")#text(" 输入向量必须非零。")

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("dot(a,b)")#text(" / ")#raw("cross(a,b)")],[#text("三维点积 / 叉积")],
[#raw("len2/len/dist")],[#text("长度平方 / 长度 / 距离")],
[#raw("unit(a)")#text(" / ")#raw("angle(a,b)")],[#text("单位向量 / 向量夹角")],
[#raw("Line3::through(a,b)")],[#text("两点构造三维直线")],
[#raw("projection(q,l)")],[#text("点到直线投影")],
[#raw("distance_to_line(q,l)")],[#text("点到直线距离")],
[#raw("projection(q,pl)")],[#text("点到平面投影")],
[#raw("distance_to_plane(q,pl)")],[#text("点到平面距离")],
[#raw("line_plane_relation(l,pl)")],[#text("直线与平面关系")],
[#raw("line_plane_intersection(l,pl)")],[#text("唯一直线平面交点")],
[#raw("line_angle(a,b)")],[#text("两直线较小夹角")],
[#raw("plane_angle(a,b)")],[#text("两平面较小夹角")],
[#raw("rotate_around_axis(v,axis,rad)")],[#text("Rodrigues 绕原点轴旋转")],
)])



=== #text("现场记忆")

#raw("点到直线投影：\n    p + v * dot(q-p,v) / |v|^2\n\n点到直线距离：\n    |(q-p) x v| / |v|\n\n点到平面投影：\n    q - n * dot(q-p,n) / |n|^2\n\n点到平面距离：\n    |dot(q-p,n)| / |n|\n\n直线平面交：\n    t = dot(Pplane-Pline,n) / dot(v,n)\n\nRodrigues：\n    v cosθ\n    + (u×v) sinθ\n    + u(u·v)(1-cosθ)",block:true)

=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
auto p = rotate_around_axis(Point3{1, 0, 0}, Point3{0, 0, 1}, PI / 2);
assert(dist(p, {0, 1, 0}) < 1e-8);
```

=== 模板引用

*代码依赖：* 先复制 #link(<shared-geometry-base>)[二维点与向量骨架]；各依赖只复制一次，下方仅展示本节实现。

#source_code(("geometry/geometry_3d.hpp",))

== 扫描线：矩形面积并 <algo-geometry-rectangle-union>

*用途：* 求轴对齐矩形覆盖区域的总面积；重叠部分只计一次。整数坐标精确计算，不依赖浮点几何骨架。

*复杂度：* n 个矩形，时间 `O(n log n)`，空间 `O(n)`，线段树深度 `O(log n)`。

*下标与接口：* `rectangle_union(a)`，a 是 0-index 的 `{x1,y1,x2,y2}` 列表；返回 `__int128_t`。自动整理端点顺序、忽略零面积矩形，空输入返回 0，无全局状态。

*注意事项：* 坐标绝对值不超过 `1e18`，差值和覆盖长度用 ll，面积用 i128；输出可复制工具章的 i128 输出方法。离散后的叶子 i 表示 `[ys[i],ys[i+1])`，故更新到 `lower_bound(y2)-1`。同 x 的事件依次处理面积增量为 0，不会重复计面积。本模板不求周长并；周长不能照搬逐事件面积写法。

*现场记忆：* 先用旧覆盖长度乘 x 的增量，再更新本条边；cnt>0 整段被盖，cnt=0 才合并子节点长度。

=== 最小调用示例

```cpp
vector<array<ll, 4>> a = {{0, 0, 2, 2}, {1, 1, 3, 3}};
assert(rectangle_union(a) == 7);
assert(rectangle_union({}) == 0);
```

=== 模板引用

#source_code(("geometry/rectangle_union.hpp",))

