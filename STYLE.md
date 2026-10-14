# ICPC C++ Code Style

本文档规定 ICPC Notebook 中所有 C++ 模板的代码风格。

目标：

代码应该像本人在 ICPC/CCPC 赛场中写出的代码：

- 简洁
- 稳定
- 易修改
- 易调试
- 易复制

不要追求工程化，而追求比赛现场快速正确完成。


---

# 1. 基础模板

默认使用个人基础模板。

```cpp
#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define ll long long
#define pll pair<long long, long long>
#define x first
#define y second
#define db double

#define debug(x) cout << " x:" << x << endl;

mt19937_64 rg(
    chrono::steady_clock::now()
    .time_since_epoch()
    .count()
);
```

不要擅自替换成其他竞赛模板。

---

# 2. 花括号风格

统一使用：

```cpp
if (...)
{
    ...
}
else
{
    ...
}
```

适用于：

- if
- for
- while
- function
- struct
- lambda

禁止 K&R 风格：

```cpp
if (...) {

}
```

---

# 3. 代码紧凑程度

保持正常竞赛代码风格。

简单逻辑不要人为拆行。

推荐：

```cpp
dfs(l, mid - 1, opL, bestPos);
```

不推荐：

```cpp
dfs(
    l,
    mid - 1,
    opL,
    bestPos
);
```

---

# 4. 类型习惯

优先使用：

```cpp
ll
pll
db
```

不要无意义展开：

```cpp
long long
pair<long long,long long>
double
```

---

# 5. 下标习惯

## 默认规则

普通：

- 数组
- vector
- 图节点
- 树节点
- 按位置 DP

优先使用 1-index。

例如：

```cpp
vector<int> a(n + 1);

for (int i = 1; i <= n; i++)
{
    cin >> a[i];
}
```

---

## 例外

天然适合 0-index 的算法保持 0-index：

包括：

- 状压 DP
- bitmask
- 位运算状态
- 部分字符串算法
- 标准库相关操作

原则：

选择更自然、更短、更不容易错的写法。

---

# 6. 命名规则

## 函数名

使用竞赛风格：

推荐：

```
dfs
bfs
calc
check
add
del
get
query
update
build
work
merge
```

不要使用工程化长命名。

---

## 变量名

推荐：

```
n,m,k

i,j

l,r,mid

u,v

cnt

ans

res

sum

dp

dis

vis
```

允许：

```
bestPos
opL
opR
```

避免歧义。

---

# 7. lambda 与辅助函数

局部简单逻辑：

优先 lambda。

例如：

```cpp
auto check = [&](ll x)
{
    ...
};
```

不要为了小功能增加大量全局函数。

但是：

- 多处调用
- 递归
- 依赖大量全局状态

可以写全局函数。

---

# 8. struct 与封装

适合 struct：

- SCC
- LCA
- Segment Tree
- Flow
- Trie
- SAM

不适合：

- gcd
- bfs
- dijkstra
- kmp

不要为了面向对象强行封装。

禁止：

- getter/setter
- 多层 class
- 无意义接口


---

# 9. DFS 与递归

天然递归问题：

优先递归：

- DFS
- 树 DP
- 换根 DP
- Tarjan
- 分治
- 回溯

不要无意义改成手写 stack。

BFS：

正常使用 queue。

原则：

天然递归的问题递归。

天然迭代的问题迭代。

---

# 10. 图论风格

默认：

无权图：

```cpp
vector<vector<int>> g(n + 1);
```

带权图：

```cpp
vector<vector<pair<int,ll>>> g(n + 1);
```

节点默认：

```
1...n
```

不要复杂图类封装。

---

# 11. 数据结构风格

已有模板：

优先修改。

不要整套替换。

例如：

已有：

```
Info
Tag
push
pull
```

就在此基础修改。

简单需求不要增加无用功能。

---

# 12. 注释规则

只保留有价值注释：

例如：

- 容易错的边界
- DP 状态
- 数学结论
- 特殊处理原因

不要：

```cpp
// 输入 n
cin >> n;

// 遍历数组
for (...)
```

---

# 13. 修改已有代码

修改已有模板：

优先最小修改。

保留：

- 变量名
- 函数名
- 数据结构
- 下标体系
- 整体算法

不要重写成另一种风格。

---

# 14. 正确性检查

提交前检查：

- 是否编译
- 样例是否正确
- 边界情况
- 数据类型
- 数组越界
- 多测清空
- 时间复杂度
- 空间复杂度
- 递归深度

可以对拍时：

优先随机对拍。

---

# 15. 最终原则

所有代码最终应该满足：

> 看起来像本人在 ICPC 现场写出来的代码。

而不是：

> 工程团队维护的代码。

优先级：

正确

>

自然

>

短

>

稳定

>

符合个人习惯