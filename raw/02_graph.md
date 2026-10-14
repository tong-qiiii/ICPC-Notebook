## 2.1 强连通分量 SCC - Tarjan + 缩点

> **用途**：求有向图强连通分量，并可缩点得到 DAG。
> **复杂度**：SCC 时间 `O(n+m)`，缩点时间 `O(n+m)`；空间 `O(n+m)`。
> **下标**：节点 `1-indexed`，SCC 编号 `1..cnt`。
> **注意**：递归 Tarjan 在链状大图上可能爆栈。

### 调用

```cpp
SCC scc(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    scc.add(u, v);
}

scc.work();

cout << scc.cnt << '\n';
cout << scc.bel[u] << '\n';

auto dag = scc.dag();
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u, v)` | 加有向边 `u -> v` |
| `work()` | 求 SCC |
| `dag()` | SCC 缩点，需先 `work()` |
| `cnt` | SCC 数量 |
| `bel[u]` | `u` 所属 SCC |
| `siz[id]` | SCC `id` 的大小 |

### 模板

```cpp
struct SCC {
    int n, timer, cnt;
    vector<vector<int>> g;
    vector<int> dfn, low, bel, siz, stk;
    vector<char> in;

    SCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        timer = cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        bel.assign(n + 1, 0);
        siz.clear();
        stk.clear();
        in.assign(n + 1, false);
    }

    void add(int u, int v) {
        g[u].push_back(v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in[u] = true;

        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in[v]) {
                // back edge: use dfn[v], not low[v]
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            ++cnt;
            while (true) {
                int x = stk.back();
                stk.pop_back();
                in[x] = false;
                bel[x] = cnt;
                if (x == u) break;
            }
        }
    }

    void work() {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        fill(in.begin(), in.end(), false);
        stk.clear();

        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                dfs(i);

        siz.assign(cnt + 1, 0);
        for (int u = 1; u <= n; ++u)
            ++siz[bel[u]];
    }

    vector<vector<int>> dag() const {
        vector<vector<int>> nodes(cnt + 1), res(cnt + 1);
        for (int u = 1; u <= n; ++u)
            nodes[bel[u]].push_back(u);

        vector<int> vis(cnt + 1, 0);

        for (int c = 1; c <= cnt; ++c) {
            for (int u : nodes[c]) {
                for (int v : g[u]) {
                    int d = bel[v];
                    if (c != d && vis[d] != c) {
                        vis[d] = c;
                        res[c].push_back(d);
                    }
                }
            }
        }

        return res;
    }
};
```

### 现场记忆

```text
dfn[u] = DFS 时间，low[u] = u 能回到的最小 dfn。

树边 u->v：
    dfs(v)
    low[u] = min(low[u], low[v])

v 仍在栈：
    low[u] = min(low[u], dfn[v])

low[u] == dfn[u] -> 弹栈得到一个 SCC。
Tarjan 弹出的 SCC 编号顺序是拓扑序的反序。
```

## 2.2 割点 / 桥

> **用途**：求无向图所有割点和桥。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **注意**：用边编号跳过父边，因此可以正确处理重边；递归 DFS 可能爆栈。

### 调用

```cpp
CutBridge cb(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    cb.add(u, v);
}

cb.work();

if (cb.is_cut[u]) {
    // u is a cut vertex
}

for (auto [u, v] : cb.bridges) {
    // (u, v) is a bridge
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u, v)` | 加无向边 |
| `work()` | 求割点和桥 |
| `is_cut[u]` | `u` 是否为割点 |
| `bridges` | 所有桥的两个端点 |

### 模板

```cpp
struct CutBridge {
    struct Edge {
        int to, id;
    };

    int n, edge_cnt, timer;
    vector<vector<Edge>> g;
    vector<int> dfn, low;
    vector<char> is_cut;
    vector<pair<int, int>> bridges;

    CutBridge(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edge_cnt = timer = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        is_cut.assign(n + 1, false);
        bridges.clear();
    }

    void add(int u, int v) {
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
        ++edge_cnt;
    }

    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++timer;
        int child = 0;

        for (auto [v, id] : g[u]) {
            if (!dfn[v]) {
                ++child;
                dfs(v, id);
                low[u] = min(low[u], low[v]);

                if (pe != -1 && low[v] >= dfn[u])
                    is_cut[u] = true;

                if (low[v] > dfn[u])
                    bridges.push_back({u, v});
            } else if (id != pe) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (pe == -1 && child >= 2)
            is_cut[u] = true;
    }

    void work() {
        timer = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_cut.begin(), is_cut.end(), false);
        bridges.clear();

        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                dfs(i, -1);
    }
};
```

### 现场记忆

```text
树边 u-v 后先：
    low[u] = min(low[u], low[v])

非根割点：
    low[v] >= dfn[u]

桥：
    low[v] > dfn[u]

DFS 根是割点 <=> DFS 树儿子数 >= 2。
无向图有重边时必须按边 id 跳父边，不能只写 v != fa。
```

## 2.3 边双连通分量 EBCC

> **用途**：求无向图边双连通分量，并可缩点得到桥树 / 桥森林。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`，边双编号 `1..cnt`。
> **注意**：断开的原图缩点后得到的是森林；递归 DFS 可能爆栈。

### 调用

```cpp
EBCC ebcc(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    ebcc.add(u, v);
}

ebcc.work();

cout << ebcc.cnt << '\n';
cout << ebcc.bel[u] << '\n';

auto tree = ebcc.bridge_tree();
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u, v)` | 加无向边 |
| `work()` | 求桥和边双 |
| `bridge_tree()` | 缩点建桥树 / 森林，需先 `work()` |
| `cnt` | 边双数量 |
| `bel[u]` | `u` 所属边双 |
| `siz[id]` | 边双大小 |
| `is_bridge[id]` | 第 `id` 条边是否为桥 |

### 模板

```cpp
struct EBCC {
    struct Edge {
        int to, id;
    };

    int n, edge_cnt, timer, cnt;
    vector<vector<Edge>> g;
    vector<int> dfn, low, bel, siz;
    vector<char> is_bridge;

    EBCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edge_cnt = timer = cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        bel.assign(n + 1, 0);
        siz.clear();
        is_bridge.clear();
    }

    void add(int u, int v) {
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
        ++edge_cnt;
    }

    void dfs_bridge(int u, int pe) {
        dfn[u] = low[u] = ++timer;

        for (auto [v, id] : g[u]) {
            if (!dfn[v]) {
                dfs_bridge(v, id);
                low[u] = min(low[u], low[v]);

                if (low[v] > dfn[u])
                    is_bridge[id] = true;
            } else if (id != pe) {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void dfs_comp(int u) {
        bel[u] = cnt;
        ++siz[cnt];

        for (auto [v, id] : g[u]) {
            if (!bel[v] && !is_bridge[id])
                dfs_comp(v);
        }
    }

    void work() {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        is_bridge.assign(edge_cnt, false);

        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                dfs_bridge(i, -1);

        siz.assign(n + 1, 0);

        for (int i = 1; i <= n; ++i) {
            if (!bel[i]) {
                ++cnt;
                dfs_comp(i);
            }
        }

        siz.resize(cnt + 1);
    }

    vector<vector<int>> bridge_tree() const {
        vector<vector<int>> tree(cnt + 1);

        for (int u = 1; u <= n; ++u) {
            for (auto [v, id] : g[u]) {
                if (bel[u] != bel[v])
                    tree[bel[u]].push_back(bel[v]);
            }
        }

        return tree;
    }
};
```

### 现场记忆

```text
先 Tarjan 找桥：
    low[v] > dfn[u] <=> DFS 树边 u-v 是桥。

删掉所有桥后：
    每个连通块就是一个边双。

重边必须按边 id 跳父边：
    两条平行边不会被误判成桥。

缩点后所有跨边双的边都是桥，因此天然得到桥树 / 桥森林。
```

## 2.4 点双连通分量 VBCC / 圆方树

> **用途**：求无向图点双连通分量和割点，并建立圆方树。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：原节点 `1..n`，点双编号 `1..cnt`；圆方树方点为 `n+1..n+cnt`。
> **限制**：默认无自环；支持重边。
> **注意**：一个割点可以属于多个点双，因此不存在单一的 `bel[u]`。

### 调用

```cpp
VBCC vbcc(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    vbcc.add(u, v);
}

vbcc.work();

for (int id = 1; id <= vbcc.cnt; ++id) {
    for (int u : vbcc.bcc[id]) {
        // u belongs to this VBCC
    }
}

auto tree = vbcc.block_tree();
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u, v)` | 加无向边 |
| `work()` | 求点双和割点 |
| `block_tree()` | 建圆方树 / 森林，需先 `work()` |
| `cnt` | 点双数量 |
| `bcc[id]` | 第 `id` 个点双包含的原节点 |
| `is_cut[u]` | `u` 是否为割点 |

### 模板

```cpp
struct VBCC {
    struct Edge {
        int to, id;
    };

    int n, edge_cnt, timer, cnt;
    vector<vector<Edge>> g;
    vector<int> dfn, low, stk;
    vector<vector<int>> bcc;
    vector<char> is_cut;

    VBCC(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        edge_cnt = timer = cnt = 0;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        stk.clear();
        bcc.assign(1, {});
        is_cut.assign(n + 1, false);
    }

    void add(int u, int v) {
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
        ++edge_cnt;
    }

    void dfs(int u, int pe) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        int child = 0;

        for (auto [v, id] : g[u]) {
            if (!dfn[v]) {
                ++child;
                dfs(v, id);
                low[u] = min(low[u], low[v]);

                if (low[v] >= dfn[u]) {
                    if (pe != -1)
                        is_cut[u] = true;

                    ++cnt;
                    bcc.push_back({u});

                    while (true) {
                        int x = stk.back();
                        stk.pop_back();
                        bcc[cnt].push_back(x);
                        if (x == v) break;
                    }
                }
            } else if (id != pe) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (pe == -1 && child >= 2)
            is_cut[u] = true;
    }

    void work() {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(is_cut.begin(), is_cut.end(), false);
        stk.clear();
        bcc.assign(1, {});

        for (int i = 1; i <= n; ++i) {
            if (dfn[i]) continue;

            if (g[i].empty()) {
                dfn[i] = low[i] = ++timer;
                ++cnt;
                bcc.push_back({i});
                continue;
            }

            dfs(i, -1);

            // DFS 根不会被子块弹出
            if (!stk.empty() && stk.back() == i)
                stk.pop_back();
        }
    }

    vector<vector<int>> block_tree() const {
        vector<vector<int>> tree(n + cnt + 1);

        for (int id = 1; id <= cnt; ++id) {
            int square = n + id;

            for (int u : bcc[id]) {
                tree[u].push_back(square);
                tree[square].push_back(u);
            }
        }

        return tree;
    }
};
```

### 现场记忆

```text
点栈：DFS 到 u 时把 u 入栈。

树边 u-v 返回后：
    low[u] = min(low[u], low[v])

low[v] >= dfn[u]：
    u 与栈顶一直弹到 v，组成一个点双；
    u 本身不弹，因为它可能属于后面的点双。

非根 u 出现上述条件 -> u 是割点。
DFS 根是割点 <=> DFS 树儿子数 >= 2。

圆方树：每个点双新建一个方点，并连接该点双中的所有原点。
```

## 2.5 2-SAT

> **用途**：判断只含二元布尔约束的系统是否可满足，并构造一组可行赋值。
> **复杂度**：设变量数为 `n`、蕴含边数为 `m`，时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：变量 `1..n`，`assignment[i]` 为 `0/1`。
> **注意**：递归 Tarjan 在蕴含图很深时可能爆栈。

### 调用

```cpp
TwoSAT sat(n);

// (x = true) OR (y = false)
sat.either(x, true, y, false);

// x = true
sat.force(x, true);

// x == y
sat.eq(x, y);

// x != y
sat.neq(x, y);

if (sat.satisfiable()) {
    cout << sat.assignment[x] << '\n';
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `either(x, vx, y, vy)` | `(x=vx) OR (y=vy)` |
| `force(x, v)` | 强制 `x=v` |
| `eq(x, y)` | `x == y` |
| `neq(x, y)` | `x != y` |
| `satisfiable()` | 判断是否有解并生成方案 |
| `assignment[x]` | 一组可行布尔赋值 |

### 模板

```cpp
struct TwoSAT {
    int n, timer, cnt;
    vector<vector<int>> g;
    vector<int> dfn, low, bel, stk;
    vector<char> in;
    vector<int> assignment;

    TwoSAT(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        timer = cnt = 0;
        g.assign(2 * n + 1, {});
        dfn.assign(2 * n + 1, 0);
        low.assign(2 * n + 1, 0);
        bel.assign(2 * n + 1, 0);
        stk.clear();
        in.assign(2 * n + 1, false);
        assignment.assign(n + 1, 0);
    }

    int id(int x, bool v) const {
        return 2 * x - (v ? 1 : 0);
    }

    int opp(int u) const {
        return (u & 1) ? u + 1 : u - 1;
    }

    void either(int x, bool vx, int y, bool vy) {
        int a = id(x, vx);
        int b = id(y, vy);

        g[opp(a)].push_back(b);
        g[opp(b)].push_back(a);
    }

    void force(int x, bool v) {
        either(x, v, x, v);
    }

    void eq(int x, int y) {
        either(x, true, y, false);
        either(x, false, y, true);
    }

    void neq(int x, int y) {
        either(x, true, y, true);
        either(x, false, y, false);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in[u] = true;

        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            ++cnt;

            while (true) {
                int x = stk.back();
                stk.pop_back();
                in[x] = false;
                bel[x] = cnt;
                if (x == u) break;
            }
        }
    }

    bool satisfiable() {
        timer = cnt = 0;
        fill(dfn.begin(), dfn.end(), 0);
        fill(low.begin(), low.end(), 0);
        fill(bel.begin(), bel.end(), 0);
        fill(in.begin(), in.end(), false);
        fill(assignment.begin(), assignment.end(), 0);
        stk.clear();

        for (int i = 1; i <= 2 * n; ++i)
            if (!dfn[i])
                dfs(i);

        for (int x = 1; x <= n; ++x) {
            int f = id(x, false);
            int t = id(x, true);

            if (bel[f] == bel[t])
                return false;

            assignment[x] = bel[t] < bel[f];
        }

        return true;
    }
};
```

### 现场记忆

```text
每个文字 a：
    (a OR b) => (!a -> b) + (!b -> a)

无解：
    x 与 !x 在同一个 SCC。

Tarjan SCC 按弹栈顺序编号，编号越小越靠近蕴含图汇点。
因此本模板：
    assignment[x] = bel[true] < bel[false]

force(x, v) 等价于：
    (x=v) OR (x=v)
```

## 2.6 链式前向星 Vector 版

> **用途**：用连续边数组维护邻接表，适合需要直接操作边编号或反向边的图算法。
> **复杂度**：加边 `O(1)`，遍历 `u` 的出边 `O(outdegree(u))`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`，边编号 `0-indexed`。

### 调用

```cpp
ForwardStar g(n, m);

int id = g.add(u, v, w);

for (int i = g.head[u]; i != -1; i = g.e[i].nxt) {
    int v = g.e[i].to;
    long long w = g.e[i].w;
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `init(n, m)` | 初始化，可预留 `m` 条边 |
| `add(u, v, w)` | 加边并返回边编号 |
| `head[u]` | `u` 第一条出边编号 |
| `e[i]` | 第 `i` 条边 |
| `e[i].to` | 终点 |
| `e[i].w` | 边权 |
| `e[i].nxt` | 下一条同起点边 |

### 模板

```cpp
struct ForwardStar {
    struct Edge {
        int to, nxt;
        long long w;
    };

    int n;
    vector<int> head;
    vector<Edge> e;

    ForwardStar(int n = 0, int m = 0) {
        init(n, m);
    }

    void init(int n_, int m = 0) {
        n = n_;
        head.assign(n + 1, -1);
        e.clear();
        if (m)
            e.reserve(m);
    }

    int add(int u, int v, long long w = 1) {
        int id = e.size();
        e.push_back({v, head[u], w});
        head[u] = id;
        return id;
    }
};
```

### 现场记忆

```text
遍历：
    for (int i = head[u]; i != -1; i = e[i].nxt)

新边插到链表头，因此遍历顺序与 add 顺序相反。

若无向边连续 add(u,v), add(v,u)：
    反向边编号 = i ^ 1
```


## 2.7 Dijkstra 最短路

> **用途**：求非负权图中单源最短路。
> **复杂度**：时间 `O((n+m) log n)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **限制**：所有边权必须非负。

### 调用

```cpp
Dijkstra dij(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    dij.add(u, v, w);
}

auto dis = dij.dijkstra(s);
```

### 模板

```cpp
struct Dijkstra {
    struct Edge {
        int to;
        long long w;
    };

    int n;
    vector<vector<Edge>> g;

    Dijkstra(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add(int u, int v, long long w) {
        g[u].push_back({v, w});
    }

    vector<long long> dijkstra(int s) const {
        const long long INF = 1LL << 62;

        vector<long long> dis(n + 1, INF);
        priority_queue<
            pair<long long, int>,
            vector<pair<long long, int>>,
            greater<pair<long long, int>>
        > pq;

        dis[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d != dis[u])
                continue;

            for (auto [v, w] : g[u]) {
                if (dis[v] > d + w) {
                    dis[v] = d + w;
                    pq.push({dis[v], v});
                }
            }
        }

        return dis;
    }
};
```

### 现场记忆

```text
小根堆存 (dis[u], u)。

弹出旧状态：
    if (d != dis[u]) continue;

松弛：
    if (dis[v] > dis[u] + w)

只适用于非负边权。
无向边手动 add(u,v,w) + add(v,u,w)。
```


## 2.8 SPFA 判负环

> **用途**：判断整张有向图中是否存在负权环。
> **复杂度**：最坏时间 `O(nm)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **注意**：所有点初始入队，等价于建立一个向所有点连 `0` 边的超级源，因此能检测任意连通块中的负环。

### 调用

```cpp
SPFA spfa(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    spfa.add(u, v, w);
}

if (spfa.neg_cycle()) {
    // negative cycle exists
}
```

### 模板

```cpp
struct SPFA {
    struct Edge {
        int to;
        long long w;
    };

    int n;
    vector<vector<Edge>> g;

    SPFA(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add(int u, int v, long long w) {
        g[u].push_back({v, w});
    }

    bool neg_cycle() const {
        vector<long long> dis(n + 1, 0);
        vector<int> cnt(n + 1, 0);
        vector<char> in(n + 1, true);
        queue<int> q;

        for (int i = 1; i <= n; ++i)
            q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in[u] = false;

            for (auto [v, w] : g[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    cnt[v] = cnt[u] + 1;

                    if (cnt[v] >= n)
                        return true;

                    if (!in[v]) {
                        q.push(v);
                        in[v] = true;
                    }
                }
            }
        }

        return false;
    }
};
```

### 现场记忆

```text
所有点初始 dis=0 并入队
<=> 超级源向所有点连权值 0 的边。

松弛 v 时：
    cnt[v] = cnt[u] + 1

若 cnt[v] >= n：
    当前更短路径至少经过 n 条边
    -> 必然经过负环。
```


## 2.9 Floyd：全源最短路 / 传递闭包

> **用途**：小规模图求全源最短路；或用 `bitset` 求传递闭包。
> **复杂度**：Floyd 时间 `O(n^3)`；传递闭包约 `O(n^3 / w)`；空间均为 `O(n^2)`。
> **下标**：节点 `1-indexed`。
> **注意**：普通最短路允许负边，但若存在负环则最短路无定义；Floyd 后 `dis[i][i] < 0` 表示存在相关负环。

### 模板

```cpp
void floyd(vector<vector<long long>>& dis) {
    int n = dis.size() - 1;
    const long long INF = 1LL << 62;

    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            if (dis[i][k] == INF)
                continue;

            for (int j = 1; j <= n; ++j) {
                if (dis[k][j] == INF)
                    continue;

                dis[i][j] = min(
                    dis[i][j],
                    dis[i][k] + dis[k][j]
                );
            }
        }
    }
}

template <size_t N>
void closure(vector<bitset<N>>& reach, int n) {
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            if (reach[i][k])
                reach[i] |= reach[k];
}
```

### 现场记忆

```text
Floyd：
    k 必须放最外层。
    dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j])

初始化：
    dis[i][i] = 0
    重边取 min

bitset 闭包：
    reach[i][k] -> reach[i] |= reach[k]

若要求“自己可达自己”，先设 reach[i][i] = 1。
```

## 2.10 差分约束

> **用途**：判断一组形如 `x_v - x_u <= w` 的差分约束是否有解，并构造一组可行解。
> **复杂度**：SPFA 最坏时间 `O(nm)`，空间 `O(n+m)`。
> **下标**：变量 `1-indexed`。
> **注意**：所有变量初值为 `0` 等价于加入超级源，可检测任意负环。

### 调用

```cpp
DiffCons dc(n);

// x_v - x_u <= w
dc.leq(u, v, w);

auto [ok, x] = dc.solve();

if (ok) {
    cout << x[i] << '\n';
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `leq(u, v, w)` | 加入 `x_v - x_u <= w` |
| `solve()` | 返回 `{是否有解, 一组可行解}` |

### 模板

```cpp
struct DiffCons {
    struct Edge {
        int to;
        long long w;
    };

    int n;
    vector<vector<Edge>> g;

    DiffCons(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    // x_v - x_u <= w  <=>  x_v <= x_u + w
    void leq(int u, int v, long long w) {
        g[u].push_back({v, w});
    }

    pair<bool, vector<long long>> solve() const {
        vector<long long> dis(n + 1, 0);
        vector<int> cnt(n + 1, 0);
        vector<char> in(n + 1, true);
        queue<int> q;

        for (int i = 1; i <= n; ++i)
            q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in[u] = false;

            for (auto [v, w] : g[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    cnt[v] = cnt[u] + 1;

                    if (cnt[v] >= n)
                        return {false, {}};

                    if (!in[v]) {
                        q.push(v);
                        in[v] = true;
                    }
                }
            }
        }

        return {true, dis};
    }
};
```

### 现场记忆

```text
核心只记：
    x_v - x_u <= w
<=> x_v <= x_u + w
<=> u -> v，边权 w。

x_v - x_u >= w：
    x_u - x_v <= -w
    -> v -> u，边权 -w。

存在负环 <=> 约束矛盾。

返回的 dis 就是一组可行势能；
所有变量同时加同一个常数仍然是解。
```

## 2.11 Kruskal 最小生成树

> **用途**：求无向带权图的最小生成树。
> **复杂度**：时间 `O(m log m)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **注意**：图不连通时返回 `{false, ...}`。

### 调用

```cpp
Kruskal mst(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    mst.add(u, v, w);
}

auto [ok, ans] = mst.work();
```

### 模板

```cpp
struct Kruskal {
    struct Edge {
        int u, v;
        long long w;
    };

    struct DSU {
        vector<int> fa, siz;

        DSU(int n = 0) {
            init(n);
        }

        void init(int n) {
            fa.resize(n + 1);
            siz.assign(n + 1, 1);
            iota(fa.begin(), fa.end(), 0);
        }

        int find(int x) {
            return fa[x] == x ? x : fa[x] = find(fa[x]);
        }

        bool merge(int x, int y) {
            x = find(x);
            y = find(y);

            if (x == y)
                return false;

            if (siz[x] < siz[y])
                swap(x, y);

            fa[y] = x;
            siz[x] += siz[y];
            return true;
        }
    };

    int n;
    vector<Edge> e;

    Kruskal(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        e.clear();
    }

    void add(int u, int v, long long w) {
        e.push_back({u, v, w});
    }

    pair<bool, long long> work() {
        sort(e.begin(), e.end(), [](const Edge& a, const Edge& b) {
            return a.w < b.w;
        });

        DSU dsu(n);
        int cnt = 0;
        long long ans = 0;

        for (auto [u, v, w] : e) {
            if (dsu.merge(u, v)) {
                ans += w;
                if (++cnt == n - 1)
                    break;
            }
        }

        return {cnt == n - 1, ans};
    }
};
```

### 现场记忆

```text
边按权从小到大。

若 u,v 已连通：
    加这条边会成环 -> 跳过。

否则：
    merge(u,v)
    ans += w

选满 n-1 条边即得到 MST。
```

## 2.12 Prim 最小生成树

> **用途**：用邻接表求无向带权图的最小生成树，适合直接从图上跑 MST。
> **复杂度**：时间 `O(m log n)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **注意**：图不连通时返回 `{false, ...}`；允许负边权。

### 调用

```cpp
Prim mst(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    mst.add(u, v, w);
}

auto [ok, ans] = mst.work();
```

### 模板

```cpp
struct Prim {
    struct Edge {
        int to;
        long long w;
    };

    int n;
    vector<vector<Edge>> g;

    Prim(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add(int u, int v, long long w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    pair<bool, long long> work(int s = 1) const {
        const long long INF = 1LL << 62;

        vector<long long> best(n + 1, INF);
        vector<char> vis(n + 1, false);

        priority_queue<
            pair<long long, int>,
            vector<pair<long long, int>>,
            greater<pair<long long, int>>
        > pq;

        best[s] = 0;
        pq.push({0, s});

        int cnt = 0;
        long long ans = 0;

        while (!pq.empty()) {
            auto [w, u] = pq.top();
            pq.pop();

            if (vis[u])
                continue;

            vis[u] = true;
            ++cnt;
            ans += w;

            for (auto [v, cost] : g[u]) {
                if (!vis[v] && best[v] > cost) {
                    best[v] = cost;
                    pq.push({cost, v});
                }
            }
        }

        return {cnt == n, ans};
    }
};
```

### 现场记忆

```text
best[v] 不是最短路距离，
而是“当前生成树连到 v 的最小边权”。

每次取最小 best[u] 的未选点：
    ans += best[u]

再用 u 的边更新其他未选点。
```

## 2.13 并查集 DSU

> **用途**：维护集合合并、连通性和集合大小。
> **复杂度**：均摊时间 `O(α(n))`，空间 `O(n)`。
> **下标**：节点 `1-indexed`。

### 调用

```cpp
DSU dsu(n);

dsu.merge(x, y);

if (dsu.same(x, y)) {
    cout << dsu.size(x) << '\n';
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `find(x)` | 查询代表元 |
| `merge(x, y)` | 合并，已连通返回 `false` |
| `same(x, y)` | 是否属于同一集合 |
| `size(x)` | `x` 所在集合大小 |
| `fa[x]` | 父节点 |
| `siz[root]` | 集合大小 |

### 模板

```cpp
struct DSU {
    vector<int> fa, siz;

    DSU(int n = 0) {
        init(n);
    }

    void init(int n) {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);

        if (x == y)
            return false;

        if (siz[x] < siz[y])
            swap(x, y);

        fa[y] = x;
        siz[x] += siz[y];
        return true;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int size(int x) {
        return siz[find(x)];
    }
};
```

### 现场记忆

```text
find：路径压缩。
merge：小集合挂到大集合。

只有根的 siz 有意义。
```

## 2.14 可撤销并查集 Rollback DSU

> **用途**：支持撤销集合合并，常用于线段树分治、离线动态连通性。
> **复杂度**：`find/merge` 为 `O(log n)`，单次撤销 `O(1)`；空间 `O(n+q)`。
> **下标**：节点 `1-indexed`。
> **注意**：不能路径压缩，只能按大小 / 按秩合并。

### 调用

```cpp
RollbackDSU dsu(n);

int snap = dsu.snapshot();

dsu.merge(x, y);
dsu.merge(a, b);

dsu.rollback(snap);
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `merge(x, y)` | 合并两个集合 |
| `same(x, y)` | 判断连通 |
| `size(x)` | 集合大小 |
| `snapshot()` | 保存当前状态编号 |
| `rollback(snap)` | 回滚到指定状态 |
| `undo()` | 撤销最近一次 `merge` |

### 模板

```cpp
struct RollbackDSU {
    struct Change {
        int x, y, sy;
    };

    vector<int> fa, siz;
    vector<Change> stk;

    RollbackDSU(int n = 0) {
        init(n);
    }

    void init(int n) {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        iota(fa.begin(), fa.end(), 0);
        stk.clear();
    }

    int find(int x) const {
        while (fa[x] != x)
            x = fa[x];
        return x;
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);

        if (x == y) {
            stk.push_back({-1, -1, 0});
            return false;
        }

        if (siz[x] > siz[y])
            swap(x, y);

        stk.push_back({x, y, siz[y]});

        fa[x] = y;
        siz[y] += siz[x];
        return true;
    }

    void undo() {
        auto [x, y, sy] = stk.back();
        stk.pop_back();

        if (x == -1)
            return;

        fa[x] = x;
        siz[y] = sy;
    }

    int snapshot() const {
        return stk.size();
    }

    void rollback(int snap) {
        while ((int)stk.size() > snap)
            undo();
    }

    bool same(int x, int y) const {
        return find(x) == find(y);
    }

    int size(int x) const {
        return siz[find(x)];
    }
};
```

### 现场记忆

```text
绝对不能路径压缩。

只允许：
    小树 -> 大树
因此树高 O(log n)。

每次 merge 都压一个历史记录；
即使没有真正合并也压空记录，
保证一次 merge 对应一次 undo。
```

## 2.15 可持久化并查集 Persistent DSU

> **用途**：保留并查集的所有历史版本，可从任意旧版本继续合并或查询。
> **复杂度**：`find/merge/same` 时间 `O(log^2 n)`；`q` 次修改空间 `O(n+q log n)`。
> **下标**：节点 `1-indexed`。
> **注意**：不能路径压缩；依靠按秩合并保证并查集树高 `O(log n)`。

### 调用

```cpp
PDSU dsu(n);

vector<int> root(q + 1);
root[0] = dsu.base;

// 基于上一版本合并
root[i] = dsu.merge(root[i - 1], x, y);

// 回到第 k 个版本
root[i] = root[k];

// 查询当前版本
if (dsu.same(root[i], x, y)) {
    // connected
}
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `base` | 初始版本根 |
| `find(root, x)` | 在指定版本查询代表元 |
| `merge(root, x, y)` | 返回合并后的新版本根 |
| `same(root, x, y)` | 指定版本中是否连通 |

### 模板

```cpp
struct PDSU {
    struct Node {
        int l, r;
        int fa, rk;
    };

    int n, base;
    vector<Node> tr;

    PDSU(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        tr.clear();
        tr.push_back({});
        base = n ? build(1, n) : 0;
    }

    int new_node() {
        tr.push_back({});
        return tr.size() - 1;
    }

    int new_node(const Node& x) {
        tr.push_back(x);
        return tr.size() - 1;
    }

    int build(int l, int r) {
        int u = new_node();

        if (l == r) {
            tr[u].fa = l;
            return u;
        }

        int mid = (l + r) >> 1;
        tr[u].l = build(l, mid);
        tr[u].r = build(mid + 1, r);
        return u;
    }

    int get(int u, int l, int r, int x) const {
        if (l == r)
            return u;

        int mid = (l + r) >> 1;

        if (x <= mid)
            return get(tr[u].l, l, mid, x);
        else
            return get(tr[u].r, mid + 1, r, x);
    }

    int modify(int pre, int l, int r, int x, int fa, int rk) {
        int u = new_node(tr[pre]);

        if (l == r) {
            if (fa != -1)
                tr[u].fa = fa;
            if (rk != -1)
                tr[u].rk = rk;
            return u;
        }

        int mid = (l + r) >> 1;

        if (x <= mid)
            tr[u].l = modify(tr[pre].l, l, mid, x, fa, rk);
        else
            tr[u].r = modify(tr[pre].r, mid + 1, r, x, fa, rk);

        return u;
    }

    int find(int root, int x) const {
        while (true) {
            int u = get(root, 1, n, x);

            if (tr[u].fa == x)
                return x;

            x = tr[u].fa;
        }
    }

    int merge(int root, int x, int y) {
        x = find(root, x);
        y = find(root, y);

        if (x == y)
            return root;

        int px = get(root, 1, n, x);
        int py = get(root, 1, n, y);

        if (tr[px].rk > tr[py].rk) {
            swap(x, y);
            swap(px, py);
        }

        int res = modify(root, 1, n, x, y, -1);

        if (tr[px].rk == tr[py].rk)
            res = modify(res, 1, n, y, -1, tr[py].rk + 1);

        return res;
    }

    bool same(int root, int x, int y) const {
        return find(root, x) == find(root, y);
    }
};
```

### 现场记忆

```text
线段树维护每个点当前版本的：
    fa、rank。

每次只修改一个根的 fa，
必要时再修改另一个根的 rank，
因此产生 O(log n) 个新节点。

不能路径压缩。
按秩合并 -> DSU 深度 O(log n)；
每找一层父亲要在线段树 query 一次
-> find O(log^2 n)。

版本本质就是一棵线段树的 root。
```

## 2.16 带权并查集

> **用途**：维护同一连通块内点之间的相对关系，可用于差值、奇偶 / XOR、模 `k` 关系等。
> **复杂度**：均摊时间 `O(α(n))`，空间 `O(n)`。
> **下标**：节点 `1-indexed`。
> **核心**：普通并查集结构完全相同，只需根据关系类型替换 `dis` 的维护公式。

### 通用结构

以下以加法差分为基准：

```cpp
struct WeightedDSU {
    vector<int> fa, siz;
    vector<long long> dis;

    WeightedDSU(int n = 0) {
        init(n);
    }

    void init(int n) {
        fa.resize(n + 1);
        siz.assign(n + 1, 1);
        dis.assign(n + 1, 0);
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x) {
        if (fa[x] == x)
            return x;

        int f = fa[x];
        fa[x] = find(f);
        dis[x] += dis[f]; // 权值维护 ①

        return fa[x];
    }

    // 加法版：val[y] - val[x] = w
    bool merge(int x, int y, long long w) {
        int fx = find(x);
        int fy = find(y);

        if (fx == fy)
            return dis[x] - dis[y] == w; // 权值维护 ②

        if (siz[fx] > siz[fy]) {
            swap(x, y);
            swap(fx, fy);
            w = -w; // XOR 不需要；mod k 取 -w
        }

        fa[fx] = fy;
        dis[fx] = dis[y] - dis[x] + w; // 权值维护 ③
        siz[fy] += siz[fx];

        return true;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    long long diff(int x, int y) {
        find(x);
        find(y);
        return dis[x] - dis[y]; // 权值维护 ④
    }
};
```

### 权值维护速查

| 类型 | `dis[x]` 定义 | 路径压缩 | `fx -> fy` | 查询 |
|---|---|---|---|---|
| 加法 | `val[fa[x]] - val[x]` | `dis[x] += dis[f]` | `dis[fx] = dis[y] - dis[x] + w` | `val[y]-val[x] = dis[x]-dis[y]` |
| XOR | `val[x] ^ val[fa[x]]` | `dis[x] ^= dis[f]` | `dis[fx] = dis[x] ^ dis[y] ^ w` | `val[x]^val[y] = dis[x]^dis[y]` |
| mod `k` | `(val[fa[x]]-val[x]) mod k` | `dis[x] = norm(dis[x]+dis[f])` | `dis[fx] = norm(dis[y]-dis[x]+w)` | `val[y]-val[x] = norm(dis[x]-dis[y])` |

### 加法差分

```text
定义：
    dis[x] = val[fa[x]] - val[x]

约束：
    val[y] - val[x] = w

find：
    dis[x] += dis[f]

fx -> fy：
    dis[fx] = dis[y] - dis[x] + w

已有关系检查：
    dis[x] - dis[y] == w

查询：
    val[y] - val[x] = dis[x] - dis[y]

若交换 x,y：
    w = -w
```

### XOR / 奇偶

```text
定义：
    dis[x] = val[x] ^ val[fa[x]]

约束：
    val[x] ^ val[y] = w

find：
    dis[x] ^= dis[f]

fx -> fy：
    dis[fx] = dis[x] ^ dis[y] ^ w

已有关系检查：
    (dis[x] ^ dis[y]) == w

查询：
    val[x] ^ val[y] = dis[x] ^ dis[y]

交换 x,y 不影响 w。

二分图：
    w = 1
```

### mod k

```text
定义：
    dis[x] = val[fa[x]] - val[x] (mod k)

约束：
    val[y] - val[x] = w (mod k)

find：
    dis[x] = norm(dis[x] + dis[f])

fx -> fy：
    dis[fx] = norm(dis[y] - dis[x] + w)

已有关系检查：
    norm(dis[x] - dis[y]) == norm(w)

查询：
    val[y] - val[x] = norm(dis[x] - dis[y])

交换 x,y：
    w = norm(-w)

其中：
    norm(x) = (x % k + k) % k
```

### 现场记忆

```text
带权 DSU = 普通 DSU + 到父亲的关系 dis。

find：
    父亲变成根
    -> 把父链上的关系合并起来。

merge：
    一个根挂到另一个根
    -> 用 x、y 的已知关系反推两个根之间的关系。

真正需要重新推的只有四处：
    1. dis 的定义
    2. find 如何合并权值
    3. merge 如何反推根权值
    4. diff 如何由两个 dis 得到答案

可撤销带权 DSU：
    不做路径压缩，
    权值变化只记录在 merge 中。
```


## 2.17 拓展域并查集

> **用途**：把“同类 / 异类”等有限分类关系拆成多个并查集节点，用普通 DSU 维护关系与矛盾。
> **复杂度**：每次关系需要常数次 DSU 操作，均摊 `O(α(n))`。
> **下标**：原节点 `1..n`，第 `t` 个域可写为 `x + t*n`。

### 二域：同类 / 异类

```cpp
DSU dsu(2 * n);

auto opp = [&](int x) {
    return x + n;
};

// x 与 y 同类
if (dsu.same(x, y + n)) {
    // contradiction
}
dsu.merge(x, y);
dsu.merge(x + n, y + n);

// x 与 y 异类
if (dsu.same(x, y)) {
    // contradiction
}
dsu.merge(x, y + n);
dsu.merge(x + n, y);
```

### 现场记忆

```text
x：
    本域 x
    对立域 x+n

同类：
    x <-> y
    x+n <-> y+n

异类：
    x <-> y+n
    x+n <-> y

加关系前先检查“相反关系是否已经成立”。

k 个拓展域时先明确每个域的语义，
不要只凭下标机械套公式。
```

## 2.18 并查集虚点 / 参考点

> **用途**：把 `val[x]=w` 这类绝对值约束转成并查集维护的相对关系约束。
> **核心**：新增虚点 `S` 并规定 `val[S]=0`，所有绝对值都改写为与 `S` 的关系。
> **注意**：虚点不需要、也不保证始终是并查集的根。

### 加法差分

若带权并查集约定：

```cpp
merge(x, y, w);
// val[y] - val[x] = w
```

则：

```cpp
int S = n + 1;
WeightedDSU dsu(n + 1);

// val[x] = w
dsu.merge(S, x, w);

// 查询 val[x]
if (dsu.same(S, x)) {
    long long value = dsu.diff(S, x);
}
```

### XOR

```text
val[S] = 0

val[x] = w
<=> val[S] XOR val[x] = w
```

因此可加入：

```cpp
dsu.merge(S, x, w);
```

### mod k

```text
val[S] = 0

val[x] ≡ w (mod k)
<=> val[x] - val[S] ≡ w (mod k)
```

同样加入：

```cpp
dsu.merge(S, x, w);
```

### 现场记忆

```text
绝对值约束
    val[x] = w

建立参考点 S：
    val[S] = 0

转成：
    x 与 S 的相对关系。

按本套加法 DSU 的方向：
    merge(S, x, w)
    diff(S, x) = val[x]

不要假设 S 是根。
永远通过 diff / find 后的相对关系查询。
```

## 2.19 拓扑排序

> **用途**：求有向图的一个拓扑序，并判断是否存在有向环。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。

### 调用

```cpp
Topo topo(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    topo.add(u, v);
}

auto order = topo.work();

if (order.empty()) {
    // 有环
}
```

### 模板

```cpp
struct Topo {
    int n;
    vector<vector<int>> g;
    vector<int> indeg;

    Topo(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        indeg.assign(n + 1, 0);
    }

    void add(int u, int v) {
        g[u].push_back(v);
        ++indeg[v];
    }

    vector<int> work() const {
        vector<int> deg = indeg;
        vector<int> order;
        queue<int> q;

        for (int i = 1; i <= n; ++i)
            if (deg[i] == 0)
                q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            order.push_back(u);

            for (int v : g[u])
                if (--deg[v] == 0)
                    q.push(v);
        }

        if ((int)order.size() != n)
            return {};

        return order;
    }
};
```

### 现场记忆

```text
Kahn：

所有 indeg=0 的点入队。

每删除 u：
    for v in g[u]:
        --indeg[v]
        若变成 0 -> 入队

最终取出 n 个点 <=> 无环。

要字典序最小：
    queue 换成 priority_queue<int, vector<int>, greater<int>>。
```

## 2.20 欧拉路径 / 欧拉回路：有向图

> **用途**：求经过每条有向边恰好一次的欧拉路径 / 欧拉回路。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **注意**：返回点序列，长度应为 `m+1`；若不存在则返回空数组。

### 调用

```cpp
EulerDir euler(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    euler.add(u, v);
}

auto path = euler.work();

if (path.empty()) {
    // 不存在
}
```

### 模板

```cpp
struct EulerDir {
    int n, m;
    vector<vector<int>> g;
    vector<int> indeg, outdeg;

    EulerDir(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        m = 0;
        g.assign(n + 1, {});
        indeg.assign(n + 1, 0);
        outdeg.assign(n + 1, 0);
    }

    void add(int u, int v) {
        g[u].push_back(v);
        ++outdeg[u];
        ++indeg[v];
        ++m;
    }

    int get_start() const {
        int plus = 0, minus = 0;
        int start = -1, any = -1;

        for (int i = 1; i <= n; ++i) {
            if (outdeg[i])
                any = i;

            int d = outdeg[i] - indeg[i];

            if (d == 1) {
                ++plus;
                start = i;
            } else if (d == -1) {
                ++minus;
            } else if (d != 0) {
                return -1;
            }
        }

        if (plus == 1 && minus == 1)
            return start;

        if (plus == 0 && minus == 0)
            return any == -1 ? 1 : any;

        return -1;
    }

    vector<int> work() const {
        int start = get_start();

        if (start == -1)
            return {};

        vector<int> cur(n + 1, 0);
        vector<int> stk = {start};
        vector<int> path;

        while (!stk.empty()) {
            int u = stk.back();

            if (cur[u] < (int)g[u].size()) {
                stk.push_back(g[u][cur[u]++]);
            } else {
                path.push_back(u);
                stk.pop_back();
            }
        }

        if ((int)path.size() != m + 1)
            return {};

        reverse(path.begin(), path.end());
        return path;
    }
};
```

### 现场记忆

```text
有向欧拉路径：

起点：
    out - in = 1

终点：
    in - out = 1

其余：
    in = out

欧拉回路：
    所有点 in = out
    从任意有出边点开始。

Hierholzer：
    能走边就继续走；
    无边可走时把点加入答案；
    最后 reverse。

度数条件不保证边都连在一起，
所以最后必须检查 path.size() == m+1。
```

## 2.21 欧拉路径 / 欧拉回路：无向图

> **用途**：求经过每条无向边恰好一次的欧拉路径 / 欧拉回路。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`。
> **下标**：节点 `1-indexed`，边编号 `0-indexed`。
> **注意**：支持重边、自环；若不存在则返回空数组。

### 调用

```cpp
EulerUndir euler(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    euler.add(u, v);
}

auto path = euler.work();

if (path.empty()) {
    // 不存在
}
```

### 模板

```cpp
struct EulerUndir {
    struct Edge {
        int to, id;
    };

    int n, m;
    vector<vector<Edge>> g;
    vector<int> deg;

    EulerUndir(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        m = 0;
        g.assign(n + 1, {});
        deg.assign(n + 1, 0);
    }

    void add(int u, int v) {
        g[u].push_back({v, m});
        g[v].push_back({u, m});
        ++deg[u];
        ++deg[v];
        ++m;
    }

    int get_start() const {
        int odd = 0;
        int start = -1, any = -1;

        for (int i = 1; i <= n; ++i) {
            if (deg[i])
                any = i;

            if (deg[i] & 1) {
                ++odd;
                start = i;
            }
        }

        if (odd == 2)
            return start;

        if (odd == 0)
            return any == -1 ? 1 : any;

        return -1;
    }

    vector<int> work() const {
        int start = get_start();

        if (start == -1)
            return {};

        vector<int> cur(n + 1, 0);
        vector<char> used(m, false);
        vector<int> stk = {start};
        vector<int> path;

        while (!stk.empty()) {
            int u = stk.back();

            while (
                cur[u] < (int)g[u].size() &&
                used[g[u][cur[u]].id]
            ) {
                ++cur[u];
            }

            if (cur[u] == (int)g[u].size()) {
                path.push_back(u);
                stk.pop_back();
            } else {
                auto [v, id] = g[u][cur[u]++];
                used[id] = true;
                stk.push_back(v);
            }
        }

        if ((int)path.size() != m + 1)
            return {};

        reverse(path.begin(), path.end());
        return path;
    }
};
```

### 现场记忆

```text
无向图：

奇度点 = 2：
    从任意奇度点开始 -> 欧拉路径。

奇度点 = 0：
    从任意有边点开始 -> 欧拉回路。

其他情况：
    不存在。

一条无向边存两次，
必须用同一个 id + used[id] 防止走两遍。

度数条件还不够：
    最后检查 path.size() == m+1。
```


## 2.22 线段树优化建图

> **用途**：大量维护点到区间、区间到点、区间到区间的有向边，常配合最短路 / 可达性。
> **复杂度**：建图 `O(n)`；`p2r / r2p` 每次 `O(log n)` 条边；`r2r` 每次新增 `1` 个点和 `O(log n)` 条边。
> **下标**：原图节点 `1..n`，辅助节点 `n+1..tot`。
> **注意**：叶节点直接复用原图节点，不额外建立两套叶子。

### 调用

```cpp
SegGraph sg(n);

// 普通点 -> 点
sg.p2p(u, v, w);

// u -> [L,R] 中每个点，边权都是 w
sg.p2r(u, L, R, w);

// [L,R] 中每个点 -> v，边权都是 w
sg.r2p(L, R, v, w);

// [L1,R1] 中任意点 -> [L2,R2] 中任意点，边权都是 w
sg.r2r(L1, R1, L2, R2, w);

// 最终图就是：
sg.g;      // 邻接表
sg.tot;    // 实际总节点数

// 跑最短路 / SCC / 可达性时都要跑 1..sg.tot。
// 真正题目中的点仍然只有 1..n。
```

### 接口速查

| 接口 | 实际加入的关系 |
|---|---|
| `p2p(u,v,w)` | `u -> v (w)` |
| `p2r(u,L,R,w)` | `u -> 每个 x∈[L,R] (w)` |
| `r2p(L,R,v,w)` | `每个 x∈[L,R] -> v (w)` |
| `r2r(L1,R1,L2,R2,w)` | `任意 x∈[L1,R1] -> 任意 y∈[L2,R2] (w)` |

### 模板

```cpp
struct SegGraph {
    struct Edge {
        int to;
        long long w;
    };

    int n, tot;
    vector<vector<Edge>> g;
    vector<int> out_id, in_id;

    SegGraph(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        tot = n;

        g.assign(n + 1, {});
        out_id.assign(4 * n + 5, 0);
        in_id.assign(4 * n + 5, 0);

        if (n)
            build(1, 1, n);
    }

    void add(int u, int v, long long w) {
        g[u].push_back({v, w});
    }

    void p2p(int u, int v, long long w) {
        add(u, v, w);
    }

    void p2r(int u, int L, int R, long long w) {
        link_p2r(1, 1, n, L, R, u, w);
    }

    void r2p(int L, int R, int v, long long w) {
        link_r2p(1, 1, n, L, R, v, w);
    }

    void r2r(
        int L1, int R1,
        int L2, int R2,
        long long w
    ) {
        int mid = newnode();

        r2p(L1, R1, mid, w);
        p2r(mid, L2, R2, 0);
    }

private:
    int newnode() {
        ++tot;
        g.emplace_back();
        return tot;
    }

    void build(int id, int l, int r) {
        if (l == r) {
            out_id[id] = in_id[id] = l;
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        out_id[id] = newnode();
        in_id[id] = newnode();

        // out: auxiliary -> original points
        add(out_id[id], out_id[id << 1], 0);
        add(out_id[id], out_id[id << 1 | 1], 0);

        // in: original points -> auxiliary
        add(in_id[id << 1], in_id[id], 0);
        add(in_id[id << 1 | 1], in_id[id], 0);
    }

    void link_p2r(
        int id, int l, int r,
        int L, int R,
        int u, long long w
    ) {
        if (L <= l && r <= R) {
            add(u, out_id[id], w);
            return;
        }

        int mid = (l + r) >> 1;

        if (L <= mid)
            link_p2r(id << 1, l, mid, L, R, u, w);

        if (R > mid)
            link_p2r(id << 1 | 1, mid + 1, r, L, R, u, w);
    }

    void link_r2p(
        int id, int l, int r,
        int L, int R,
        int v, long long w
    ) {
        if (L <= l && r <= R) {
            add(in_id[id], v, w);
            return;
        }

        int mid = (l + r) >> 1;

        if (L <= mid)
            link_r2p(id << 1, l, mid, L, R, v, w);

        if (R > mid)
            link_r2p(id << 1 | 1, mid + 1, r, L, R, v, w);
    }
};
```

### 现场记忆

```text
只记两棵树的箭头：

out：
        区间辅助点
          /    \
         ↓      ↓
       子区间  子区间
          ↓
        原始点

所以：
    u -> out[区间]
就是：
    u -> 整个区间


in：
        区间辅助点
          ↑    ↑
       子区间  子区间
          ↑
        原始点

所以：
    in[区间] -> v
就是：
    整个区间 -> v


点 -> 区间：
    接 out

区间 -> 点：
    接 in


区间 A -> 区间 B：

    A
    ↓  via in
   mid
    ↓  via out
    B

具体：
    r2p(A, mid, w)
    p2r(mid, B, 0)

这样是 O(log n)，
而不是把 A 的 O(log n) 个节点
和 B 的 O(log n) 个节点两两连接成 O(log² n)。


跑算法时：
    图的节点数 = tot
    原题答案只看 1..n。
```


## 2.23 倍增优化树上路径建图

> **用途**：大量维护“点到树上路径”或“树上路径到点”的依赖边，常配合拓扑排序 / 可达性。
> **复杂度**：预处理 `O(n log n)`；`p2path / path2p` 每次 `O(log n)` 条边；辅助图规模 `O(n log n)`。
> **下标**：原树节点 `1..n`，辅助节点 `n+1..tot`。
> **注意**：`p2path / path2p` 默认包含路径两个端点；若源点 / 目标点本身在路径中，可能产生自环。

### 调用

```cpp
JumpGraph jg(n);

// 先建立原树
for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    jg.add_tree(u, v);
}

// 建倍增辅助图
jg.build(1);

// 普通依赖：u 必须在 v 前
jg.p2p(u, v);

// s 必须在树路径 u-v 上所有点之前
jg.p2path(s, u, v);

// 树路径 u-v 上所有点都必须在 t 前
jg.path2p(u, v, t);

// 判断依赖是否有环，并得到原节点拓扑序
auto order = jg.topo();

if (order.empty()) {
    // dependency cycle
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `add_tree(u,v)` | 加原树无向边 |
| `build(root)` | 建倍增和辅助图，之后再加依赖 |
| `p2p(u,v)` | `u -> v` |
| `p2path(s,u,v)` | `s -> path(u,v)` 上所有点 |
| `path2p(u,v,t)` | `path(u,v)` 上所有点 `-> t` |
| `kth(u,k)` | `u` 的第 `k` 个祖先 |
| `lca(u,v)` | 最近公共祖先 |
| `next_on_path(u,v)` | 从 `u` 朝 `v` 走一步 |
| `topo()` | 检查整个辅助图并返回原节点拓扑序 |

### 模板

```cpp
struct JumpGraph {
    int n, LOG, tot;

    vector<vector<int>> tree;
    vector<int> dep;

    vector<vector<int>> fa;
    vector<vector<int>> out_id, in_id;

    vector<vector<int>> g;
    vector<int> indeg;

    JumpGraph(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        LOG = 0;
        tot = n;

        tree.assign(n + 1, {});
        dep.clear();
        fa.clear();
        out_id.clear();
        in_id.clear();

        g.clear();
        indeg.clear();
    }

    void add_tree(int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    void build(int root = 1) {
        LOG = 1;
        while ((1LL << LOG) <= n)
            ++LOG;

        dep.assign(n + 1, 0);
        fa.assign(LOG, vector<int>(n + 1, 0));

        vector<int> stk = {root};
        dep[root] = 1;

        while (!stk.empty()) {
            int u = stk.back();
            stk.pop_back();

            for (int v : tree[u]) {
                if (v == fa[0][u])
                    continue;

                fa[0][v] = u;
                dep[v] = dep[u] + 1;
                stk.push_back(v);
            }
        }

        for (int k = 1; k < LOG; ++k)
            for (int u = 1; u <= n; ++u)
                fa[k][u] = fa[k - 1][fa[k - 1][u]];

        tot = n;
        g.assign(n + 1, {});
        indeg.assign(n + 1, 0);

        out_id.assign(LOG, vector<int>(n + 1, 0));
        in_id.assign(LOG, vector<int>(n + 1, 0));

        for (int u = 1; u <= n; ++u)
            out_id[0][u] = in_id[0][u] = u;

        for (int k = 1; k < LOG; ++k) {
            for (int u = 1; u <= n; ++u) {
                if ((1LL << k) > dep[u])
                    continue;

                int mid = fa[k - 1][u];

                out_id[k][u] = newnode();
                in_id[k][u] = newnode();

                // out: segment -> original points
                add(out_id[k][u], out_id[k - 1][u]);
                add(out_id[k][u], out_id[k - 1][mid]);

                // in: original points -> segment
                add(in_id[k - 1][u], in_id[k][u]);
                add(in_id[k - 1][mid], in_id[k][u]);
            }
        }
    }

    void p2p(int u, int v) {
        add(u, v);
    }

    void p2path(int s, int u, int v) {
        int L = lca(u, v);

        add_p2anc(s, u, L);

        if (v != L) {
            int son = kth(v, dep[v] - dep[L] - 1);
            add_p2anc(s, v, son);
        }
    }

    void path2p(int u, int v, int t) {
        int L = lca(u, v);

        add_anc2p(u, L, t);

        if (v != L) {
            int son = kth(v, dep[v] - dep[L] - 1);
            add_anc2p(v, son, t);
        }
    }

    int kth(int u, int k) const {
        for (int i = 0; i < LOG; ++i)
            if ((k >> i) & 1)
                u = fa[i][u];

        return u;
    }

    int lca(int u, int v) const {
        if (dep[u] < dep[v])
            swap(u, v);

        u = kth(u, dep[u] - dep[v]);

        if (u == v)
            return u;

        for (int k = LOG - 1; k >= 0; --k) {
            if (fa[k][u] != fa[k][v]) {
                u = fa[k][u];
                v = fa[k][v];
            }
        }

        return fa[0][u];
    }

    // u != v
    int next_on_path(int u, int v) const {
        int L = lca(u, v);

        if (L != u)
            return fa[0][u];

        return kth(v, dep[v] - dep[u] - 1);
    }

    vector<int> topo() const {
        vector<int> deg = indeg;
        queue<int> q;

        for (int i = 1; i <= tot; ++i)
            if (deg[i] == 0)
                q.push(i);

        vector<int> order;
        order.reserve(n);

        int cnt = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            ++cnt;

            if (u <= n)
                order.push_back(u);

            for (int v : g[u])
                if (--deg[v] == 0)
                    q.push(v);
        }

        if (cnt != tot)
            return {};

        return order;
    }

private:
    int newnode() {
        ++tot;
        g.emplace_back();
        indeg.push_back(0);
        return tot;
    }

    void add(int u, int v) {
        g[u].push_back(v);
        ++indeg[v];
    }

    // s -> ancestor chain [anc ... x]
    void add_p2anc(int s, int x, int anc) {
        int len = dep[x] - dep[anc] + 1;
        int cur = x;

        for (int k = LOG - 1; k >= 0; --k) {
            if ((1LL << k) <= len) {
                add(s, out_id[k][cur]);
                cur = fa[k][cur];
                len -= 1 << k;
            }
        }
    }

    // ancestor chain [anc ... x] -> t
    void add_anc2p(int x, int anc, int t) {
        int len = dep[x] - dep[anc] + 1;
        int cur = x;

        for (int k = LOG - 1; k >= 0; --k) {
            if ((1LL << k) <= len) {
                add(in_id[k][cur], t);
                cur = fa[k][cur];
                len -= 1 << k;
            }
        }
    }
};
```

### 现场记忆

```text
它其实就是：
    “线段树优化建图”
换成了
    “倍增段优化建图”。


一个倍增块：

    id[k][u]

表示从 u 向祖先方向连续 2^k 个点：

    u
    fa[u]
    fa²[u]
    ...
    共 2^k 个点


仍然只记两个方向：

out：
    辅助段 -> 段内所有原点

所以：
    s -> out[k][u]
等价于：
    s -> 这 2^k 个点


in：
    段内所有原点 -> 辅助段

所以：
    in[k][u] -> t
等价于：
    这 2^k 个点 -> t


路径 u-v 怎么拆：

          L = lca(u,v)
         / \
        /   \
       u     v

拆成两条向祖先走的链：

    u -> L       （包含 L）
    v -> son(L)  （不再包含 L）

每条祖先链再按二进制拆成 O(log n) 个倍增块。


因此：

p2path(s,u,v)
    = s -> path(u,v) 每个点

path2p(u,v,t)
    = path(u,v) 每个点 -> t


拓扑题中边的含义：

    a -> b
    = a 必须排在 b 前面


特别注意自环：

若 s 本身在 path(u,v) 中，
直接：
    p2path(s,u,v)

会包含：
    s -> s

拓扑图立刻有环。

若要排除路径端点 u：

    int x = next_on_path(u, v);

路径 u-v 去掉 u
    = path(x,v)


建图顺序固定：

    1. add_tree()
    2. build()
    3. p2p / p2path / path2p
    4. topo()

build() 会重新建立辅助图，
所以依赖边必须在 build() 之后加。
```

## 2.24 最大流 - Dinic

> **用途**：求有向网络最大流，并可在最大流后取得对应最小割的源点侧。
> **复杂度**：一般图最坏时间 `O(V^2E)`，空间 `O(V+E)`。
> **下标**：节点 `1-indexed`。
> **注意**：`maxflow()` 会直接修改残量网络；若要从原图重新计算，需要重新 `init + add`。

### 调用

```cpp
Dinic dinic(n);

dinic.add(u, v, cap);                 // 有向边
dinic.add_undirected(u, v, cap);      // 无向容量边

long long flow = dinic.maxflow(S, T);

// 最大流后，最小割的 S 侧
auto side = dinic.reachable(S);
```

限制流量：

```cpp
long long flow = dinic.maxflow(S, T, K);
```

动态增加辅助点：

```cpp
int x = dinic.add_node();
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `add(u,v,cap)` | 加有向容量边 |
| `add_undirected(u,v,cap)` | 加无向容量边 |
| `add_node()` | 新增节点并返回编号 |
| `maxflow(s,t)` | 求最大流 |
| `maxflow(s,t,lim)` | 最多增广 `lim` 流量 |
| `reachable(s)` | 当前残量网络中从 `s` 可达的点 |

### 模板

```cpp
struct Dinic {
    struct Edge {
        int to, rev;
        long long cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, cur;

    Dinic(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        level.assign(n + 1, 0);
        cur.assign(n + 1, 0);
    }

    int add_node() {
        ++n;
        g.emplace_back();
        level.push_back(0);
        cur.push_back(0);
        return n;
    }

    void add(int u, int v, long long cap) {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap});
        g[v].push_back({u, iu, 0});
    }

    void add_undirected(int u, int v, long long cap) {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap});
        g[v].push_back({u, iu, cap});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);

        queue<int> q;
        level[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    long long dfs(int u, int t, long long flow) {
        if (u == t)
            return flow;

        long long used = 0;

        for (int& i = cur[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];

            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                long long pushed =
                    dfs(e.to, t, min(flow - used, e.cap));

                if (pushed) {
                    e.cap -= pushed;
                    g[e.to][e.rev].cap += pushed;
                    used += pushed;
                }

                if (used == flow)
                    return used;
            }
        }

        level[u] = -1;
        return used;
    }

    long long maxflow(
        int s, int t,
        long long lim = (1LL << 60)
    ) {
        if (s == t)
            return 0;

        long long flow = 0;

        while (flow < lim && bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);

            while (flow < lim) {
                long long pushed = dfs(s, t, lim - flow);

                if (!pushed)
                    break;

                flow += pushed;
            }
        }

        return flow;
    }

    vector<char> reachable(int s) const {
        vector<char> vis(n + 1, false);
        queue<int> q;

        vis[s] = true;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& e : g[u]) {
                if (e.cap > 0 && !vis[e.to]) {
                    vis[e.to] = true;
                    q.push(e.to);
                }
            }
        }

        return vis;
    }
};
```

### 现场记忆

```text
BFS：
    只走 cap > 0 的边
    建 level 分层图。

