= 区间贪心 / 双指针 <chapter-interval-greedy>

#text("完整保留 raw/05_interval_greedy.md 的 5 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#let source_code(files) = {
  let source = files.map(file => read("../code/" + file)
    .replace(regex("(?m)^#pragma once\r?\n"), "")
    .replace(regex("(?m)^#include \".*\"\r?\n"), "")).join("\n")
  raw(source, lang: "cpp", block: true)
}

== #text("区间集合合法二元组计数") <algo-interval-greedy-count-pairs-small>

#text("原稿来源：5.1 区间集合合法二元组计数")


#strong[#text("用途")]#text("：统计 ")#raw("1 <= l <= r <= n")#text(" 中，满足存在给定区间 ")#raw("[a,b] ⊆ [l,r]")#text(" 的二元组 ")#raw("(l,r)")#text(" 数量。")
#strong[#text("复杂度")]#text("：小值域时间 ")#raw("O(n + m)")#text("、空间 ")#raw("O(n + m)")#text("；大值域时间 ")#raw("O(m log m)")#text("、空间 ")#raw("O(m)")#text("。")
#strong[#text("下标")]#text("：")#raw("segs[1..m]")#text("，值域 ")#raw("1..n")#text("。")
#strong[#text("限制")]#text("：输入区间满足 ")#raw("1 <= a <= b <= n")#text("，答案需能放入 ")#raw("ll")#text("。")





=== #text("现场记忆")

#text("固定左端点 ")#raw("l")#text("，只看所有满足 ")#raw("a >= l")#text(" 的给定区间。")

#text("其中最小右端点记为 ")#raw("mn")#text("，则合法 ")#raw("r")#text(" 恰好是 ")#raw("mn..n")#text("。")

#text("小值域从 ")#raw("n")#text(" 往 ")#raw("1")#text(" 扫；大值域中 ")#raw("mn")#text(" 只会在某个区间左端点处改变。")

#text("一段左端点长度 ")#raw("len")#text(" 的贡献就是 ")#raw("len * (n - mn + 1)")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(count_pairs_small(3, vector<pll>{{}, {1, 2}}) == 2);
```

=== 模板引用

#source_code(("interval_greedy/count_pairs_small.hpp",))

==== #text("count_pairs_large")

```cpp
assert(count_pairs_large(3, vector<pll>{{}, {1, 2}}) == 2);
```

#source_code(("interval_greedy/count_pairs_large.hpp",))

== #text("区间最小点覆盖 / 最大不相交区间数") <algo-interval-greedy-min-points>

#text("原稿来源：5.2 区间最小点覆盖 / 最大不相交区间数")


#strong[#text("用途")]#text("：求覆盖所有闭区间的最少点集，或最多选出多少个两两不相交闭区间。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：")#raw("segs[1..n]")#text("；")#raw("min_points()")#text(" 返回 ")#raw("points[1..k]")#text("。")
#strong[#text("注意")]#text("：按闭区间处理，")#raw("[a,b]")#text(" 与 ")#raw("[b,c]")#text(" 算相交；若题目允许端点相接，判断改成对应的 ")#raw(">= / <=")#text("。")





=== #text("现场记忆")

#text("两个问题都先按右端点从小到大排序。")

#text("最小点覆盖：当前已选点不在新区间内，就选新区间右端点。")

#text("最大不相交：新区间满足 ")#raw("l > last")#text(" 才选，再令 ")#raw("last = r")#text("。")

#text("闭区间端点相等仍然相交。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto p = min_points(vector<pll>{{}, {1, 2}, {2, 3}});
assert(p.size() == 2 && p[1] == 2);
```

=== 模板引用

#source_code(("interval_greedy/min_points.hpp",))

==== #text("max_disjoint")

```cpp
assert(max_disjoint(vector<pll>{{}, {1, 2}, {2, 3}, {3, 4}}) == 2);
```

#source_code(("interval_greedy/max_disjoint.hpp",))

== #text("区间分组") <algo-interval-greedy-interval-group>

#text("原稿来源：5.3 区间分组")


#strong[#text("用途")]#text("：把若干闭区间分到最少组，使每组内部区间两两不相交，并返回每个区间的组号。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：输入 ")#raw("segs[1..n]")#text("，")#raw("bel[1..n]")#text(" 对应原区间，组号为 ")#raw("1..cnt")#text("。")
#strong[#text("注意")]#text("：闭区间只有 ")#raw("上一段.r < 当前段.l")#text(" 才能复用同一组；半开区间改成 ")#raw("<=")#text("。")





=== #text("现场记忆")

#text("先按左端点排序，小根堆维护每组当前最后一个区间的右端点。")

#raw("q.top().x < l")#text("：最早结束的组可以复用。")

#text("否则连最早结束的组都冲突，所有已有组都不能用，必须新开组。")

