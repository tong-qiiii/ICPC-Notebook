= 图论建模 <chapter-graph-problems>

本章回答“题目怎样变成图”，已有实现见 #ref(<chapter-graph>, supplement: [图论章])。节点默认 1-index；布尔变量与图节点编号不同，使用模板接口转换。自拟例只用于解释建模。先确定有向 / 无向、边权符号、可重复访问与约束方向，再选算法；改动模型后重新检查复杂度与可行性。

== 分层图：有限资源的最短路 <algo-graph-model-layered>

*识别与例题：* 路径允许使用至多 K 次特殊操作，历史只需记已用次数。#link("https://cses.fi/problemset/task/1195/")[Flight Discount]：一条边可享向下取整半价。自拟例：1→2 费用 8、2→3 费用 2，一张券最优费用 6。

*状态与建边：* 设 (u,k) 为到 u 且已用 k 次资源。普通边 (u,k)→(v,k) 费用 w；可用特殊操作时连到 (v,k+1)，费用按题意如 floor(w/2)。从 (s,0) 开始；至多 K 次取目标所有层最小，恰好 K 次只取第 K 层。

*调用与边界：* 编号可用 k×n+u，接 #ref(<algo-graph-dijkstra>, supplement: [Dijkstra])；边仅 0/1 时用 #ref(<algo-graph-zero-one-bfs>, supplement: [0-1 BFS])。显式图有 $O(n K)$ 点、$O(m K)$ 边，Dijkstra 时间 $O((n+m)(K+1)log(n(K+1)))$。负边不能直接套 Dijkstra；不能给同一原点只保留一个 dis。*记忆：* 资源次数是状态，不是到点后再贪心决定。

== 差分约束：不等式方向与可行解 <algo-graph-model-difference>

*识别与自拟例：* 开始时间、名次差、距离限制均可写成两变量之差。要求 x₂≥x₁+3 且 x₂≤x₁+5，可取 x₁=0、x₂=3；再加 x₂≤x₁+2 就矛盾。

*建边：* 统一写 $x_v<=x_u+w$，连 u→v 权 w。大于等于先反向取负；等式拆成两条；整数严格小于 $x_v<x_u+w$ 可改成≤w−1，实数不能减 1。所有点初始距离 0 等价于超级源连零边，可检查任意连通块的矛盾。

*调用与边界：* #ref(<algo-graph-difference-constraints>, supplement: [DiffCons]) leq(u,v,w)，solve 返回可行性与一组势能。负环即无解；无负环的最短距离是一组满足所有≤约束的解。最坏 $O(n m)$ 时间、$O(n+m)$ 空间；若要求最大化某变量还需锚点、可达性与有界性分析，不把任意可行势能当优化答案。*记忆：* 每条边表示“终点不超过起点加边权”。

== SCC 缩点：环内收集与 DAG DP <algo-graph-model-condensation>

*识别与例题：* 有向图可反复走，每个点的非负收益只收一次，最大化沿路所得。#link("https://cses.fi/problemset/task/1686")[Coin Collector]。自拟例：1↔2 收益 4、5，2→3 收益 2，最优 11。

*转化与状态：* #ref(<algo-graph-scc>, supplement: [SCC]) 求 bel，分量收益 W 为内部点权和；跨分量边构成 DAG。任意起点时 f[c]=W[c]，按拓扑序更新 $f_v=max(f_v,f_u+W_v)$；固定起点则仅起点分量可达，其余负无穷。答案按终点限制取 max。

*复杂度与边界：* SCC、建 DAG、拓扑 DP 均 $O(n+m)$，空间 $O(n+m)$。分量编号不保证正好是所需拓扑顺序；重边可保留但入度也要按保留条数算。负点权、必须简单路径或禁止反复访问时，不能无条件把环内收益全相加。*记忆：* 强连通保证能收完再到任意出口，单向 DAG 决定后续选择。

== 2-SAT：二选一限制的子句化 <algo-graph-model-two-sat>

*识别与例题：* 每个变量仅真假两种选择，每条约束可写成两个文字的或。#link("https://cses.fi/problemset/task/1684/")[Giant Pizza]。自拟例：“x、y 至少一个真”与“不能同时真”合起来就是恰一个真。

*建边与调用：* 对 $(x=a) or (y=b)$ 用 #ref(<algo-graph-two-sat>, supplement: [TwoSAT]) 的 either(x,a,y,b)，内部连两个逆否蕴含。禁止同时 x=a、y=b 则 either(x,!a,y,!b)；蕴含 x=a→y=b 则 either(x,!a,y,b)。强制值用 force，同值用 eq、异值用 neq；satisfiable 判断，assignment 读取解。

*复杂度与边界：* 变量 n、子句 m，$O(n+m)$ 时间空间。三变量“至少一个真”一般不能直接拆成三条两两或，那会加强条件；只加单向蕴含而不加逆否也易错。SCC 中变量与其反面同块即无解，解的取向按现有实现，不硬背分量号比较。*记忆：* 先写出禁止的布尔组合，再把它取反成二元或。