DFS：
    只走 level[v] = level[u] + 1。
    cur[u] = 当前弧优化。

增广：
    e.cap -= f
    reverse.cap += f

最大流结束后：
    残量网络中从 S 可达的点 = 最小割 S 侧。

有向边：
    正向 cap，反向 0。

无向容量边：
    正反初始容量都为 cap。
```

## 2.25 最小费用最大流 - SPFA

> **用途**：求最大流及其最小费用；实现短，适合规模较小或费用图不大的题。
> **复杂度**：设共增广 `A` 次，每次 SPFA 最坏 `O(VE)`，总计最坏 `O(AVE)`；空间 `O(V+E)`。
> **下标**：节点 `1-indexed`。
> **限制**：默认残量网络中不存在可利用的负费用环。
> **注意**：允许负费用边；`flow()` 会修改残量网络。

### 调用

```cpp
MCMF mcmf(n);

mcmf.add(u, v, cap, cost);

auto [flow, cost] = mcmf.flow(S, T);
```

要求最多发送 `K` 单位流：

```cpp
auto [flow, cost] = mcmf.flow(S, T, K);

if (flow == K) {
    // cost = K 单位流的最小费用
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `add(u,v,cap,cost)` | 加容量 `cap`、费用 `cost` 的有向边 |
| `flow(s,t)` | 返回 `{最大流, 最小费用}` |
| `flow(s,t,lim)` | 最多发送 `lim` 单位流 |

### 模板

```cpp
struct MCMF {
    struct Edge {
        int to, rev;
        long long cap, cost;
    };

    int n;
    vector<vector<Edge>> g;
    vector<long long> dis;
    vector<int> pre_v, pre_e;
    vector<char> in;

    MCMF(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dis.resize(n + 1);
        pre_v.resize(n + 1);
        pre_e.resize(n + 1);
        in.resize(n + 1);
    }

    void add(
        int u, int v,
        long long cap, long long cost
    ) {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap, cost});
        g[v].push_back({u, iu, 0, -cost});
    }

    bool spfa(int s, int t) {
        const long long INF = 1LL << 60;

        fill(dis.begin(), dis.end(), INF);
        fill(in.begin(), in.end(), false);

        queue<int> q;
        dis[s] = 0;
        in[s] = true;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in[u] = false;

            for (int i = 0; i < (int)g[u].size(); ++i) {
                Edge& e = g[u][i];

                if (
                    e.cap > 0 &&
                    dis[e.to] > dis[u] + e.cost
                ) {
                    dis[e.to] = dis[u] + e.cost;
                    pre_v[e.to] = u;
                    pre_e[e.to] = i;

                    if (!in[e.to]) {
                        in[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }

        return dis[t] != INF;
    }

    pair<long long, long long> flow(
        int s, int t,
        long long lim = (1LL << 60)
    ) {
        if (s == t)
            return {0, 0};

        long long maxflow = 0;
        long long mincost = 0;

        while (maxflow < lim && spfa(s, t)) {
            long long f = lim - maxflow;

            for (int v = t; v != s; v = pre_v[v])
                f = min(f, g[pre_v[v]][pre_e[v]].cap);

            for (int v = t; v != s; v = pre_v[v]) {
                Edge& e = g[pre_v[v]][pre_e[v]];

                e.cap -= f;
                g[e.to][e.rev].cap += f;
            }

            maxflow += f;
            mincost += f * dis[t];
        }

        return {maxflow, mincost};
    }
};
```

### 现场记忆

```text
残量边：
    正向 (cap, cost)
    反向 (0, -cost)

每轮：
    SPFA 找当前残量网络最短 S->T 路
    找路径瓶颈 f
    整条路径增广 f

费用增加：
    mincost += f * dis[T]

求恰好 K 流：
    flow(S,T,K)
    最后检查 flow == K。

SPFA 版允许负边，
但不要拿它处理存在可利用负费用环的残量网络。
```

## 2.26 最小费用最大流 - 势能 Dijkstra / Johnson

> **用途**：用势能重标号把残量边费用变为非负，再用 Dijkstra 求最短增广路；适合较大的最小费用流。
> **复杂度**：初始势能 SPFA 最坏 `O(VE)`；之后每次增广 `O(E log V)`，共增广 `A` 次，总计 `O(VE + AE log V)`；空间 `O(V+E)`。
> **下标**：节点 `1-indexed`。
> **限制**：默认残量网络中不存在可利用的负费用环。
> **注意**：初始 SPFA 使模板可以直接处理负费用边。

### 调用

```cpp
MCMF mcmf(n);

mcmf.add(u, v, cap, cost);

auto [flow, cost] = mcmf.flow(S, T);
```

限制流量：

```cpp
auto [flow, cost] = mcmf.flow(S, T, K);

if (flow == K) {
    // cost = K 单位流的最小费用
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `add(u,v,cap,cost)` | 加容量 `cap`、费用 `cost` 的有向边 |
| `flow(s,t)` | 返回 `{最大流, 最小费用}` |
| `flow(s,t,lim)` | 最多发送 `lim` 单位流 |
| `h[u]` | Johnson 势能 |

### 模板

```cpp
struct MCMF {
    struct Edge {
        int to, rev;
        long long cap, cost;
    };

    int n;
    vector<vector<Edge>> g;
    vector<long long> dis, h;
    vector<int> pre_v, pre_e;
    vector<char> in;

    MCMF(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dis.resize(n + 1);
        h.resize(n + 1);
        pre_v.resize(n + 1);
        pre_e.resize(n + 1);
        in.resize(n + 1);
    }

    void add(
        int u, int v,
        long long cap, long long cost
    ) {
        int iu = g[u].size();
        int iv = g[v].size();

        if (u == v)
            ++iv;

        g[u].push_back({v, iv, cap, cost});
        g[v].push_back({u, iu, 0, -cost});
    }

    void init_potential(int s) {
        const long long INF = 1LL << 60;

        fill(h.begin(), h.end(), INF);
        fill(in.begin(), in.end(), false);

        queue<int> q;
        h[s] = 0;
        in[s] = true;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in[u] = false;

            for (const auto& e : g[u]) {
                if (
                    e.cap > 0 &&
                    h[e.to] > h[u] + e.cost
                ) {
                    h[e.to] = h[u] + e.cost;

                    if (!in[e.to]) {
                        in[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }

        for (int i = 1; i <= n; ++i)
            if (h[i] == INF)
                h[i] = 0;
    }

    bool dijkstra(int s, int t) {
        const long long INF = 1LL << 60;

        fill(dis.begin(), dis.end(), INF);

        priority_queue<
            pair<long long, int>,
            vector<pair<long long, int>>,
            greater<pair<long long, int>>
        > pq;

        dis[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d != dis[u])
                continue;

            for (int i = 0; i < (int)g[u].size(); ++i) {
                Edge& e = g[u][i];

                if (e.cap == 0)
                    continue;

                long long nd =
                    d + e.cost + h[u] - h[e.to];

                if (dis[e.to] > nd) {
                    dis[e.to] = nd;
                    pre_v[e.to] = u;
                    pre_e[e.to] = i;
                    pq.push({nd, e.to});
                }
            }
        }

        if (dis[t] == INF)
            return false;

        for (int i = 1; i <= n; ++i)
            if (dis[i] != INF)
                h[i] += dis[i];

        return true;
    }

    pair<long long, long long> flow(
        int s, int t,
        long long lim = (1LL << 60)
    ) {
        if (s == t)
            return {0, 0};

        long long maxflow = 0;
        long long mincost = 0;

        init_potential(s);

        while (maxflow < lim && dijkstra(s, t)) {
            long long f = lim - maxflow;

            for (int v = t; v != s; v = pre_v[v])
                f = min(f, g[pre_v[v]][pre_e[v]].cap);

            for (int v = t; v != s; v = pre_v[v]) {
                Edge& e = g[pre_v[v]][pre_e[v]];

                e.cap -= f;
                g[e.to][e.rev].cap += f;
            }

            maxflow += f;
            mincost += f * (h[t] - h[s]);
        }

        return {maxflow, mincost};
    }
};
```

### 现场记忆

```text
势能 h[u] = 从 S 到 u 的当前最短距离势能。

重标号费用：
    cost'(u,v)
    = cost(u,v) + h[u] - h[v]
    >= 0

所以后续可以 Dijkstra。

Dijkstra 后更新：
    h[v] += dis[v]
只更新本轮可达点。

此时：
    h[T] - h[S]
就是当前增广路的真实费用。

费用增加：
    mincost += f * (h[T] - h[S])

初始有负费用边：
    先 SPFA 求一次 h。

若初始所有费用都非负，
理论上可以直接令 h = 0 跳过初始 SPFA。
```

## 2.27 最小割树 - Gomory-Hu

> **用途**：用一棵树表示无向图任意两点之间的最小割；树上 `s-t` 路径最小边权就是原图 `s-t` 最小割。
> **复杂度**：建树需要 `n-1` 次最大流，即 `O(n · T_maxflow)`；当前 `query()` 单次 `O(n)`；空间 `O(n+m)`。
> **下标**：节点 `1-indexed`。
> **限制**：仅用于无向非负容量图；依赖前面的 `Dinic`。
> **注意**：原图不连通也可以使用，对应点对最小割为 `0`。

### 调用

```cpp
GomoryHu gh(n);

for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    gh.add(u, v, w);
}

gh.build();

cout << gh.query(u, v) << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u,v,w)` | 加无向容量边 |
| `build()` | 建 Gomory-Hu Tree |
| `query(u,v)` | 查询原图 `u-v` 最小割，要求 `u != v` |
| `tree` | 最小割树 |
| `fa[u]` | 构造过程中 `u` 的父亲 |
| `cut[u]` | 最终树边 `(u,fa[u])` 的权值 |

### 模板

```cpp
struct GomoryHu {
    struct Edge {
        int u, v;
        long long w;
    };

    int n;
    vector<Edge> e;
    vector<int> fa;
    vector<long long> cut;
    vector<vector<pair<int, long long>>> tree;

    GomoryHu(int n = 0) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        e.clear();
        fa.clear();
        cut.clear();
        tree.clear();
    }

    void add(int u, int v, long long w) {
        if (u != v)
            e.push_back({u, v, w});
    }

    pair<long long, vector<char>>
    solve_flow(int s, int t) const {
        Dinic dinic(n);

        for (auto [u, v, w] : e)
            dinic.add_undirected(u, v, w);

        long long flow = dinic.maxflow(s, t);

        return {
            flow,
            dinic.reachable(s)
        };
    }

    void build() {
        fa.assign(n + 1, 1);
        cut.assign(n + 1, 0);
        tree.assign(n + 1, {});

        for (int s = 2; s <= n; ++s) {
            int t = fa[s];

            auto [flow, side] = solve_flow(s, t);

            for (int i = s + 1; i <= n; ++i) {
                if (fa[i] == t && side[i])
                    fa[i] = s;
            }

            if (side[fa[t]]) {
                fa[s] = fa[t];
                fa[t] = s;

                cut[s] = cut[t];
                cut[t] = flow;
            } else {
                cut[s] = flow;
            }
        }

        for (int i = 2; i <= n; ++i) {
            tree[i].push_back({fa[i], cut[i]});
            tree[fa[i]].push_back({i, cut[i]});
        }
    }

    long long query(int s, int t) const {
        const long long INF = 1LL << 60;

        vector<long long> mn(n + 1, -1);
        queue<int> q;

        mn[s] = INF;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (u == t)
                return mn[u];

            for (auto [v, w] : tree[u]) {
                if (mn[v] == -1) {
                    mn[v] = min(mn[u], w);
                    q.push(v);
                }
            }
        }

        return -1;
    }
};
```

### 现场记忆

```text
Gomory-Hu 只用于无向图。

