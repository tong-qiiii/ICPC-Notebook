## 3.1 倍增 LCA

> **用途**：在线查询树上两点最近公共祖先，也可查询节点的第 `k` 级祖先。
> **复杂度**：预处理 `O(n log n)`，单次查询 `O(log n)`，空间 `O(n log n)`。
> **下标**：节点 `1-indexed`，`0` 表示不存在的祖先。
> **限制**：输入必须是一棵树。
> **注意**：递归 DFS 在极深链上可能爆栈。

### 调用

```cpp
LCA lca(n);

for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    lca.add(u, v);
}

lca.build(1);

cout << lca.lca(u, v) << '\n';
cout << lca.kth_up(u, k) << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 初始化 |
| `add(u, v)` | 加无向树边 |
| `build(root)` | 以 `root` 为根预处理 |
| `lca(u, v)` | 最近公共祖先 |
| `kth_up(u, k)` | 向上跳 `k` 条边，越过根返回 `0` |
| `dep[u]` | `u` 的深度 |
| `fa[k][u]` | `u` 的 `2^k` 级祖先 |

### 模板

```cpp
struct LCA {
    int n = 0, lg = 0;
    vector<vector<int>> g, fa;
    vector<int> dep;

    LCA(int n = 0) {
        if (n) init(n);
    }

    void init(int n_) {
        n = n_;
        lg = 1;
        while ((1LL << lg) <= n) ++lg;

        g.assign(n + 1, {});
        fa.assign(lg, vector<int>(n + 1));
        dep.assign(n + 1, 0);
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 1) {
        dfs(root, 0);

        for (int k = 1; k < lg; ++k)
            for (int u = 1; u <= n; ++u)
                fa[k][u] = fa[k - 1][fa[k - 1][u]];
    }

    int kth_up(int u, int k) const {
        if (k >= dep[u]) return 0;

        for (int i = 0; i < lg; ++i)
            if (k >> i & 1)
                u = fa[i][u];

        return u;
    }

    int lca(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);

        u = kth_up(u, dep[u] - dep[v]);
        if (u == v) return u;

        for (int k = lg - 1; k >= 0; --k) {
            if (fa[k][u] != fa[k][v]) {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    }

private:
    void dfs(int u, int p) {
        fa[0][u] = p;
        dep[u] = dep[p] + 1;

        for (int v : g[u])
            if (v != p)
                dfs(v, u);
    }
};
```

### 现场记忆

`fa[k][u] = fa[k-1][fa[k-1][u]]`

先把深度大的点跳到同一深度。

若已经相等，当前点就是 LCA。

否则从大到小枚举 `k`，只在 `fa[k][u] != fa[k][v]` 时一起跳。

最后答案是 `fa[0][u]`。

## 3.2 树剖 LCA

> **用途**：用重链剖分预处理树，在线查询两点最近公共祖先。
> **复杂度**：预处理 `O(n)`，单次查询 `O(log n)`，空间 `O(n)`。
> **下标**：节点 `1-indexed`。
> **限制**：输入必须是一棵树。
> **注意**：递归 DFS 在极深链上可能爆栈。

### 调用

```cpp
LCAHLD lca(n);

for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    lca.add(u, v);
}

lca.build(1);

cout << lca.lca(u, v) << '\n';
```

### 模板

```cpp
struct LCAHLD {
    int n = 0;
    vector<vector<int>> g;
    vector<int> fa, dep, siz, son, top;

    LCAHLD(int n = 0) {
        if (n) init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        fa.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 1) {
        fill(son.begin(), son.end(), 0);
        dfs1(root, 0);
        dfs2(root, root);
    }

    int lca(int u, int v) const {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]])
                swap(u, v);
            u = fa[top[u]];
        }

        return dep[u] < dep[v] ? u : v;
    }

private:
    void dfs1(int u, int p) {
        fa[u] = p;
        dep[u] = dep[p] + 1;
        siz[u] = 1;

        for (int v : g[u]) {
            if (v == p) continue;

            dfs1(v, u);
            siz[u] += siz[v];

            if (siz[v] > siz[son[u]])
                son[u] = v;
        }
    }

    void dfs2(int u, int tp) {
        top[u] = tp;

        if (son[u])
            dfs2(son[u], tp);

        for (int v : g[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }
    }
};
```

### 现场记忆

`son[u]` = 子树最大的儿子，重儿子和父亲共处一条重链。

不同链时，让 `top` 更深的一边跳：

`u = fa[top[u]]`

两点进入同一条链后，深度更浅的点就是 LCA。

重新 `build()` 时记得清空 `son`。

## 3.3 Tarjan 离线 LCA

> **用途**：一次性求出树上一批已知询问的最近公共祖先。
> **复杂度**：时间 `O((n+q) α(n))`，空间 `O(n+q)`。
> **下标**：节点 `1-indexed`，询问编号 `0-indexed`。
> **限制**：询问必须在执行 `work()` 前加入；输入必须是一棵树。
> **注意**：递归 DFS 在极深链上可能爆栈。

### 调用

```cpp
LCATarjan lca(n);