== 匹配与最小点覆盖：配对、覆盖、独立集 <algo-graph-model-matching-cover>

*识别与例题：* 两类对象一一配对、每个最多使用一次。#link("https://cses.fi/problemset/task/1696/")[School Dance]。自拟例：L₁ 可配 R₁/R₂，L₂ 仅可配 R₁，最大配对 2；贪心先占 R₁ 可能卡住 L₂。

*建模与维护：* 两侧分别编号，把可配关系加边，调用 #ref(<algo-graph-hopcroft-karp>, supplement: [HopcroftKarp]) work 求最大匹配。要选最少顶点覆盖所有边，最大匹配后由未匹配左点出发，沿“非匹配左→右、匹配右→左”走交替路；最小点覆盖为左侧未访问点与右侧已访问点的并集，其补集为最大独立集。

*调用与限制：* 模板 cover() 已实现覆盖集合；$O(m sqrt(n_L+n_R))$ 时间、$O(n_L+n_R+m)$ 空间。必须先确认二分图；一般图不能套 König 等式。每点可用多次改容量网络，带权一一分配改 Hungarian / 费用流。*记忆：* 匹配选择不冲突的边，覆盖选择能挡住每条边的点，两者对象不同。

== 最小割二选一：一元代价与不同标签惩罚 <algo-graph-model-binary-cut>

*识别与自拟例：* 每点选 0/1，代价为一元费用与若干 $w_[i j][x_i!=x_j]$，w≥0。两点费用分别 (0,4)、(4,0)，不同标签罚 3，则选 (0,1) 总费用 3，优于同标签的 4。

*建图：* 约定 S 侧是 0，T 侧是 1。S→i 容量 cost1[i]，i→T 容量 cost0[i]；每对不同标签罚 w 加 i→j、j→i 各容量 w，割分开时恰切一条。一元费用为负时先给每点两选项减去同一个最小值，常数计回答案。强制 0 加 S→i 的 INF，强制 1 加 i→T 的 INF。

*调用与边界：* #ref(<algo-graph-dinic>, supplement: [Dinic]) 求最大流即最小割，再从 S 走正残量边恢复标签。INF 取大于某个已知可行有限割的容量，累加检查溢出。一般 Dinic 最坏 $O(V^2 E)$、空间 $O(V+E)$；负的“不同惩罚”不能直接当容量，普通二元代价须另验次模条件。*记忆：* 用割边逐项核对每种选择的真实费用，不靠猜源边方向。

== 最大权闭合子图：项目收益与前置依赖 <algo-graph-model-closure>

*识别与自拟例：* 选项目得收益 / 付成本，选 u 必须同时选 v。项目 1 收益 8、依赖成本 5 的项目 2，则一起选净收益 3；不允许只取 8。

*建图与证明：* 正收益 w 连 S→u 容量 w，负收益连 u→T 容量 −w，依赖 u→v 连 INF。S 侧表示选择；有限割不能让 u 选而 v 不选。令 P 为正收益总和，答案 P−最小割；所有不选的正收益、被选的成本正好由割容量扣除。

*调用与限制：* #ref(<algo-graph-max-weight-closure>, supplement: [MaxWeightClosure]) 设置 w 与依赖后 work 返回最大收益和选中集合。依赖可成环，含义是成组同选；若还要求恰选 k 个，普通闭合模型不够。图有 V 个点、E 条边时，Dinic 一般最坏 $O(V^2 E)$ 时间、$O(V+E)$ 空间；INF 与 P 必须在安全整数范围。*记忆：* 依赖边从“被选择就触发条件”的项目指向它的前置项。

== 上下界流与费用流：先守恒、再谈最优 <algo-graph-model-bounded-cost-flow>

*识别与自拟例：* 每条边至少 / 至多运多少，或工人到任务的分配有费用。循环边 1→2 下上界 [1,2]、2→1 [0,2] 可取流量 (1,1)。两工人两任务费用矩阵 [[1,4],[3,2]]，一一分配最小费用 3。

*上下界可行流：* 先放每边下界 low，残量容量 high−low；bal[u]−=low、bal[v]+=low。正余额连 SS→u，负余额连 u→TT，超级源需求全部满流才可行，原边流量=low+残量用量。指定 s、t 的可行流先加 t→s 的 INF 边；求最大流不能简单保留所有超级边继续跑，引用 #ref(<algo-graph-lower-bound-flow>, supplement: [可行流])、#ref(<algo-graph-lower-bound-max-flow>, supplement: [上下界最大流]) 的完整流程。

*费用流：* 分配网络为 S→工人→任务→T，各容量 1，工人任务边费用为代价；发送指定流量 F，实际流不足即不可行。#ref(<algo-graph-min-cost-flow-potential>, supplement: [势能费用流]) 的整数容量增广次数≤F；常见界为初始求势能 $O(V E)$ 加 $O(F E log V)$，空间 $O(V+E)$。负边需合法初始势能，任意负环循环流不能直接当普通 s–t 模板处理。

*记忆：* 有上下界先补平每点余额；要费用最优还需计入下界费用并维护残量最优性，不把任意满需求流当成最小费用解。