核心性质：
    原图 mincut(s,t)
    = 最小割树上 s-t 路径的最小边权。

建树：
    一共做 n-1 次 s-t 最大流。

每次最大流后：
    side = 残量网络中从 s 可达的点
    = 本次最小割的 s 侧。

cut[u]：
    最终树边 u-fa[u] 的权值。

query：
    沿树上路径取 min。

查询很多时：
    可在 tree 上加倍增，
    把单次 query 优化到 O(log n)。
```

## 2.28 全局最小割 - Stoer-Wagner

> **用途**：求无向带权图所有非平凡割中的最小割权值，不需要指定源点和汇点。
> **复杂度**：时间 `O(n^3)`，空间 `O(n^2)`。
> **下标**：节点 `1-indexed`，邻接矩阵为 `g[1..n][1..n]`。
> **限制**：仅用于无向非负权图。
> **注意**：重边需要在邻接矩阵中累加；图不连通时答案为 `0`。

### 调用

```cpp
vector<vector<long long>> g(
    n + 1,
    vector<long long>(n + 1)
);

for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;

    g[u][v] += w;
    g[v][u] += w;
}

cout << stoer_wagner(g) << '\n';
```

### 模板

```cpp
long long stoer_wagner(
    vector<vector<long long>> g
) {
    int n = (int)g.size() - 1;

    if (n <= 1)
        return 0;

    vector<int> v(n);
    iota(v.begin(), v.end(), 1);

    const long long INF = 1LL << 62;
    long long ans = INF;

    while ((int)v.size() > 1) {
        int sz = v.size();

        vector<long long> dis(n + 1, 0);
        vector<char> vis(n + 1, false);

        int pre = -1;

        for (int i = 0; i < sz; ++i) {
            int sel = -1;

            for (int x : v) {
                if (
                    !vis[x] &&
                    (sel == -1 || dis[x] > dis[sel])
                ) {
                    sel = x;
                }
            }

            if (i == sz - 1) {
                ans = min(ans, dis[sel]);

                for (int x : v) {
                    if (x == sel || x == pre)
                        continue;

                    g[pre][x] += g[sel][x];
                    g[x][pre] = g[pre][x];
                }

                v.erase(
                    find(v.begin(), v.end(), sel)
                );

                break;
            }

            vis[sel] = true;

            for (int x : v) {
                if (!vis[x])
                    dis[x] += g[sel][x];
            }

            pre = sel;
        }
    }

    return ans;
}
```

### 现场记忆

```text
每一轮 Maximum Adjacency Search：