for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    lca.add(u, v);
}

for (int i = 0; i < q; ++i) {
    int u, v;
    cin >> u >> v;
    lca.ask(u, v);
}

const auto &ans = lca.work(1);

for (int x : ans)
    cout << x << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 初始化 |
| `add(u, v)` | 加无向树边 |
| `ask(u, v)` | 加入询问并返回询问编号 |
| `work(root)` | 执行全部离线询问 |
| `ans[id]` | 第 `id` 个询问的 LCA |

### 模板

```cpp
struct LCATarjan {
    int n = 0;
    vector<vector<int>> g;
    vector<vector<pair<int, int>>> qry;
    vector<int> ans;

    LCATarjan(int n = 0) {
        if (n) init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        qry.assign(n + 1, {});
        ans.clear();

        dsu.resize(n + 1);
        siz.assign(n + 1, 1);
        anc.assign(n + 1, 0);
        vis.assign(n + 1, 0);
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int ask(int u, int v) {
        int id = ans.size();
        ans.push_back(0);
        qry[u].push_back({v, id});
        qry[v].push_back({u, id});
        return id;
    }

    const vector<int>& work(int root = 1) {
        iota(dsu.begin(), dsu.end(), 0);
        fill(siz.begin(), siz.end(), 1);
        fill(anc.begin(), anc.end(), 0);
        fill(vis.begin(), vis.end(), 0);
        fill(ans.begin(), ans.end(), 0);

        tarjan(root, 0);
        return ans;
    }

private:
    vector<int> dsu, siz, anc;
    vector<char> vis;

    int find(int x) {
        return dsu[x] == x ? x : dsu[x] = find(dsu[x]);
    }

    void merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;

        if (siz[x] < siz[y])
            swap(x, y);

        dsu[y] = x;
        siz[x] += siz[y];
    }

    void tarjan(int u, int p) {
        anc[u] = u;

        for (int v : g[u]) {
            if (v == p) continue;

            tarjan(v, u);
            merge(u, v);
            anc[find(u)] = u;
        }

        vis[u] = 1;

        for (auto [v, id] : qry[u])
            if (vis[v])
                ans[id] = anc[find(v)];
    }
};
```

### 现场记忆

DFS 完一个儿子后：

`merge(u, v)`，再令 `anc[find(u)] = u`。

`vis[v] = 1` 表示 `v` 的整棵子树已经处理完成。

处理询问 `(u, v)` 时，只有 `v` 已完成才回答：

`ans = anc[find(v)]`

查询挂在两个端点各一份，但只会在合法时机得到同一个答案。

## 3.4 重链剖分 HLD

> **用途**：将树上路径拆成若干个连续 DFS 序区间，并将子树映射为一个连续区间；具体区间操作交给线段树、Fenwick 等数据结构处理。

> **复杂度**：预处理 `O(n)`；`path()` / `path_edge()` 产生 `O(log n)` 个区间；`lca()` `O(log n)`；`subtree()` `O(1)`；空间 `O(n)`。

> **下标**：节点、DFS 序均为 `1-indexed`。

> **注意**：
> - `path()` 只保证完整覆盖路径，不保证回调顺序等于 `u -> v` 的路径顺序；非交换信息需要额外处理方向。
> - `path_edge()` 默认将边权放在较深端点的 `dfn` 上，因此会排除 LCA。
> - 两次 DFS 使用递归，极深链上可能爆栈。

### 调用

```cpp
HLD hld(n);

for (int i = 1; i < n; i++)
{
    int u, v;
    cin >> u >> v;
    hld.add(u, v);
}

hld.build(1);

// 点权按 DFS 序放入外部数据结构
for (int u = 1; u <= n; u++)
{
    a[hld.dfn[u]] = val[u];
}

// 路径操作
hld.path(u, v, [&](int l, int r)
{
    seg.modify(l, r, x);
});

// 路径查询
ll ans = 0;
hld.path(u, v, [&](int l, int r)
{
    ans += seg.query(l, r);
});

// 子树操作
auto [l, r] = hld.subtree(u);
seg.modify(l, r, x);

// LCA
int p = hld.lca(u, v);
```

