= 字符串 <chapter-string>

#text("完整保留 raw/09_string.md 的 13 个原始条目及其重要变体，按原顺序排列。原稿附项按当前位置参与算法自动编号。")

#text("统一约定：输入规模、节点号、版本号和查询区间须合法；空区间仅在接口明确支持时使用。递归实现需评估栈空间；整数中间值及累计值必须在对应类型范围内。多测重新调用 init / build / clear，普通函数的外部维护状态由调用者清空。各模板独立选用，同名 Info / Tag / Seg / Query 不应同时粘贴。")

#import "../styles/source.typ": source_code


=== 公共代码与复制顺序

本章源码块仅展示算法自身实现；公共骨架只打印一次。按依赖链接复制所需的公共代码，再复制本算法与调用示例，比赛时不需要仓库头文件。已有基础算法被复用时，直接复制其原条目，不在每个代码块里重贴。

*依赖索引：* #ref(<algo-string-suffix-array>, supplement: none)。

== #text("KMP / Prefix Function") <algo-string-kmp>

#text("原稿来源：9.1 KMP / Prefix Function")


#strong[#text("用途")]#text("：计算前缀函数；在线性时间内查找模式串在文本中的所有出现位置。")
#strong[#text("复杂度")]#text("：前缀函数 ")#raw("O(m)")#text("；匹配 ")#raw("O(n + m)")#text("；空间 ")#raw("O(m)")
#strong[#text("下标")]#text("：")#raw("0-indexed")


=== #text("现场记忆")

#raw("pi[i]")#text(" = ")#raw("s[0..i]")#text(" 的最长相等真前后缀长度。")

#text("失配：")
#raw("j = pi[j - 1]")

#text("匹配完整模式串后仍要：")
#raw("j = pi[j - 1]")

#text("这样才能继续寻找重叠匹配。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("文本、模式和结果起点均 0-indexed；支持任意字节。原约定空模式返回空匹配列表，不按每个间隙算一次匹配。")

=== 最小调用示例

```cpp
assert((kmp_match("ababa", "aba") == vector<int>{0, 2}));
assert(kmp_match("abc", "").empty());
```

=== 模板引用

#source_code(("string/kmp.hpp",))


== #text("Z 函数") <algo-string-z-function>

#text("原稿来源：9.2 Z 函数")


#strong[#text("用途")]#text("：求每个后缀与整串前缀的 LCP；可用于字符串匹配、周期等问题。")
#strong[#text("复杂度")]#text("：")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：")#raw("0-indexed")
#strong[#text("注意")]#text("：约定 ")#raw("z[0] = n")#text("。")


=== #text("现场记忆")

#text("维护最右 Z-box ")#raw("[l, r]")#text("。")

#text("若 ")#raw("i <= r")#text("：")
#raw("z[i] = min(r - i + 1, z[i - l])")

#text("然后继续暴力扩展。")

#text("新区间更靠右时更新 ")#raw("[l, r]")#text("。")


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("支持任意字节，通过整数 256 分隔模式和文本，避免分隔字符碰撞。z[0]=n；空模式返回空匹配列表。")

=== 最小调用示例

```cpp
assert((z_function(string("aaaa")) == vector<int>{4, 3, 2, 1}));
```

=== 模板引用

#source_code(("string/z_function.hpp",))


== #text("Manacher：回文半径") <algo-string-manacher>

#text("原稿来源：9.3 Manacher：回文半径")


