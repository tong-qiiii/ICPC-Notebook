= 字符串题型 <chapter-string-problems>

本章面向连续子串 / 模式匹配模型，已有源码见 #ref(<chapter-string>, supplement: [字符串章])；不把子序列题混作子串题。字符串位置及 pi / Z / SA 数组沿用原实现 0-index，区间说明若为 [l,r) 则长度 r−l。先核对字符集、空串和重叠计数；字符分隔符必须不出现在原串中。

== 前后缀与周期：border 不等于整除周期 <algo-string-model-periods>

*识别与例题：* 找所有前后缀重合长度、最短重复节或允许末尾截断的周期。#link("https://cses.fi/problemset/task/1733/")[Finding Periods]。自拟例：ababa 的截断周期为 2、4、5，但不能完整重复短串得到它。

*状态与公式：* 调 #ref(<algo-string-kmp>, supplement: [prefix_function])，非空串 border 从 pi[n−1] 开始反复跳 pi[len−1]。最短候选周期 p=n−pi[n−1]；要求完整重复须 n%p=0，否则完整重复节长度 n。允许末尾截断时，用 #ref(<algo-string-z-function>, supplement: [Z]) 检查每个 $1<=p<n$ 是否 z[p]≥n−p，再补 n。

*复杂度与边界：* $O(n)$ 时间空间；空串先特判，不访问 pi[−1]。border 是否包含整串要按题意单独加 n。*记忆：* 截断周期只要求移位相等，完整重复另要长度整除。

== 拼接与重叠匹配：KMP 状态跨片段传递 <algo-string-model-concatenation>

*识别与例题：* 统计模式出现，或两字符串最大首尾重叠后拼接。#link("https://cses.fi/problemset/task/1753/")[String Matching] 允许重叠。自拟例：aaaa 中 aa 出现 3 次；abcab 与 abxyz 最大重叠 ab，最短拼成 abcabxyz。

*状态与调用：* #ref(<algo-string-kmp>, supplement: [kmp_match(text,pat)]) 返回起点；统计出现时匹配完成后 j=pi[j−1]，不能清为 0，否则漏重叠。求 A 的后缀与 B 的前缀重叠，可对 B + 分隔符 + A 求 pi，末项就是长度；若自写状态扫描 A，末字符完成整串匹配时须保留 $j=abs(B)$，仅在还要读取下一字符时才先回退，避免把完全重叠缩短。

*维护与边界：* 扫描总 $O(abs(A)+abs(B))$，pi 空间 $O(abs(B))$；不必真拼接时只保存模式 pi 与 j。处理“扫描完整模式后的回退”时区分是否还会读下一个字符。循环文本匹配可扫描 A+A，原始起点限制在 [0,n)，若模式长度大于 2n 则需按题意扩展。*记忆：* 匹配状态就是当前文本后缀与模式前缀的最长相等长度。

== 回文统计：中心半径与不同回文 <algo-string-model-palindromes>

*识别与例题：* 最长回文、回文出现总数，或不同回文及其频次。#link("https://cses.fi/problemset/task/1111/")[Longest Palindrome]。自拟例：aaa 的回文出现总数为 6，不同回文只有 a、aa、aaa 共 3 种。

*状态与维护：* #ref(<algo-string-manacher>, supplement: [Manacher]) 的奇中心半径 d1[i] 以自身长度 1 起，偶中心半径 d2[i] 可为 0；总出现数为 sum d1 + sum d2，最长长度取 max(2d1−1,2d2)。不同回文用 #ref(<algo-string-palindromic-tree>, supplement: [PAM])，每个真实节点代表一种回文，排除两个根；末尾命中次数按长度降序沿 fail 汇总得到总出现次数。

*复杂度与边界：* 固定字符集下均 $O(n)$ 时间空间。不要把半径总和当不同回文数；PAM 汇总后继续插入须按现有接口约定重置或重算，不能重复累计。总计数用 ll。*记忆：* 中心区分“出现位置”，PAM 节点区分“回文本身”。

== 多模式匹配：AC 的末尾命中与 fail 汇总 <algo-string-model-multi-pattern>

*识别与例题：* 一个文本，很多模式，逐个问是否出现或出现多少次。#link("https://cses.fi/problemset/task/2102/")[Finding Patterns]。自拟例：文本 aaaa，模式 a、aa、aaa 的次数为 4、3、2。

*状态与调用：* #ref(<algo-string-aho-corasick>, supplement: [AC]) 插入模式并保存终点 id，build 补转移；扫描文本到状态 u 时 cnt[u]++。按 BFS 序逆序执行 cnt[fail[u]]+=cnt[u]，模式答案取其终点计数；重复模式共享终点，但输出仍按输入 id。

*复杂度与边界：* S 为 Trie 状态数、σ 为字符集，稠密转移建树 $O(S sigma)$，文本扫描与汇总 $O(n+S)$，空间 $O(S sigma)$；不是每个字符沿 fail 链枚举所有模式。现有模板按小写字母，其他字符集先改映射。*记忆：* 扫描只记落在哪个状态，短后缀模式的贡献最后统一向 fail 祖先传。