Fenwick 等其他数据结构同样直接接区间：

```cpp
ll ans = 0;

hld.path(u, v, [&](int l, int r)
{
    ans += bit.query(r) - bit.query(l - 1);
});
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 初始化 |
| `add(u,v)` | 加无向树边 |
| `build(root)` | 完成重链剖分 |
| `path(u,v,f)` | 将点路径拆成若干 `[l,r]`，调用 `f(l,r)` |
| `path_edge(u,v,f)` | 将边路径拆成若干 `[l,r]` |
| `subtree(u)` | 返回 `u` 子树的 DFS 序区间 |
| `lca(u,v)` | 最近公共祖先 |
| `fa[u]` | 父节点 |
| `dep[u]` | 深度 |
| `siz[u]` | 子树大小 |
| `son[u]` | 重儿子 |
| `top[u]` | 所在重链链顶 |
| `dfn[u]` | DFS 序 |

### 模板

```cpp
struct HLD
{
    int n = 0, timer = 0;
    vector<vector<int>> g;
    vector<int> fa, dep, siz, son, top, dfn;

    HLD(int n = 0)
    {
        if (n)
        {
            init(n);
        }
    }

    void init(int n_)
    {
        n = n_;
        timer = 0;
        g.assign(n + 1, {});
        fa.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
        dfn.assign(n + 1, 0);
    }

    void add(int u, int v)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 1)
    {
        timer = 0;
        fill(son.begin(), son.end(), 0);
        dfs1(root, 0);
        dfs2(root, root);
    }

    template<class F>
    void path(int u, int v, F &&f) const
    {
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }

            f(dfn[top[u]], dfn[u]);
            u = fa[top[u]];
        }

        if (dep[u] > dep[v])
        {
            swap(u, v);
        }

        f(dfn[u], dfn[v]);
    }

    template<class F>
    void path_edge(int u, int v, F &&f) const
    {
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }

            f(dfn[top[u]], dfn[u]);
            u = fa[top[u]];
        }

        if (dep[u] > dep[v])
        {
            swap(u, v);
        }

        if (dfn[u] < dfn[v])
        {
            f(dfn[u] + 1, dfn[v]);
        }
    }

    pair<int, int> subtree(int u) const
    {
        return {dfn[u], dfn[u] + siz[u] - 1};
    }

    int lca(int u, int v) const
    {
        while (top[u] != top[v])
        {
            if (dep[top[u]] < dep[top[v]])
            {
                swap(u, v);
            }

            u = fa[top[u]];
        }

        return dep[u] < dep[v] ? u : v;
    }

private:
    void dfs1(int u, int p)
    {
        fa[u] = p;
        dep[u] = dep[p] + 1;
        siz[u] = 1;

        for (int v : g[u])
        {
            if (v == p)
            {
                continue;
            }

            dfs1(v, u);
            siz[u] += siz[v];

            if (siz[v] > siz[son[u]])
            {
                son[u] = v;
            }
        }
    }

    void dfs2(int u, int tp)
    {
        top[u] = tp;
        dfn[u] = ++timer;

        if (son[u])
        {
            dfs2(son[u], tp);
        }

        for (int v : g[u])
        {
            if (v == fa[u] || v == son[u])
            {
                continue;
            }

            dfs2(v, v);
        }
    }
};
```

### 现场记忆

```text
dfs2 重儿子优先 -> 一条重链连续，整棵子树也连续。

path：
处理 [dfn[top[u]], dfn[u]]
然后 u = fa[top[u]]

同链后处理 [dfn[u], dfn[v]]

subtree(u) = [dfn[u], dfn[u] + siz[u] - 1]

边权放到深端点，path_edge 最后一段排除 LCA。

path() 不保证 u -> v 顺序；非交换合并要单独处理方向。
```


## 3.5 虚树

> **用途**：把一批关键点及其必要 LCA 压缩成一棵 `O(k)` 大小的树，方便只在关键结构上做树形 DP。
> **复杂度**：预处理 `O(n log n)`；设关键点数为 `k`，单次构建 `O(k log n)`，虚树节点数不超过 `2k - 1`。
> **下标**：原树节点 `1-indexed`。
> **注意**：`vt` 中的边为祖先到儿子的有向边；每次 `build()` 会自动清空上一棵虚树。递归 DFS 在极深链上可能爆栈。

### 调用

```cpp
VTree vt(n);