从空集合 A 开始，
不断选择与 A 连接权值最大的未选点。

最后加入的两个点：
    pre = 倒数第二个
    sel = 最后一个

这一轮：
    dis[sel]
就是一个合法割值，
用它更新答案。

然后把：
    sel 合并进 pre

即：
    g[pre][x] += g[sel][x]

重复直到只剩一个点。

Gomory-Hu：
    求所有点对最小割结构，需要 n-1 次最大流。

Stoer-Wagner：
    只求整张图的全局最小割，O(n^3)。
```

## 2.29 0-1 BFS

> **用途**：求边权只有 `0/1` 的图上单源最短路。
> **复杂度**：时间 `O(n+m)`，空间 `O(n+m)`
> **下标**：节点 `1 ~ n`
> **限制**：边权只能为 `0` 或 `1`。

### 调用

```cpp
vector<vector<pair<int, int>>> g(n + 1);

g[u].push_back({v, w});
g[v].push_back({u, w});

auto dis = zeroOneBFS(g, s);
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<ll> zeroOneBFS(const vector<vector<pair<int, int>>>& g, int s) {
    int n = (int)g.size() - 1;
    const ll INF = 1LL << 60;

    vector<ll> dis(n + 1, INF);
    deque<int> q;

    dis[s] = 0;
    q.push_front(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop_front();

        for (auto [v, w] : g[u]) {
            if (dis[v] <= dis[u] + w) continue;

            dis[v] = dis[u] + w;

            if (w == 0) q.push_front(v);
            else q.push_back(v);
        }
    }

    return dis;
}
```

### 现场记忆

```text
边权 0 -> push_front
边权 1 -> push_back