#text("堆中始终每组只有一个当前结束位置。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
auto [cnt, bel] = interval_group(vector<pll>{{}, {1, 2}, {2, 3}});
assert(cnt == 2 && bel[1] != bel[2]);
```

=== 模板引用

#source_code(("interval_greedy/interval_group.hpp",))


== #text("区间覆盖目标区间") <algo-interval-greedy-min-cover>

#text("原稿来源：5.4 区间覆盖目标区间")


#strong[#text("用途")]#text("：从若干闭区间中选择最少个，使其连续覆盖目标闭区间 ")#raw("[L,R]")#text("。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")#text("。")
#strong[#text("下标")]#text("：")#raw("segs[1..n]")#text("。")
#strong[#text("限制")]#text("：目标区间满足 ")#raw("L <= R")#text("；无解返回 ")#raw("-1")#text("。")
#strong[#text("注意")]#text("：这里是实数意义下的连续覆盖，相邻区间至少要端点相接，不能留下空隙。")





=== #text("现场记忆")

#raw("cur")#text(" 表示当前已经连续覆盖到的最右位置。")

#text("每轮吃掉所有 ")#raw("l <= cur")#text(" 的区间，只保留其中最大的 ")#raw("r")#text("。")

#raw("far <= cur")#text(" 说明无法继续扩展，直接无解。")

#text("每选一个区间后令 ")#raw("cur = far")#text("；")#raw("L == R")#text(" 要单独判断是否有区间包含这个点。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 最小调用示例

```cpp
assert(min_cover(vector<pll>{{}, {1, 3}, {3, 5}}, 1, 5) == 2);
assert(min_cover(vector<pll>{{}, {1, 2}, {3, 5}}, 1, 5) == -1);
```

=== 模板引用

#source_code(("interval_greedy/min_cover.hpp",))


== #text("双指针模板") <algo-interval-greedy-window-longest>

#text("原稿来源：5.5 双指针模板")


#strong[#text("用途")]#text("：维护具有单调性的连续窗口，或在有序序列上让左右指针单向逼近。")
#strong[#text("复杂度")]#text("：只要两个指针都不回退，通常时间 ")#raw("O(n)")#text("；额外空间取决于窗口维护的数据结构。")
#strong[#text("下标")]#text("：")#raw("1-indexed")#text("。")
#strong[#text("限制")]#text("：移动某个指针后必须具有单调性；没有单调性不能硬套双指针。")

=== #text("最长 / 一般合法窗口")


=== #text("最短合法窗口")


=== #text("有序序列左右逼近")


=== #text("现场记忆")

#text("同向窗口：右指针只扩展，左指针只收缩。")

#text("最长合法：")#raw("不合法 -> 缩左端")#text("，恢复合法后更新答案。")

#text("最短合法：")#raw("合法 -> 先更新，再继续缩左端")#text("。")

#text("左右逼近的核心不是写法，而是当前状态能确定只移动哪一侧，并且不会漏答案。")

=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("三个函数均接收 n 和回调，位置 1..n，n=0 不调用回调。window_longest / window_shortest 的 add(pos)、del(pos) 维护窗口，ok() 判断当前合法性，update(l,r) 记录答案；函数无自动清空外部状态，重复调用前自行清空。two_ends(n,too_small,too_large,update) 先按前两者推进端点，找到答案后 update(l,r) 返回 0 结束、正数推进左端点、负数后退右端点，保证每步移动。是否能这样移动须由题意单调性证明。")

=== 最小调用示例

```cpp
vector<int> a{0, 1, 2, 3};
int sum = 0, best = 0;
window_longest(
    3, [&](int p) { sum += a[p]; }, [&](int p) { sum -= a[p]; },
    [&]() { return sum <= 3; },
    [&](int l, int r) { best = max(best, r - l + 1); });
assert(best == 2);
```

=== 模板引用

#source_code(("interval_greedy/window_longest.hpp",))

==== #text("window_shortest")

```cpp
vector<int> a{0, 1, 2, 3};
int sum = 0, best = 4;
window_shortest(
    3, [&](int p) { sum += a[p]; }, [&](int p) { sum -= a[p]; },
    [&]() { return sum >= 3; },
    [&](int l, int r) { best = min(best, r - l + 1); });
assert(best == 1);
```

#source_code(("interval_greedy/window_shortest.hpp",))
==== #text("two_ends")

```cpp
vector<int> a{0, 1, 2, 4};
bool found = false;
two_ends(
    3, [&](int l, int r) { return a[l] + a[r] < 5; },
    [&](int l, int r) { return a[l] + a[r] > 5; },
    [&](int, int)
    {
        found = true;
        return 0;
    });
assert(found);
```

#source_code(("interval_greedy/two_ends.hpp",))