for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    vt.add(u, v);
}

vt.prep(1);

vector<int> key = {a, b, c};
int root = vt.build(key);

dfs(root);  // 沿 vt.vt[u] 做虚树 DP
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n)` | 初始化 |
| `add(u, v)` | 加原树无向边 |
| `prep(root)` | 预处理 DFS 序和倍增 LCA |
| `lca(u, v)` | 原树上的最近公共祖先 |
| `build(key)` | 构建虚树并返回虚树根，空集返回 `0` |
| `vt[u]` | 虚树中 `u` 的儿子 |
| `nodes` | 当前虚树全部节点 |
| `dep[u]` | 原树深度 |
| `dfn[u]` | 原树 DFS 序 |

### 模板

```cpp
struct VTree {
    int n = 0, lg = 0, tms = 0;
    vector<vector<int>> g, vt, fa;
    vector<int> dep, siz, dfn, nodes;

    VTree(int n = 0) {
        if (n) init(n);
    }

    void init(int n_) {
        n = n_;
        lg = 1;
        while ((1LL << lg) <= n) ++lg;

        tms = 0;
        g.assign(n + 1, {});
        vt.assign(n + 1, {});
        fa.assign(lg, vector<int>(n + 1, 0));
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        dfn.assign(n + 1, 0);
        nodes.clear();
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void prep(int root = 1) {
        clear_vt();
        tms = 0;

        dfs(root, 0);

        for (int k = 1; k < lg; ++k)
            for (int u = 1; u <= n; ++u)
                fa[k][u] = fa[k - 1][fa[k - 1][u]];
    }

    int lca(int u, int v) const {
        if (dep[u] < dep[v])
            swap(u, v);

        int d = dep[u] - dep[v];
        for (int k = 0; k < lg; ++k)
            if (d >> k & 1)
                u = fa[k][u];

        if (u == v)
            return u;

        for (int k = lg - 1; k >= 0; --k) {
            if (fa[k][u] != fa[k][v]) {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    }

    int build(vector<int> key) {
        clear_vt();

        if (key.empty())
            return 0;

        auto cmp = [&](int u, int v) {
            return dfn[u] < dfn[v];
        };

        sort(key.begin(), key.end(), cmp);
        key.erase(unique(key.begin(), key.end()), key.end());

        int k = key.size();
        for (int i = 0; i + 1 < k; ++i)
            key.push_back(lca(key[i], key[i + 1]));

        sort(key.begin(), key.end(), cmp);
        key.erase(unique(key.begin(), key.end()), key.end());
        nodes = key;

        vector<int> stk;

        for (int u : nodes) {
            while (!stk.empty() && !is_ancestor(stk.back(), u))
                stk.pop_back();

            if (!stk.empty())
                vt[stk.back()].push_back(u);

            stk.push_back(u);
        }

        return nodes[0];
    }

private:
    bool is_ancestor(int u, int v) const {
        return dfn[u] <= dfn[v] && dfn[v] < dfn[u] + siz[u];
    }

    void clear_vt() {
        for (int u : nodes)
            vt[u].clear();
        nodes.clear();
    }

    void dfs(int u, int p) {
        dfn[u] = ++tms;
        fa[0][u] = p;
        dep[u] = dep[p] + 1;
        siz[u] = 1;

        for (int v : g[u]) {
            if (v == p) continue;

            dfs(v, u);
            siz[u] += siz[v];
        }
    }
};
```

### 现场记忆

关键点先按 `dfn` 排序，只需补相邻关键点的 LCA。

补完 LCA 再排序去重，虚树节点数最多 `2k - 1`。

祖先判断：

`dfn[u] <= dfn[v] < dfn[u] + siz[u]`

节点按 DFS 序扫描，栈顶不是当前点祖先就弹栈。

最终 `vt[parent].push_back(child)`，直接按原树祖先方向做 DP。


## 3.6 启发式合并 / DSU on Tree：子树不同颜色数

> **用途**：统计以每个节点为根的子树中不同颜色的数量。
> **复杂度**：时间 `O(n log n)`，空间 `O(n)`。
> **下标**：节点 `1-indexed`。
> **注意**：颜色值可为任意 `int`，`work()` 内部自动离散化；递归在链形树上可能爆栈。

### 调用

```cpp
DSUOnTree dsu(n);

for (int u = 1; u <= n; ++u) {
    int c;
    cin >> c;
    dsu.set_color(u, c);
}

for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    dsu.add(u, v);
}

const auto &ans = dsu.work(1);

cout << ans[u] << '\n';
```

### 模板

```cpp
struct DSUOnTree {
    int n = 0, diff = 0;
    vector<vector<int>> g;
    vector<int> color, siz, son, ans;

    DSUOnTree(int n = 0) {
        if (n) init(n);
    }

    void init(int n_) {
        n = n_;

        g.assign(n + 1, {});
        color.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        ans.assign(n + 1, 0);

        col.assign(n + 1, 0);
        cnt.clear();
        diff = 0;
    }

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void set_color(int u, int c) {
        color[u] = c;
    }

    const vector<int>& work(int root = 1) {
        vector<int> xs(color.begin() + 1, color.end());
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());

        for (int u = 1; u <= n; ++u)
            col[u] = lower_bound(xs.begin(), xs.end(), color[u]) - xs.begin();

        cnt.assign(xs.size(), 0);
        fill(son.begin(), son.end(), 0);
        fill(ans.begin(), ans.end(), 0);
        diff = 0;

        dfs_size(root, 0);
        dfs(root, 0, false);

        return ans;
    }

private:
    vector<int> col, cnt;

    void dfs_size(int u, int p) {
        siz[u] = 1;

        for (int v : g[u]) {
            if (v == p) continue;

            dfs_size(v, u);
            siz[u] += siz[v];

            if (siz[v] > siz[son[u]])
                son[u] = v;
        }
    }

    void add_node(int u) {
        if (cnt[col[u]]++ == 0)
            ++diff;
    }

    void del_node(int u) {
        if (--cnt[col[u]] == 0)
            --diff;
    }

    void add_subtree(int u, int p) {
        add_node(u);

        for (int v : g[u])
            if (v != p)
                add_subtree(v, u);
    }

    void del_subtree(int u, int p) {
        del_node(u);

        for (int v : g[u])
            if (v != p)
                del_subtree(v, u);
    }

    void dfs(int u, int p, bool keep) {
        for (int v : g[u]) {
            if (v == p || v == son[u]) continue;
            dfs(v, u, false);
        }

        if (son[u])
            dfs(son[u], u, true);

        for (int v : g[u]) {
            if (v == p || v == son[u]) continue;
            add_subtree(v, u);
        }

        add_node(u);
        ans[u] = diff;

        if (!keep)
            del_subtree(u, p);
    }
};
```

### 现场记忆

先算 `siz / son`。

轻儿子：递归求完后清空。

重儿子：最后递归，并保留统计结果。

再把所有轻儿子整棵加入，最后加入 `u`：

`ans[u] = 当前统计答案`

`keep = false` 时离开前把整棵 `u` 子树清空。


## 3.7 点分治：统计距离不超过 k 的点对数

> **用途**：统计加权树中距离不超过 `k` 的无序点对数量。
> **复杂度**：时间 `O(n log^2 n)`，空间 `O(n)`。
> **下标**：节点 `1-indexed`。
> **注意**：距离和 `k` 使用 `long long`；递归在极深树上可能爆栈。

### 调用

```cpp
Centroid cd(n, k);

for (int i = 1; i < n; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    cd.add(u, v, w);
}

cout << cd.work() << '\n';
```

### 模板

```cpp
struct Centroid {
    using ll = long long;