== AC 自动机 DP：禁止子串与长串计数 <algo-string-model-automaton-dp>

*识别与例题：* 长度 N 的字符串不能包含若干模式，问方案数。#link("https://atcoder.jp/contests/abc305/tasks/abc305_g")[ABC305 G — Banned Substrings]。自拟例：二元字母 {a,b}、禁 aa、长度 3，合法数为 5。

*状态与转移：* 在 #ref(<algo-string-aho-corasick>, supplement: [AC]) 基础上标 bad[u]=终点或 bad[fail[u]]。dp[i][u] 为合法前缀长 i、当前状态 u 的方案数，dp[0][root]=1；枚举字符转到 v，仅在 !bad[v] 时累加。按位置滚动，最终求所有安全状态之和。

*长 N 与限制：* 把安全状态间单字符转移计数放入矩阵 T，初向量乘 $T^N$；同一对状态被多个字符连到时要累加，不只置 1。普通 DP $O(N S sigma)$ 时间、$O(S)$ 额外空间；矩阵版 $O(S^3 log N)$ 时间、$O(S^2)$ 空间。ABC305 G 的二元禁串长度≤6，可只保留长度≤5 的安全前缀，S≤63；不能将任意大 AC 都稠密矩阵化。*记忆：* 状态只记对未来匹配有影响的末尾，已经命中禁串就永不允许进入。

== SAM：不同子串与出现次数 <algo-string-model-sam-substrings>

*识别与例题：* 统计不同子串、按出现次数分组或求重复子串指标。#link("https://cses.fi/problemset/task/2105/")[Distinct Substrings]。自拟例：aba 的不同子串为 a、b、ab、ba、aba 共 5 种，a 出现 2 次。

*状态与公式：* #ref(<algo-string-suffix-automaton>, supplement: [SAM]) 的每个非根状态 v 代表长度区间 (len[link[v]],len[v]]，这些子串具有相同 endpos 集。不同子串数累加 len[v]−len[link[v]]；普通扩展节点的初始出现数为 1，clone 为 0，按 len 降序沿 link 汇总。出现至少 k 次的最长子串取 occ[v]≥k 的最大 len[v]。

*调用与边界：* 先完整构建，再调用 calc_occ；现有实现每次从 cnt 重建 occ，可重复调用，手写传播时也须先恢复初值，clone 不能误置 1。固定字符集 $O(n)$ 时间空间，总子串数 ll。若求“出现数恰为 k 的不同子串数量”，需累加对应状态的长度差，而非状态数。*记忆：* SAM 节点不是一个子串，而是一段长度、同一组结束位置。

== SA / LCP：子串比较与两串公共子串 <algo-string-model-suffix-lcp>

*识别与自拟例：* 比较任意两子串，或求两字符串最长公共子串。banana 中起点 1 的 anana 与起点 3 的 ana，LCP 长度 3；xab 与 zab 的最长公共子串为 ab，长度 2。

*状态与调用：* #ref(<algo-string-lcp-rmq>, supplement: [LCPRMQ]) query(i,j) 得后缀 LCP，子串长度 a、b 时取 h=min(LCP,a,b)。若 h=min(a,b)，短者字典序小或两者相等；否则比较 s[i+h] 与 s[j+h]。两串拼接 A + 唯一分隔符 + B，SA 中相邻且属于不同原串的后缀，其 LCP 最大值就是最长公共子串，排除分隔符后缀。

*复杂度与边界：* 当前 #ref(<algo-string-suffix-array>, supplement: [倍增 SA]) 和 RMQ 建树 $O(n log n)$ 时间空间，LCP 查询 $O(1)$；子串范围先检查，不读取端点外字符。原串可能含全部字节时改用整数编码保留独立哨兵，不能默认某字符永不出现。*记忆：* 后缀间的 LCP 可复用，但子串比较必须先截到各自长度。

== 循环串：最小表示与旋转等价 <algo-string-model-rotation>

*识别与例题：* 环形序列不指定起点，需要比较是否旋转相等或取规范表示。#link("https://cses.fi/problemset/task/1110/")[Minimal Rotation]。自拟例：baba 与 abab 同环，最小表示均为 abab；反转后的等价是另一种题意。

*转化与调用：* 调 #ref(<algo-string-minimum-rotation>, supplement: [最小表示法]) 得最优起点 p，拼接 s[p…n) 与 s[0…p)。两等长串各转为该规范形式即可精确比较；或用 KMP 检查 B 是否在 A+A 的前 n 个起点出现。允许翻转时另比较反串的规范形式，不允许时不能自行合并镜像。

*复杂度与边界：* $O(n)$ 时间，输出规范串需 $O(n)$ 空间；空串单独处理，不对 n=0 取模。周期串可能有多个最优起点，比较内容即可；要求最小起点时按接口约定处理平局。*记忆：* 先明确等价关系是旋转，还是旋转加翻转，再做唯一表示。