本质：deque 维持距离非降顺序。

只有 0/1 边权才能直接套。
```

## 2.30 二分图最大匹配 Hopcroft-Karp

> **用途**：求二分图最大匹配，比普通增广路适合较大的稀疏二分图。
> **复杂度**：时间 `O(m√n)`，空间 `O(n+m)`
> **下标**：左部点 `1 ~ nL`，右部点 `1 ~ nR`

### 调用

```cpp
HopcroftKarp hk(nL, nR);

for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    hk.add(u, v);
}

cout << hk.work() << '\n';
cout << hk.matchL[u] << '\n';
```

### 接口速查

| 接口 / 变量 | 含义 |
|---|---|
| `add(u,v)` | 左部 `u` 连右部 `v` |
| `work()` | 最大匹配数 |
| `matchL[u]` | 左部 `u` 匹配的右点 |
| `matchR[v]` | 右部 `v` 匹配的左点 |

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

struct HopcroftKarp {
    int nL, nR;
    vector<vector<int>> g;
    vector<int> matchL, matchR, dis;

    HopcroftKarp(int nL_, int nR_) : nL(nL_), nR(nR_) {
        g.assign(nL + 1, {});
        matchL.assign(nL + 1, 0);
        matchR.assign(nR + 1, 0);
        dis.resize(nL + 1);
    }

    void add(int u, int v) {
        g[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        bool found = false;

        fill(dis.begin(), dis.end(), -1);

        for (int u = 1; u <= nL; ++u) {
            if (!matchL[u]) {
                dis[u] = 0;
                q.push(u);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                int w = matchR[v];

                if (!w) found = true;
                else if (dis[w] == -1) {
                    dis[w] = dis[u] + 1;
                    q.push(w);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            int w = matchR[v];

            if (!w || (dis[w] == dis[u] + 1 && dfs(w))) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }

        dis[u] = -1;
        return false;
    }

    int work() {
        fill(matchL.begin(), matchL.end(), 0);
        fill(matchR.begin(), matchR.end(), 0);

        int ans = 0;

        while (bfs())
            for (int u = 1; u <= nL; ++u)
                if (!matchL[u] && dfs(u))
                    ++ans;

        return ans;
    }
};
```