    struct Edge {
        int to;
        ll w;
    };

    int n = 0;
    ll k = 0;
    vector<vector<Edge>> g;

    Centroid(int n = 0, ll k = 0) {
        if (n) init(n, k);
    }

    void init(int n_, ll k_) {
        n = n_;
        k = k_;

        g.assign(n + 1, {});
        siz.assign(n + 1, 0);
        removed.assign(n + 1, 0);
        ans = 0;
    }

    void add(int u, int v, ll w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    ll work() {
        ans = 0;
        fill(removed.begin(), removed.end(), 0);
        decompose(1);
        return ans;
    }

private:
    ll ans = 0;
    vector<int> siz;
    vector<char> removed;

    void get_size(int u, int p) {
        siz[u] = 1;

        for (auto [v, w] : g[u]) {
            if (v == p || removed[v]) continue;

            get_size(v, u);
            siz[u] += siz[v];
        }
    }

    int get_centroid(int u, int p, int total) {
        for (auto [v, w] : g[u]) {
            if (v == p || removed[v]) continue;

            if (siz[v] > total / 2)
                return get_centroid(v, u, total);
        }

        return u;
    }

    void collect(int u, int p, ll dis, vector<ll> &ds) {
        ds.push_back(dis);

        for (auto [v, w] : g[u]) {
            if (v == p || removed[v]) continue;
            collect(v, u, dis + w, ds);
        }
    }

    ll count_pairs(vector<ll> &ds) {
        sort(ds.begin(), ds.end());

        ll res = 0;
        int l = 0, r = (int)ds.size() - 1;

        while (l < r) {
            if ((__int128)ds[l] + ds[r] <= k) {
                res += r - l;
                ++l;
            } else {
                --r;
            }
        }

        return res;
    }

    void decompose(int entry) {
        get_size(entry, 0);

        int total = siz[entry];
        int c = get_centroid(entry, 0, total);
        removed[c] = 1;

        vector<ll> all = {0};
        all.reserve(total);

        for (auto [v, w] : g[c]) {
            if (removed[v]) continue;

            vector<ll> sub;
            collect(v, c, w, sub);

            ans -= count_pairs(sub);
            all.insert(all.end(), sub.begin(), sub.end());
        }

        ans += count_pairs(all);

        for (auto [v, w] : g[c])
            if (!removed[v])
                decompose(v);
    }
};
```

### 现场记忆

对当前重心 `c`：

`所有经过 c 的合法点对 = count(all) - Σ count(subtree)`

其中距离都表示节点到 `c` 的距离，`all` 额外加入重心自身的距离 `0`。

`count()`：排序后双指针；若 `d[l] + d[r] <= k`，则当前 `l` 能和 `l+1 ... r` 全部配对。

统计完删除重心，再递归每个剩余连通块。

本实现每层需要排序距离数组，因此总复杂度 `O(n log^2 n)`。

## 3.8 Kruskal 重构树

> **用途**：处理最小瓶颈、边权阈值下的连通块，将问题转成重构树上的 LCA / 祖先 / 子树问题。
> **复杂度**：构树 `O(m log m)`，倍增 `O(n log n)`；`lca / climb` 单次 `O(log n)`，空间 `O(n log n + m)`。
> **下标**：原图节点 `1..n`，重构节点 `n+1..tot`。
> **注意**：边权从小到大构造；原图不连通时得到森林，`bottleneck(u,v)` 要求两点连通。

### 调用

```cpp
KRT krt(n);

for (int i = 1; i <= m; i++)
{
    int u, v;
    ll w;
    cin >> u >> v >> w;
    krt.add(u, v, w);
}

krt.build();

if (krt.connected(u, v))
{
    cout << krt.bottleneck(u, v) << endl;
}

int p = krt.climb(u, x);

cout << krt.siz[p] << endl;

int l = krt.dfn[p];
int r = krt.dfn[p] + krt.siz[p] - 1;
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u,v,w)` | 加无向边 |
| `build()` | 建 Kruskal 重构森林并预处理倍增 |
| `connected(u,v)` | 判断两点最终是否连通 |
| `lca(u,v)` | 重构树 LCA；不连通返回 `0` |
| `bottleneck(u,v)` | 两点最小瓶颈值 |
| `climb(u,x)` | 最高的 `val <= x` 祖先 |
| `val[u]` | 重构点对应边权 |
| `siz[u]` | 子树中的原图节点数 |
| `dfn[u]` | 原图节点 DFS 序区间左端点 |
| `bel[u]` | 所在重构树的根 |
| `roots` | 重构森林所有根 |

### 模板

```cpp
struct KRT
{
    struct Edge
    {
        int u, v;
        ll w;