#strong[#text("用途")]#text("：线性求所有奇偶回文半径，可查询最长回文和指定区间是否回文。")
#strong[#text("复杂度")]#text("：预处理 ")#raw("O(n)")#text("；最长回文 ")#raw("O(n)")#text("；单次区间判断 ")#raw("O(1)")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("，区间为闭区间 ")#raw("[l, r]")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("build(s)")],[#text("预处理回文半径")],
[#raw("longest()")],[#text("最长回文子串长度")],
[#raw("is_pal(l,r)")],[#text("判断闭区间 ")#raw("[l,r]")#text(" 是否回文")],
[#raw("d1[i]")],[#text("以 ")#raw("i")#text(" 为中心的奇回文半径")],
[#raw("d2[i]")],[#text("以 ")#raw("i-1,i")#text(" 中间为中心的偶回文半径")],
)])



=== #text("现场记忆")

#raw("d1[i]")#text("：中心是 ")#raw("i")#text("，半径包含中心，长度 ")#raw("2*d1[i]-1")#text("。")

#raw("d2[i]")#text("：中心在 ")#raw("i-1")#text(" 和 ")#raw("i")#text(" 之间，长度 ")#raw("2*d2[i]")#text("。")

#text("偶回文 ")#raw("[l,r]")#text(" 的中心右侧位置：")
#raw("(l+r)/2 + 1")

#text("Manacher 维护的 ")#raw("[l,r]")#text(" 是当前最右回文闭区间。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Manacher m("abacaba");
assert(m.longest() == 7 && m.is_pal(1, 5));
```

=== 模板引用

#source_code(("string/manacher.hpp",))


== #text("字符串哈希：双模数") <algo-string-hash-double>

#text("原稿来源：9.4 字符串哈希：双模数")


#strong[#text("用途")]#text("：预处理字符串后，")#raw("O(1)")#text(" 获取子串哈希、比较两个子串或拼接哈希。")
#strong[#text("复杂度")]#text("：预处理 ")#raw("O(n)")#text("；")#raw("get / equal / merge")#text(" 均为 ")#raw("O(1)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：")#raw("0-indexed")#text("，")#raw("get(l,r)")#text(" 使用闭区间 ")#raw("[l,r]")
#strong[#text("注意")]#text("：哈希存在极低概率冲突；")#raw("merge()")#text(" 的 ")#raw("right_len")#text(" 不能超过已预处理的幂次数。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("build(s)")],[#text("预处理字符串")],
[#raw("get(l,r)")],[#text("闭区间 ")#raw("[l,r]")#text(" 的双哈希")],
[#raw("equal(...)")],[#text("比较两个子串")],
[#raw("merge(a,b,len)")],[#text("将哈希 ")#raw("a")#text(" 与长度为 ")#raw("len")#text(" 的 ")#raw("b")#text(" 拼接")],
)])



=== #text("现场记忆")

#text("前缀哈希：")
#raw("h[i+1] = h[i] * BASE + s[i]")

#text("子串 ")#raw("[l,r]")#text("：")
#raw("h[r+1] - h[l] * BASE^(r-l+1)")

#text("拼接 ")#raw("A + B")#text("：")
#raw("hash(A) * BASE^|B| + hash(B)")

#text("比较子串时先保证长度相同。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Hash2 h("ababa");
assert(h.equal(0, 2, 2, 4));
assert(h.merge(h.get(0, 1), h.get(2, 4), 3) == h.get(0, 4));
```

=== 模板引用

#source_code(("string/hash_double.hpp",))


== #text("Trie：字符串插入 / 前缀计数") <algo-string-trie>

#text("原稿来源：9.5 Trie：字符串插入 / 前缀计数")

#strong[#text("下标")]#text("：字符位置 / 返回容器为 0-indexed；自动机节点从 0 开始。")


#strong[#text("用途")]#text("：维护小写字符串集合，支持插入、统计完整字符串出现次数和指定前缀数量。")
#strong[#text("复杂度")]#text("：单次插入 / 查询 ")#raw("O(|s|)")#text("，空间 ")#raw("O(字符总数 × 字符集)")#text(" 的节点存储。")
#strong[#text("字符集")]#text("：")#raw("a ~ z")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口")],[#text("含义")]),
[#raw("init()")],[#text("清空 Trie")],
[#raw("insert(s)")],[#text("插入字符串")],
[#raw("count_word(s)")],[#raw("s")#text(" 完整出现次数")],
[#raw("count_prefix(s)")],[#text("以 ")#raw("s")#text(" 为前缀的字符串数量")],
)])



=== #text("现场记忆")

#text("• ")#raw("pass[u]")#text("：经过节点 ")#raw("u")#text(" 的字符串数量。")
#text("• ")#raw("end[u]")#text("：恰好在节点 ")#raw("u")#text(" 结束的字符串数量。")
#text("• 本模板默认字符均为 ")#raw("a ~ z")#text("。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Trie t;
t.insert("app");
t.insert("apple");
t.insert("");
assert(t.count_word("app") == 1 && t.count_prefix("app") == 2 &&
       t.count_prefix("") == 3);
```

=== 模板引用

#source_code(("string/trie.hpp",))


== #text("01 Trie：最大异或 / 最小异或") <algo-string-trie01>

#text("原稿来源：9.6 01 Trie：最大异或 / 最小异或")

#strong[#text("下标")]#text("：字符位置 / 返回容器为 0-indexed；自动机节点从 0 开始。")


#strong[#text("用途")]#text("：维护非负整数集合，支持插入、删除，以及查询与 ")#raw("x")#text(" 的最大 / 最小异或值。")
#strong[#text("复杂度")]#text("：单次插入 / 删除 / 查询 ")#raw("O(LOG)")#text("，默认 ")#raw("LOG = 30")#text("。")
#strong[#text("注意")]#text("：查询时集合必须非空；")#raw("erase(x)")#text(" 前必须保证 ")#raw("x")#text(" 已存在。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init(log)")],[#text("清空并设置最高二进制位")],
[#raw("insert(x)")],[#text("插入 ")#raw("x")],
[#raw("erase(x)")],[#text("删除一个 ")#raw("x")],
[#raw("max_xor(x)")],[#text("返回集合中 ")#raw("y ^ x")#text(" 的最大值")],
[#raw("min_xor(x)")],[#text("返回集合中 ")#raw("y ^ x")#text(" 的最小值")],
[#raw("size()")],[#text("当前元素数量")],
)])



=== #text("现场记忆")

#text("• 求最大异或：每一位优先走 ")#raw("b ^ 1")#text("。")
#text("• 求最小异或：每一位优先走 ")#raw("b")#text("。")
#text("• ")#raw("cnt > 0")#text(" 才表示该分支当前仍有元素。")
#text("• 默认 ")#raw("LOG = 30")#text("，适用于非负 ")#raw("int")#text("。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 最小调用示例

```cpp
Trie01 t;
t.insert(5);
t.insert(10);
assert(t.max_xor(7) == 13 && t.min_xor(7) == 2);
t.erase(5);
```

=== 模板引用

#source_code(("string/trie01.hpp",))


== #text("AC 自动机：多模式匹配") <algo-string-aho-corasick>

#text("原稿来源：9.7 AC 自动机：多模式匹配")

#strong[#text("下标")]#text("：字符位置 / 返回容器为 0-indexed；自动机节点从 0 开始。")


#strong[#text("用途")]#text("：同时匹配多个小写模式串，并统计每个模式串在文本中的出现次数。")
#strong[#text("复杂度")]#text("：插入 ")#raw("O(模式串总长)")#text("，建机 ")#raw("O(节点数 × 26)")#text("，匹配 ")#raw("O(|text| + 节点数 + 模式数)")#text("。")
#strong[#text("字符集")]#text("：")#raw("a ~ z")
#strong[#text("注意")]#text("：所有模式串必须在 ")#raw("build()")#text(" 前插入；允许重复模式串和重叠匹配。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init()")],[#text("清空自动机")],
[#raw("insert(s)")],[#text("插入模式串，返回模式编号")],
[#raw("build()")],[#text("建立 fail 指针并补全转移")],
[#raw("count(text)")],[#text("返回每个模式串的出现次数")],
)])



=== #text("现场记忆")

#text("• ")#raw("fail[u]")#text(" 指向 ")#raw("u")#text(" 的最长真后缀状态。")
#text("• 建机时不存在的转移直接继承 ")#raw("fail")#text(" 的转移。")
#text("• 扫文本只统计“到达每个状态多少次”。")
#text("• 最后按 BFS 逆序：")#raw("cnt[fail[u]] += cnt[u]")#text("。")
#text("• 模式串答案就是其终止节点的 ")#raw("cnt")#text("。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("模式编号 0-indexed，模式串要求非空且只含 a..z；空模式现在明确断言拒绝。文本中其他字符使状态回到根，按分隔符处理。build 可重复调用但之后不能继续 insert；需 init 重建。count 多次调用互不累积。")

=== 最小调用示例

```cpp
AC a;
int x = a.insert("he"), y = a.insert("she");
a.build();
auto c = a.count("sheshe");
assert(c[x] == 2 && c[y] == 2);
```

=== 模板引用

#source_code(("string/aho_corasick.hpp",))


== #text("回文自动机 PAM") <algo-string-palindromic-tree>

#text("原稿来源：9.8 回文自动机 PAM")


#strong[#text("用途")]#text("：维护所有本质不同回文子串；可查询当前最长回文后缀、回文后缀数量及每个回文串出现次数。")
#strong[#text("复杂度")]#text("：建树 ")#raw("O(n)")#text("；")#raw("calc_occ()")#text(" ")#raw("O(n)")#text("；空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：节点编号 ")#raw("0-indexed")#text("，")#raw("0")#text(" 为偶根，")#raw("1")#text(" 为奇根")
#strong[#text("限制")]#text("：字符集为 ")#raw("'a' ~ 'z'")#text("，节点数最多 ")#raw("n + 2")
#strong[#text("注意")]#text("：")#raw("tr[u].occ")#text(" 需在 ")#raw("calc_occ()")#text(" 后读取；继续插入字符后应重新调用。")



#text("逐字符维护：")
#raw("PAM pam(s.size());\nfor (char c : s)\n{\n    int u = pam.insert(c);\n    cout << pam.tr[u].len << ' ' << pam.suffix_count() << endl;\n}",lang:"cpp",block:true)

=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init()")],[#text("清空并初始化")],
[#raw("insert(c)")],[#text("插入字符，返回当前最长回文后缀节点")],
[#raw("build(s)")],[#text("清空后插入整个字符串")],
[#raw("calc_occ()")],[#text("汇总所有回文串出现次数")],
[#raw("distinct()")],[#text("本质不同回文串数量")],
[#raw("longest_suffix()")],[#text("当前前缀最长回文后缀长度")],
[#raw("suffix_count()")],[#text("当前前缀的回文后缀数量")],
[#raw("last")],[#text("当前最长回文后缀节点")],
[#raw("tr[u].len")],[#text("节点 ")#raw("u")#text(" 对应回文串长度")],
[#raw("tr[u].fail")],[#text("最长真回文后缀")],
[#raw("tr[u].occ")],[#text("回文串总出现次数")],
)])



=== #text("现场记忆")

#text("两个根：")

#raw("0: len = 0")#text("，偶根")
#raw("1: len = -1")#text("，奇根")

#text("插入字符 ")#raw("c")#text("：")
#text("沿 ")#raw("fail")#text(" 找满足两端能加 ")#raw("c")#text(" 的最长回文。")

#text("新节点：")
#raw("len = tr[cur].len + 2")

#raw("fail")#text(" = 新回文的最长真回文后缀。")

#raw("tr[u].suf_cnt = tr[fail[u]].suf_cnt + 1")

#text("出现次数最后沿 ")#raw("fail")#text(" 从长回文向短回文累加。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("只能插入 a..z，s 内部有 -1 哨兵；build / init 清空。calc_occ 每次从 cnt 重算，允许统计后继续 insert 再统计，空串 distinct=0。节点数组按创建序 0-indexed，两个根不表示非空回文。")

=== 最小调用示例

```cpp
PAM p;
p.build("ababa");
p.calc_occ();
assert(p.distinct() == 5 && p.longest_suffix() == 5 &&
       p.suffix_count() == 3);
```

=== 模板引用

#source_code(("string/palindromic_tree.hpp",))


== #text("后缀自动机 SAM") <algo-string-suffix-automaton>

#text("原稿来源：9.9 后缀自动机 SAM")

#strong[#text("下标")]#text("：字符位置 / 返回容器为 0-indexed；自动机节点从 0 开始。")


#strong[#text("用途")]#text("：维护一个字符串的所有子串；可查询不同子串数量、模式串出现次数、是否为子串及与另一串的最长公共子串。")
#strong[#text("复杂度")]#text("：建树 ")#raw("O(n)")#text("；")#raw("calc_occ()")#text(" ")#raw("O(n)")#text("；")#raw("contains / count / lcs_substr")#text(" 均为 ")#raw("O(|t|)")
#strong[#text("空间")]#text("：")#raw("O(n)")#text("，状态数最多 ")#raw("2n - 1")
#strong[#text("限制")]#text("：字符集为 ")#raw("'a' ~ 'z'")
#strong[#text("注意")]#text("：")#raw("count(t)")#text(" 前需调用 ")#raw("calc_occ()")#text("；之后若继续 ")#raw("extend()")#text("，需重新调用。")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("接口 / 变量")],[#text("含义")]),
[#raw("init()")],[#text("清空并初始化")],
[#raw("extend(c)")],[#text("在末尾加入字符")],
[#raw("build(s)")],[#text("清空后建立 SAM")],
[#raw("calc_occ()")],[#text("汇总每个状态的出现次数")],
[#raw("distinct()")],[#text("本质不同子串数量")],
[#raw("contains(t)")],[#raw("t")#text(" 是否为原串子串")],
[#raw("count(t)")],[#raw("t")#text(" 的出现次数")],
[#raw("lcs_substr(t)")],[#text("与 ")#raw("t")#text(" 的最长公共子串长度")],
[#raw("st[u].len")],[#text("状态最大长度")],
[#raw("st[u].link")],[#text("suffix link")],
[#raw("st[u].occ")],[#text("状态对应子串出现次数")],
)])



=== #text("现场记忆")

#text("普通扩展：")
#text("一路补 ")#raw("nxt[c]")#text("，直到遇到已有转移。")

#text("若 ")#raw("len[p] + 1 == len[q]")#text("：")
#text("直接 ")#raw("link[cur] = q")#text("。")

#text("否则 clone：")
#text("复制 ")#raw("q")#text("，令 ")#raw("len[clone] = len[p] + 1")#text("，")
#text("再把指向 ")#raw("q")#text(" 的对应转移改到 clone。")

#text("不同子串数：")
#raw("Σ(len[u] - len[link[u]])")

#text("出现次数按 ")#raw("len")#text(" 从大到小沿 ")#raw("link")#text(" 累加；clone 初始贡献为 ")#raw("0")#text("。")


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("只支持 a..z；build / init 清空，calc_occ 可重复且支持继续 extend 后重算。count(空串) 明确定义为 n+1（包括所有间隙），非空匹配沿用原算法。contains(空串)=true。")

=== 最小调用示例

```cpp
SAM s;
s.build("ababa");
s.calc_occ();
assert(s.distinct() == 9 && s.count("aba") == 2 &&
       s.lcs_substr("cabab") == 4);
```

=== 模板引用

#source_code(("string/suffix_automaton.hpp",))


== #text("后缀数组 SA：O(n log n)") <algo-string-suffix-array>

#text("原稿来源：9.10 后缀数组 SA：O(n log n)")


#strong[#text("用途")]#text("：求字符串所有后缀的字典序排名。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n log n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：字符串、")#raw("sa")#text(" 均 ")#raw("0-indexed")





=== #text("现场记忆")

#raw("倍增：\n比较后缀前 2k 个字符\n= 两个长度 k 的 rank。\n\n先按第二关键字排，再按第一关键字计数排序。\n\nsa[i]：排名 i 的后缀起点。\nrank[sa[i]] = i。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("字节按 unsigned char 0..255 排序，包括 NUL。空串返回空数组；sa / rk 0-indexed。输入规模还须保证倍增、2n 和数组长度可由 int 表示。")

=== 最小调用示例

```cpp
assert((buildSA("banana") == vector<int>{5, 3, 1, 0, 4, 2}));
```

=== 模板引用

#source_code(("string/suffix_array.hpp",))


== #text("LCP / Height + RMQ") <algo-string-lcp-rmq>

#text("原稿来源：9.11 LCP / Height + RMQ")


#strong[#text("用途")]#text("：预处理任意两个后缀的最长公共前缀 ")#raw("LCP(i,j)")#text("。")
#strong[#text("复杂度")]#text("：构造 SA ")#raw("O(n log n)")#text("，Height ")#raw("O(n)")#text("，RMQ ")#raw("O(n log n)")#text("，单次查询 ")#raw("O(1)")
#strong[#text("下标")]#text("：字符串位置 ")#raw("0 ~ n-1")



=== #text("接口速查")

#figure(align(center)[#table(columns:2,inset:6pt,
table.header([#text("变量 / 接口")],[#text("含义")]),
[#raw("sa[k]")],[#text("第 ")#raw("k")#text(" 小后缀起点")],
[#raw("rk[i]")],[#text("后缀 ")#raw("i")#text(" 的排名")],
[#raw("height[k]")],[#raw("LCP(sa[k-1], sa[k])")],
[#raw("query(i,j)")],[#text("后缀 ")#raw("i,j")#text(" 的 LCP")],
)])



=== #text("现场记忆")

#raw("height[i] = LCP(sa[i-1], sa[i])\n\n任意两个后缀排名 l < r：\nLCP = min(height[l+1 ... r])\n\nKasai：\n当前后缀 LCP 为 h，\n下一个后缀至少从 h-1 开始匹配。",block:true)


=== 组织方式与维护约定

#text("struct：保持相关状态与多次查询接口。")

=== 接口、下标与适用边界补充

#text("query(i,j) 只接受有效非空后缀起点 0..n-1。空串允许构造，不能查询。Kasai 遇到字典序第一后缀明确清零 h；与后缀数组共享维护实现，PDF 中展开完整依赖。")

=== 最小调用示例

```cpp
LCPRMQ l("banana");
assert(l.query(1, 3) == 3 && l.query(0, 0) == 6);
```

=== 模板引用

*代码依赖：* 先复制 #ref(<algo-string-suffix-array>, supplement: none)；各依赖只复制一次，下方仅展示本节实现。

#source_code(("string/lcp_rmq.hpp",))


== #text("Duval：Lyndon 分解") <algo-string-duval>

#text("原稿来源：9.12 Duval：Lyndon 分解")


#strong[#text("用途")]#text("：将字符串唯一分解为字典序非增的 Lyndon 串序列。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(n)")
#strong[#text("下标")]#text("：返回区间 ")#raw("[l,r)")#text("，")#raw("0-indexed")





=== #text("现场记忆")

#raw("维护 i, j, k。\n\ns[k] < s[j] -> k = i\ns[k] == s[j] -> ++k\ns[k] > s[j] -> 当前 Lyndon 块确定\n\n块长 = j-k\n不断从 i 开始切长度 j-k。",block:true)


=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("区间 0-indexed、左闭右开。原实现按 char 比较，若要跨平台字节序一致，输入限制 ASCII 0..127；非 ASCII 字节需人工确认字符序。空串返回空分解。")

=== 最小调用示例

```cpp
auto d = duval("ababb");
assert((d == vector<pair<int, int>>{{0, 5}}));
```

=== 模板引用

#source_code(("string/duval.hpp",))


== #text("字符串最小表示法") <algo-string-minimum-rotation>

#text("原稿来源：9.13 字符串最小表示法")


#strong[#text("用途")]#text("：求循环字符串所有旋转中字典序最小的旋转起点。")
#strong[#text("复杂度")]#text("：时间 ")#raw("O(n)")#text("，空间 ")#raw("O(1)")
#strong[#text("下标")]#text("：返回起点 ")#raw("0-indexed")





=== #text("现场记忆")

#raw("比较旋转 i 和 j。\n\n前 k 个字符相同，第 k 个首次不同：\n\ni 更大 -> i 到 i+k 全不可能是答案\nj 更大 -> j 到 j+k 全不可能是答案\n\n直接跳 k+1。",block:true)

=== 组织方式与维护约定

#text("普通函数 / 函数模板：单次求解或回调骨架，不增加工程封装。")

=== 接口、下标与适用边界补充

#text("返回 0-indexed 起点，空串返回 0。按原 char 比较，跨平台一致性限定 ASCII 0..127；多解允许返回任一最小旋转的起点。")

=== 最小调用示例

```cpp
assert(minRepresentation("baca") == 3 && minRepresentation("") == 0);
```

=== 模板引用

#source_code(("string/minimum_rotation.hpp",))