### 现场记忆

```text
BFS：给所有未匹配左点同时分层。
DFS：只沿着层数 +1 的增广路走。

一轮 BFS 可以找多条最短增广路。

matchL[u] / matchR[v] = 0 表示未匹配。
```

## 2.31 KM / Hungarian Assignment

> **用途**：解决带权二分图 Assignment Problem；代码求每个左点恰好匹配一个不同右点的最小总代价。
> **复杂度**：矩形 `n <= m` 时 `O(n²m)`，方阵为 `O(n³)`；空间 `O(n+m)`
> **下标**：输入矩阵 `0-indexed`
> **限制**：必须满足左部点数 `n <= m`。
> **注意**：求最大权匹配时将权值取负，最后答案再取负。

### 调用

```cpp
vector<vector<ll>> cost(n, vector<ll>(m));

Hungarian hungarian;
auto [ans, match] = hungarian.solve(cost);

cout << ans << '\n';
cout << match[i] << '\n'; // 左点 i 匹配的右点
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Hungarian {
    pair<ll, vector<int>> solve(const vector<vector<ll>>& a) {
        int n = a.size();
        int m = n ? a[0].size() : 0;

        if (!n) return {0, {}};
        assert(n <= m);

        const ll INF = 1LL << 60;

        vector<ll> u(n + 1), v(m + 1);
        vector<int> p(m + 1), way(m + 1);

        for (int i = 1; i <= n; ++i) {
            p[0] = i;

            int j0 = 0;
            vector<ll> minv(m + 1, INF);
            vector<char> used(m + 1);

            do {
                used[j0] = 1;

                int i0 = p[j0], j1 = 0;
                ll delta = INF;

                for (int j = 1; j <= m; ++j) {
                    if (used[j]) continue;

                    ll cur = a[i0 - 1][j - 1] - u[i0] - v[j];

                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }

                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }

                for (int j = 0; j <= m; ++j) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }

                j0 = j1;
            } while (p[j0] != 0);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }

        vector<int> match(n, -1);

        for (int j = 1; j <= m; ++j)
            if (p[j])
                match[p[j] - 1] = j - 1;

        return {-v[0], match};
    }
};
```