        bool operator<(const Edge &o) const
        {
            return w < o.w;
        }
    };

    int n = 0, tot = 0, lg = 0, timer = 0;
    vector<Edge> edges;
    vector<vector<int>> g, fa;
    vector<ll> val;
    vector<int> dep, siz, dfn, bel, roots;

    KRT(int n = 0)
    {
        if (n)
        {
            init(n);
        }
    }

    void init(int n_)
    {
        n = n_;
        tot = n;
        timer = 0;

        lg = 1;
        while ((1LL << lg) <= 2 * n)
        {
            lg++;
        }

        edges.clear();
    }

    void add(int u, int v, ll w)
    {
        edges.push_back({u, v, w});
    }

    void build()
    {
        sort(edges.begin(), edges.end());

        int N = 2 * n + 5;

        tot = n;
        timer = 0;

        g.assign(N, {});
        fa.assign(lg, vector<int>(N));
        val.assign(N, 0);
        dep.assign(N, 0);
        siz.assign(N, 0);
        dfn.assign(N, 0);
        bel.assign(N, 0);
        roots.clear();

        dsu.resize(N);
        iota(dsu.begin(), dsu.end(), 0);

        for (auto [u, v, w] : edges)
        {
            int fu = find(u);
            int fv = find(v);

            if (fu == fv)
            {
                continue;
            }

            int p = ++tot;

            val[p] = w;
            g[p].push_back(fu);
            g[p].push_back(fv);

            dsu[fu] = dsu[fv] = p;
            dsu[p] = p;
        }

        for (int u = 1; u <= tot; u++)
        {
            if (dsu[u] == u)
            {
                roots.push_back(u);
                dep[u] = 1;
                dfs(u, u);
            }
        }

        for (int k = 1; k < lg; k++)
        {
            for (int u = 1; u <= tot; u++)
            {
                fa[k][u] = fa[k - 1][fa[k - 1][u]];
            }
        }
    }

    bool connected(int u, int v) const
    {
        return bel[u] == bel[v];
    }

    int lca(int u, int v) const
    {
        if (!connected(u, v))
        {
            return 0;
        }

        if (dep[u] < dep[v])
        {
            swap(u, v);
        }

        int d = dep[u] - dep[v];

        for (int k = 0; k < lg; k++)
        {
            if (d >> k & 1)
            {
                u = fa[k][u];
            }
        }

        if (u == v)
        {
            return u;
        }

        for (int k = lg - 1; k >= 0; k--)
        {
            if (fa[k][u] != fa[k][v])
            {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    }

    ll bottleneck(int u, int v) const
    {
        return val[lca(u, v)];
    }

    int climb(int u, ll x) const
    {
        for (int k = lg - 1; k >= 0; k--)
        {
            int v = fa[k][u];

            if (v && val[v] <= x)
            {
                u = v;
            }
        }

        return u;
    }

private:
    vector<int> dsu;

    int find(int x)
    {
        return dsu[x] == x ? x : dsu[x] = find(dsu[x]);
    }

    void dfs(int u, int root)
    {
        bel[u] = root;
        dfn[u] = timer + 1;

        if (u <= n)
        {
            siz[u] = 1;
            timer++;
            return;
        }

        for (int v : g[u])
        {
            fa[0][v] = u;
            dep[v] = dep[u] + 1;

            dfs(v, root);

            siz[u] += siz[v];
        }
    }
};
```

### 现场记忆

Kruskal 从小到大合并两个连通块根 `fu, fv`，新建点 `p`，`val[p] = w`。

新点编号一定大于儿子，所以可以按编号从大到小下传 `bel / dfn`，不需要 DFS。

`val[lca(u,v)]` = 两点连通所需的最小边权阈值 = 最小瓶颈值。

限制边权 `<= x`：从原点倍增向上跳到最高的 `val <= x` 祖先。

该祖先对应当前连通块，大小为 `siz[p]`，原图点区间为 `[dfn[p], dfn[p] + siz[p] - 1]`。


## 3.9 LCT：动态森林 / 路径加 / 路径和

> **用途**：维护动态森林，支持动态连边、删边，以及树链上的点权加、点权和。
> **复杂度**：`link / cut / connected / set_val / path_add / path_sum` 均摊 `O(log n)`，空间 `O(n)`。
> **下标**：节点 `1-indexed`，`0` 为哨兵空节点。
> **注意**：只能维护森林，`link(u,v)` 不允许形成环；`cut(u,v)` 只删除原树中的直接边；`split / path_add / path_sum` 要求 `u,v` 已连通。

### 调用

```cpp
LCT lct(n);

for (int u = 1; u <= n; ++u) {
    long long x;
    cin >> x;
    lct.set_val(u, x);
}

lct.link(u, v);
lct.cut(u, v);

if (lct.connected(u, v)) {
    lct.path_add(u, v, x);
    cout << lct.path_sum(u, v) << '\n';
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `init(n)` | 初始化 / 清空 |
| `set_val(u, x)` | 将节点 `u` 权值设为 `x` |
| `connected(u, v)` | 判断两点是否连通 |
| `link(u, v)` | 连边，已连通返回 `false` |
| `cut(u, v)` | 删除直接边 `(u,v)`，不存在返回 `false` |
| `makeroot(u)` | 将 `u` 设为所在原树的根 |
| `split(u, v)` | 提取路径 `u -> v`，之后 `v` 的 Splay 即整条路径 |
| `path_add(u, v, x)` | 路径所有节点加 `x` |
| `path_sum(u, v)` | 查询路径节点权值和 |

### 模板

```cpp
struct LCT {
    using ll = long long;

    int n = 0;
    vector<array<int, 2>> ch;
    vector<int> fa, siz, stk;
    vector<ll> val, sum, add;
    vector<char> rev;

    LCT(int n = 0) {
        if (n) init(n);
    }

    void init(int n_) {
        n = n_;

        ch.assign(n + 1, {0, 0});
        fa.assign(n + 1, 0);
        siz.assign(n + 1, 1);
        stk.assign(n + 1, 0);

        val.assign(n + 1, 0);
        sum.assign(n + 1, 0);
        add.assign(n + 1, 0);
        rev.assign(n + 1, 0);

        siz[0] = 0;
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }

    int findroot(int x) {
        access(x);
        splay(x);

        push_down(x);

        while (ch[x][0]) {
            x = ch[x][0];
            push_down(x);
        }

        splay(x);
        return x;
    }

    bool connected(int x, int y) {
        if (x == y)
            return true;

        return findroot(x) == findroot(y);
    }

    void split(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
    }

    bool link(int x, int y) {
        makeroot(x);

        if (findroot(y) == x)
            return false;

        fa[x] = y;
        return true;
    }

    bool cut(int x, int y) {
        split(x, y);

        if (ch[y][0] != x || ch[x][1])
            return false;

        ch[y][0] = 0;
        fa[x] = 0;
        push_up(y);

        return true;
    }

    void set_val(int x, ll v) {
        access(x);
        splay(x);

        val[x] = v;
        push_up(x);
    }

    void path_add(int x, int y, ll v) {
        split(x, y);
        apply_add(y, v);
    }

    ll path_sum(int x, int y) {
        split(x, y);
        return sum[y];
    }

private:
    bool is_root(int x) const {
        int f = fa[x];
        return ch[f][0] != x && ch[f][1] != x;
    }

    void push_up(int x) {
        siz[x] = siz[ch[x][0]] + siz[ch[x][1]] + 1;
        sum[x] = sum[ch[x][0]] + sum[ch[x][1]] + val[x];
    }

    void apply_rev(int x) {
        if (!x) return;

        swap(ch[x][0], ch[x][1]);
        rev[x] ^= 1;
    }

    void apply_add(int x, ll v) {
        if (!x) return;

        val[x] += v;
        sum[x] += v * siz[x];
        add[x] += v;
    }

    void push_down(int x) {
        if (rev[x]) {
            apply_rev(ch[x][0]);
            apply_rev(ch[x][1]);
            rev[x] = 0;
        }

        if (add[x]) {
            apply_add(ch[x][0], add[x]);
            apply_add(ch[x][1], add[x]);
            add[x] = 0;
        }
    }

    void rotate(int x) {
        int y = fa[x];
        int z = fa[y];
        int k = (ch[y][1] == x);
        int w = ch[x][k ^ 1];

        if (!is_root(y))
            ch[z][ch[z][1] == y] = x;

        fa[x] = z;

        ch[x][k ^ 1] = y;
        fa[y] = x;

        ch[y][k] = w;
        if (w)
            fa[w] = y;

        push_up(y);
        push_up(x);
    }

    void splay(int x) {
        int top = 0;
        int y = x;

        stk[++top] = y;

        while (!is_root(y))
            stk[++top] = y = fa[y];

        while (top)
            push_down(stk[top--]);

        while (!is_root(x)) {
            int y = fa[x];
            int z = fa[y];

            if (!is_root(y)) {
                if ((ch[y][1] == x) == (ch[z][1] == y))
                    rotate(y);
                else
                    rotate(x);
            }

            rotate(x);
        }
    }

    void access(int x) {
        for (int y = 0; x; x = fa[y = x]) {
            splay(x);
            ch[x][1] = y;
            push_up(x);
        }
    }
};
```

### 现场记忆

`access(x) + splay(x)`：把根到 `x` 的路径变成 `x` 的 Splay。

`makeroot(x)`：`access + splay + rev`。

`split(x,y)`：`makeroot(x)` 后 `access(y), splay(y)`，此时 `y` 的 Splay 就是 `x -> y`。

所以路径修改 / 查询直接改或读 `y`。

`link`：先 `makeroot(x)`，确认不同树，再 `fa[x] = y`。

`cut`：`split(x,y)` 后，直接边满足 `ch[y][0] == x && ch[x][1] == 0`。

Splay 前必须把祖先到当前点的懒标记依次 `push_down`。