### 现场记忆

```text
Hungarian：维护顶标 u / v 和当前匹配 p。

代码求最小权 Assignment。
最大权：cost = -weight，再把最终答案取负。

n > m 时先交换二分图两侧，或转置矩阵。
```

## 2.32 上下界可行流

> **用途**：判断每条边流量限制为 `[low, high]` 时，是否存在满足流量守恒的可行流，并恢复各边流量。
> **复杂度**：一次 Dinic，通常 `O(V²E)`；空间 `O(V+E)`
> **下标**：节点 `1 ~ n`
> **限制**：所有边满足 `0 <= low <= high`。

### 调用

```cpp
LowerBoundFlow flow(n);

int id = flow.add(u, v, low, high);

if (!flow.work()) {
    cout << "NO\n";
} else {
    cout << "YES\n";
    cout << flow.edgeFlow(id) << '\n';
}
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `add(u,v,low,high)` | 加上下界边，返回边编号 |
| `work()` | 判断是否存在可行流 |
| `edgeFlow(id)` | 可行流中原边实际流量 |

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> dep, cur;

    Dinic(int n_ = 0) : n(n_), g(n), dep(n), cur(n) {}

    int add(int u, int v, ll cap) {
        int id = g[u].size();
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, id, 0});
        return id;
    }

    bool bfs(int s, int t) {
        fill(dep.begin(), dep.end(), -1);
        queue<int> q;

        dep[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto& e : g[u]) {
                if (e.cap && dep[e.to] == -1) {
                    dep[e.to] = dep[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return dep[t] != -1;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t) return f;

        for (int& i = cur[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];

            if (!e.cap || dep[e.to] != dep[u] + 1) continue;

            ll got = dfs(e.to, t, min(f, e.cap));

            if (got) {
                e.cap -= got;
                g[e.to][e.rev].cap += got;
                return got;
            }
        }

        return 0;
    }

    ll maxflow(int s, int t) {
        const ll INF = 1LL << 62;
        ll ans = 0;

        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);

            while (ll f = dfs(s, t, INF)) ans += f;
        }

        return ans;
    }
};

struct LowerBoundFlow {
    struct Info {
        int u, idx;
        ll low, cap;
    };

    int n, S, T;
    Dinic dinic;
    vector<ll> bal;
    vector<Info> edges;

    LowerBoundFlow(int n_) : n(n_), S(n + 1), T(n + 2), dinic(n + 3), bal(n + 1) {}

    int add(int u, int v, ll low, ll high) {
        int idx = dinic.add(u, v, high - low);

        bal[u] -= low;
        bal[v] += low;
        edges.push_back({u, idx, low, high - low});

        return (int)edges.size() - 1;
    }

    bool work() {
        ll need = 0;

        for (int u = 1; u <= n; ++u) {
            if (bal[u] > 0) {
                dinic.add(S, u, bal[u]);
                need += bal[u];
            } else if (bal[u] < 0) {
                dinic.add(u, T, -bal[u]);
            }
        }

        return dinic.maxflow(S, T) == need;
    }

    ll edgeFlow(int id) const {
        auto [u, idx, low, cap] = edges[id];
        return low + cap - dinic.g[u][idx].cap;
    }
};
```

### 现场记忆

```text
原边 [low, high]：
容量改成 high-low

同时：
bal[u] -= low
bal[v] += low

bal > 0：S -> u
bal < 0：u -> T

S 到 T 的最大流必须完全满足所有正 bal。
原边实际流量 = low + 残量网络中已经流掉的部分。
```

## 2.33 上下界最大流

> **用途**：在每条边具有流量上下界时，求 `s -> t` 的最大可行流。
> **复杂度**：两次 Dinic，通常 `O(V²E)`；空间 `O(V+E)`
> **下标**：节点 `1 ~ n`
> **限制**：所有边满足 `0 <= low <= high`。
> **注意**：无可行流时 `work(s,t)` 返回 `-1`。

### 调用

```cpp
LowerBoundMaxFlow flow(n);

flow.add(u, v, low, high);

ll ans = flow.work(s, t);

if (ans == -1) cout << "NO\n";
else cout << ans << '\n';
```

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> dep, cur;

    Dinic(int n_ = 0) : n(n_), g(n), dep(n), cur(n) {}

    int add(int u, int v, ll cap) {
        int id = g[u].size();
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, id, 0});
        return id;
    }

    bool bfs(int s, int t) {
        fill(dep.begin(), dep.end(), -1);
        queue<int> q;

        dep[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto& e : g[u]) {
                if (e.cap && dep[e.to] == -1) {
                    dep[e.to] = dep[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return dep[t] != -1;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t) return f;

        for (int& i = cur[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];

            if (!e.cap || dep[e.to] != dep[u] + 1) continue;

            ll got = dfs(e.to, t, min(f, e.cap));

            if (got) {
                e.cap -= got;
                g[e.to][e.rev].cap += got;
                return got;
            }
        }

        return 0;
    }

    ll maxflow(int s, int t) {
        const ll INF = 1LL << 62;
        ll ans = 0;

        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);

            while (ll f = dfs(s, t, INF)) ans += f;
        }

        return ans;
    }

    void disable(int u, int idx) {
        int v = g[u][idx].to;
        int rev = g[u][idx].rev;

        g[u][idx].cap = 0;
        g[v][rev].cap = 0;
    }
};

struct LowerBoundMaxFlow {
    int n, S, T;
    Dinic dinic;
    vector<ll> bal;

    LowerBoundMaxFlow(int n_) : n(n_), S(n + 1), T(n + 2), dinic(n + 3), bal(n + 1) {}

    void add(int u, int v, ll low, ll high) {
        dinic.add(u, v, high - low);
        bal[u] -= low;
        bal[v] += low;
    }

    ll work(int s, int t) {
        const ll INF = 1LL << 62;

        int back = dinic.add(t, s, INF);
        vector<pair<int, int>> extra;

        ll need = 0;

        for (int u = 1; u <= n; ++u) {
            if (bal[u] > 0) {
                int id = dinic.add(S, u, bal[u]);
                extra.push_back({S, id});
                need += bal[u];
            } else if (bal[u] < 0) {
                int id = dinic.add(u, T, -bal[u]);
                extra.push_back({u, id});
            }
        }

        if (dinic.maxflow(S, T) != need) return -1;

        ll base = INF - dinic.g[t][back].cap;

        for (auto [u, id] : extra) dinic.disable(u, id);
        dinic.disable(t, back);

        return base + dinic.maxflow(s, t);
    }
};
```

### 现场记忆

```text
先把上下界可行流转成循环流。

额外加：
t -> s，容量 INF

跑 S -> T 可行流后：
t -> s 上已经流过的量 = 初始 s->t 流量。

删掉超级源汇和 t->s，
再跑一次 maxflow(s,t)。

答案 = 初始流量 + 新增流量。
```

## 2.34 最大权闭合子图

> **用途**：选择若干点最大化权值和，同时满足依赖关系 `选 u => 必须选 v`。
> **复杂度**：一次最大流；Dinic 通常 `O(V²E)`
> **下标**：节点 `1 ~ n`
> **注意**：依赖边容量必须大于任何可能的有限割，代码使用 `INF = 2^62`。

### 调用

```cpp
MaxWeightClosure closure(n);

closure.addWeight(u, w);
closure.addDependency(u, v); // 选 u 必须选 v

auto [ans, chosen] = closure.work();

cout << ans << '\n';
```

### 接口速查

| 接口 | 含义 |
|---|---|
| `addWeight(u,w)` | 给点 `u` 增加权值 |
| `addDependency(u,v)` | `选 u => 必须选 v` |
| `work()` | 返回 `{最大权值, 选择的点}` |

### 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> dep, cur;

    Dinic(int n_) : n(n_), g(n), dep(n), cur(n) {}

    void add(int u, int v, ll cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        fill(dep.begin(), dep.end(), -1);
        queue<int> q;

        dep[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto& e : g[u]) {
                if (e.cap && dep[e.to] == -1) {
                    dep[e.to] = dep[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return dep[t] != -1;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t) return f;

        for (int& i = cur[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];

            if (!e.cap || dep[e.to] != dep[u] + 1) continue;

            ll got = dfs(e.to, t, min(f, e.cap));

            if (got) {
                e.cap -= got;
                g[e.to][e.rev].cap += got;
                return got;
            }
        }

        return 0;
    }

    ll maxflow(int s, int t) {
        const ll INF = 1LL << 62;
        ll ans = 0;

        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);

            while (ll f = dfs(s, t, INF)) ans += f;
        }

        return ans;
    }
};

struct MaxWeightClosure {
    int n;
    vector<ll> w;
    vector<pair<int, int>> dep;

    MaxWeightClosure(int n_) : n(n_), w(n + 1) {}

    void addWeight(int u, ll x) {
        w[u] += x;
    }

    void addDependency(int u, int v) {
        dep.push_back({u, v});
    }

    pair<ll, vector<int>> work() {
        const ll INF = 1LL << 62;

        int S = 0, T = n + 1;
        Dinic dinic(n + 2);

        ll sum = 0;

        for (int u = 1; u <= n; ++u) {
            if (w[u] > 0) {
                dinic.add(S, u, w[u]);
                sum += w[u];
            } else if (w[u] < 0) {
                dinic.add(u, T, -w[u]);
            }
        }

        for (auto [u, v] : dep) dinic.add(u, v, INF);

        ll ans = sum - dinic.maxflow(S, T);

        vector<char> vis(n + 2);
        queue<int> q;

        vis[S] = 1;
        q.push(S);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto& e : dinic.g[u]) {
                if (e.cap && !vis[e.to]) {
                    vis[e.to] = 1;
                    q.push(e.to);
                }
            }
        }

        vector<int> chosen;

        for (int u = 1; u <= n; ++u)
            if (vis[u])
                chosen.push_back(u);

        return {ans, chosen};
    }
};
```

### 现场记忆

```text
正权点：S -> u，容量 +w
负权点：u -> T，容量 -w

依赖：
选 u => 必须选 v
加 u -> v，容量 INF

答案：
所有正权之和 - 最小割

最大流后还能从 S 到达的点就是选择集合。
```
