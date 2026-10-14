= 基础工具

本章对应原始备份 `raw/01_tools.md`，源码维护在 `code/basic/`。
使用 GNU++17；头文件可独立包含，也可将需要的实现复制到比赛代码。
下列调用片段默认已包含对应头文件，并位于 `main` 或其他函数中。
原始备份不参与编译，也不随模板修正而修改。

== GCD

*用途：* 求最大公约数，用于约分与互质判断。
*形式：* 函数式，直接使用 `std::gcd`，不需要 `struct` 或自写欧几里得封装。

=== 接口、复杂度与约定

- 调用 `std::gcd(a, b)`，依赖 `<numeric>`；参数为非 `bool` 整数。
- 时间 `O(log min(|a|, |b|))`，含零输入为 `O(1)`；额外空间 `O(1)`。
- 无下标。负数输入得到非负结果；`gcd(0, 0) = 0`。
- 两个参数的绝对值必须能由公共类型表示；不要直接传入 `LLONG_MIN`，即使预期答案较小。
- 无初始化状态，可重复调用。计算 LCM 时先除后乘，但最终结果仍需检查溢出。

=== 调用方式

```cpp
long long g = std::gcd(-12LL, 18LL);  // 6
auto large_g = std::gcd(1ULL << 63, 6ULL);  // 2
```

=== 现场记忆

`<numeric>`；零零得零，负数取绝对值。最小有符号整数的绝对值超出同类型范围。

=== 模板引用

#raw(read("../code/basic/gcd.hpp"), lang: "cpp", block: true)

== 位运算枚举

*用途：* 枚举所有状态、置位位置、单集合子集以及所有集合与子集对。
*形式：* 函数式，通过回调直接处理状态；没有需要长期保存的数据，不使用 `struct`。

=== 接口、复杂度与约定

- `for_each_mask(n, visit)`：枚举 `[0, 2^n)`，时间 `O(2^n)`，要求 `0 <= n < 64`。
- `for_each_set_bit(mask, visit)`：从低位到高位枚举置位下标，时间 `O(popcount(mask))`。
- `for_each_submask(mask, visit, include_empty = true)`：降序枚举子集，时间 `O(2^popcount(mask))`。
- `for_each_mask_submask(n, visit, include_empty = false)`：枚举 `(mask, sub)`，时间 `O(3^n)`。
- 位下标从 `0` 开始，掩码使用 `uint64_t`；所有接口额外空间 `O(1)`，不计回调成本。
- `n = 0` 时状态仅有 `0`；空集的非空子集枚举无输出，含空集时输出一次 `0`。
- 置位和子集接口可使用第 `63` 位；状态枚举不能令 `n = 64`。实际可枚举位数受指数复杂度限制。
- 无初始化状态，重复调用彼此独立。回调不要修改捕获的循环输入或造成未预期的副作用。

=== 调用方式

```cpp
for_each_mask(3, [&](std::uint64_t mask) {
    // 处理 mask
});
for_each_set_bit(10, [&](int i) {
    // i 依次为 1、3
});
for_each_submask(10, [&](std::uint64_t sub) {
    // sub 依次为 10、8、2、0
});
for_each_mask_submask(3, [&](std::uint64_t mask, std::uint64_t sub) {
    // 默认只处理非空 sub；传第三个参数 true 才包含空集
});
```

=== 现场记忆

删最低位 `1`：`s &= s - 1`；下一子集：`(s - 1) & mask`。
包含空集时处理完 `0` 必须退出；`__builtin_ctzll` 的参数必须非零。
移位使用 `uint64_t{1}`，移位量必须小于 `64`。

=== 模板引用

#raw(read("../code/basic/bit_enumeration.hpp"), lang: "cpp", block: true)

== 快读

*用途：* 使用 `fread` 缓冲读取整数和不含空白的字符串。
*形式：* `struct FastIO`，需要维护文件、缓冲区及游标；不适合无状态函数式封装。

=== 接口、复杂度与约定

- `FastIO io(input)`，默认输入为 `stdin`；文件必须有效，由调用方管理其生命周期。
- `read(x)` 支持不超过 `64` 位的标准整数类型（不含 `bool`）；支持前导 `+`、`-`，负数不能读入无符号类型。
- `read(s)` 读取一个无空白字符串，不读取整行。
- 成功返回 `true`；EOF、格式错误或整数越界返回 `false`，目标变量保持原值。错误 token 会被消耗。
- 时间 `O(读取字符数)`，缓冲区空间 `O(2^20)`，字符串另占 `O(token 长度)`。
- 无算法下标；内部缓冲区从 `0` 开始。初始化游标为零，同一个输入流持续使用同一个对象。
- 支持整数最小值和最大值；数字末尾直接 EOF 仍算成功，之后再次读取返回 `false`。
- 缓冲区约 `1 MiB`，建议静态对象。不要复制正在读取的对象，不要与 `cin`、`scanf` 混读同一文件。
- `fread` 可能预读较多输入，不适用于需要逐轮通信的交互题。

=== 调用方式

```cpp
static FastIO io;
int n;
long long x;
std::string s;
if (!io.read(n)) return 0;
if (!io.read(x) || !io.read(s)) return 0;
```

=== 现场记忆

`gc()` 返回 `int` 才能区分字节和 EOF；整数在无符号类型中累积。
最小负数用 `-(value - 1) - 1` 构造，不先累积正的有符号绝对值。
先判断 `read` 返回值，再使用变量；读字符串只得到 token。

=== 模板引用

#raw(read("../code/basic/fast_io.hpp"), lang: "cpp", block: true)

== 取模类 Mint

*用途：* 模意义的加减乘、幂、逆元和除法。
*形式：* `struct Mint<MOD>`，统一维护归一化值与运算；裸函数容易混入不同模数或忘记归一化。

=== 接口、复杂度与约定

- `Mint<MOD>(x)` 接受 `long long`，默认值为 `0`；`v` 保持在 `[0, MOD)`，不要直接写入未经归一化的值。
- 加减乘、比较、取负为 `O(1)`；`pow(k)` 为 `O(log(k + 1))`，逆元与除法为 `O(log MOD)`；空间均为 `O(1)`。
- 无下标。编译期模数要求 `2 <= MOD <= INT_MAX`；加法及乘法的中间值使用 `long long`。
- `pow(k)` 要求 `k >= 0`，指数为 `0` 返回 `1`，包括 `0^0` 的模板约定。
- 加减乘与非负幂允许合数模；`inv()` 和除法只用于质数模，除数必须非零。
- `assert` 检查负指数和零逆元，但不会验证模数是否为质数；关闭断言也不代表非法调用有效。
- 各对象独立初始化，可重复运算；`pow` 和 `inv` 不修改原对象。

=== 调用方式

```cpp
using mint = Mint<998244353>;
mint a = 10, b = 3;
mint x = a + b, y = a * b, z = a / b;
mint p = a.pow(100);
std::cout << x.v << '\n';
```

=== 现场记忆

构造先 `% MOD`，负数再加模数；乘法先转 `long long`。
快速幂看 `n & 1`、平方、右移；质数模下逆元是 `a^(MOD - 2)`，零不可除。

=== 模板引用

#raw(read("../code/basic/mint.hpp"), lang: "cpp", block: true)

== STL 二分

*用途：* 在升序数组中找第一个大于等于、大于某值的位置，以及最后一个小于、小于等于某值的位置。
*形式：* 函数式，使用 STL 二分并返回下标；不维护索引或状态，不使用 `struct`。

=== 接口、复杂度与约定

- `lower_index(a, x)` / `upper_index(a, x)`：第一个 `>= x` / `> x` 的下标，也分别是 `< x` / `<= x` 的元素个数。
- `last_less_index(a, x)` / `last_less_equal_index(a, x)`：最后一个 `< x` / `<= x` 的下标。
- 参数为升序 `vector`，大小不超过 `INT_MAX`；时间 `O(log n)`，空间 `O(1)`。
- 下标从 `0` 开始。找不到后继返回 `n`，找不到前驱返回 `-1`；空数组分别返回 `0` 与 `-1`。
- 使用默认比较规则，元素与查询值应具有相容的类型；降序或自定义比较需直接使用带比较器的 STL 接口。
- 无需初始化，可重复调用；修改数组后须继续保证有序。使用返回下标前判断范围。
- STL 迭代器接口需判断 `end()` 再解引用、判断 `begin()` 再前移；非随机访问迭代器的移动次数可能为 `O(n)`。

=== 调用方式

```cpp
std::vector<int> a{1, 3, 3, 7};
int p = lower_index(a, 3);  // 1
int q = upper_index(a, 3);  // 3
int before = last_less_index(a, 1);  // -1
int at_most = last_less_equal_index(a, 3);  // 2
if (p < int(a.size())) std::cout << a[p] << '\n';
```

=== 现场记忆

`lower_bound` 找 `>=`；`upper_bound` 找 `>`。
前驱是对应位置减一；先判 `-1` / `n`，再访问数组。

=== 模板引用

#raw(read("../code/basic/binary_search.hpp"), lang: "cpp", block: true)

== 对拍

*用途：* 生成数据，依次运行正解与暴力，比较输出并保留反例；属于本地工具，不随题目提交。
*形式：* 函数式，三个命令及轮数作为参数；不为简单顺序流程增加配置 `struct`。

=== 接口、复杂度与约定

- `stress(generator, solution, brute, rounds = 0)`：有限轮全部通过返回 `true`；发现 WA 或命令失败返回 `false`。`rounds = 0` 持续运行。
- `same_output(a, b)` 比较 token，忽略空白差异；文件缺失或读入错误返回 `false`。
- 每轮时间为生成器、正解、暴力的运行时间加输出比较 `O(输出长度)`；比较额外空间 `O(最长 token 长度)`。
- 无数组下标；轮号从 `1` 开始，每次调用重新计数。
- 在独立工作目录中运行，会覆盖 `data.in`、`sol.out`、`brute.out`；失败后立即返回，保留当前文件。
- 默认命令 `./gen`、`./sol`、`./brute` 适用于 POSIX Shell；Windows 传入当地可执行命令，例如 `gen.exe`。
- 命令由 `system()` 执行，带空格路径需按所用 Shell 引号规则传入。没有内置超时，程序卡住时需外部中止。
- 比较的是字符串 token，`1` 与 `1.0` 不相等；浮点误差或 Special Judge 需替换比较函数。
- 重复调用会重用并覆盖文件；随机生成器需变化种子，固定种子每轮相同会削弱对拍价值。

=== 调用方式

```cpp
stress("./gen", "./sol", "./brute", 1000);
// Windows 示例：stress("gen.exe", "sol.exe", "brute.exe", 1000);
```

=== 现场记忆

造数据 → 正解 → 暴力 → 比较。先看进程退出码，再判 WA。
发现反例立即停，不覆盖 `data.in`；token 比较不等于浮点或 SPJ 比较。

=== 模板引用

#raw(read("../code/basic/stress.hpp"), lang: "cpp", block: true)

== 128 位整数输入输出

*用途：* 通过 `cin` / `cout` 读写有符号 `__int128`，容纳超过 `long long` 的中间结果。
*形式：* 函数式流运算符重载，不维护额外状态，不使用 `struct` 包裹整数。

=== 接口、复杂度与约定

- `i128` 为有符号 `__int128`，`u128` 为无符号类型；仅为 `i128` 提供流输入输出重载。
- 时间和字符串空间均为 `O(数字位数)`；没有下标约定。
- 依赖支持 `__int128` 的 GCC 目标，属于 GNU 扩展，不适用于普通 MSVC。
- 输入为十进制整数 token，允许前导 `+` / `-`；范围 `[-2^127, 2^127 - 1]`。
- EOF 保留流状态；格式错误或越界设置 `failbit`，目标变量不变。要继续读取需先 `cin.clear()`，错误 token 已消耗。
- 输出固定为十进制，不处理 `hex`、宽度等格式选项；支持零与最小负数。
- 重复调用无额外初始化。乘法必须在运算前提升类型：`i128(a) * b`，不要先让 `long long` 溢出。
- 128 位有符号算术仍会溢出；`1LL << 127` 非法，使用无符号 `u128{1} << 127` 表示边界。

=== 调用方式

```cpp
i128 x;
if (!(std::cin >> x)) return 0;
long long a = 1000000000000LL, b = 1000000000000LL;
std::cout << i128(a) * b << '\n';
std::cout << x << '\n';
```

=== 现场记忆

输入先检查流，转为无符号累积；最小负数不能直接取负。
输出绝对值用 `-u128(x)`；乘法前转换，转换结果不能补救已发生的溢出。

=== 模板引用

#raw(read("../code/basic/int128_io.hpp"), lang: "cpp", block: true)

== 随机数 RNG

*用途：* 生成随机整数、打乱数组、为随机算法和对拍提供随机源。
*形式：* 函数式 `rnd` 配合标准库引擎 `rng`；引擎本身保存状态，无需另建自定义 `struct`。

=== 接口、复杂度与约定

- `rnd(l, r)` 在闭区间 `[l, r]` 内生成 `long long`；必须满足 `l <= r`。
- 单次生成期望时间 `O(1)`；引擎固定空间 `O(1)`，内部保存固定大小状态。`shuffle` 时间 `O(n)`。
- `rnd` 无下标；数组打乱不改变原有下标基准。允许 `l = r`、负区间和完整 `long long` 范围。
- 引擎在首次初始化时混合时钟与静态对象地址，避免原模板 `new char` 泄漏；仅为比赛随机化，不用于密码学。
- `rng.seed(seed)` 可重置为可复现序列；通常每个进程播种一次，不要每次 `rnd` 都播种。
- 多次调用推进同一个引擎状态。`rng` 为 C++17 inline 变量，多翻译单元共享同一对象。

=== 调用方式

```cpp
long long x = rnd(-100, 100);
std::vector<int> a{1, 2, 3};
std::shuffle(a.begin(), a.end(), rng);  // 另包含 <algorithm>、<vector>
rng.seed(123456);  // 调试复现时使用固定种子
```

=== 现场记忆

`uniform_int_distribution` 的两端都包含；避免 `rng() % range` 的偏差和区间长度溢出。
复现靠固定种子；随机运行的生成器不要每次都重新固定播种。

=== 模板引用

#raw(read("../code/basic/rng.hpp"), lang: "cpp", block: true)

== Custom Hash / 防 Hack

*用途：* 降低整数与 pair 键的无序容器被预先构造冲突数据拖慢的风险。
*形式：* `struct CustomHash`，标准库容器要求可调用哈希类型，适合轻量函数对象。

=== 接口、复杂度与约定

- `CustomHash{}(x)` 支持能转换为 `uint64_t` 的整数；pair 的两个分量需要各自支持 `std::hash`。
- 哈希计算时间与空间 `O(1)`（不计复杂 pair 分量的自身哈希成本）；容器操作期望 `O(1)`，最坏仍可能 `O(n)`。
- 无下标。负整数转换为无符号表示后计算；`splitmix64` 的无符号溢出是预期行为。
- 随机盐首次使用时初始化，同一进程始终固定，整数和 pair 两条路径都加入盐。
- 无需手动初始化。禁止在已有元素的容器使用期间改变哈希规则，否则查找会失效。
- 随机盐不提供最坏复杂度保证；严格最坏界可选有序容器。
- `reserve` 可减少扩容；需要降低负载因子时先设置，再 `reserve`。重哈希使迭代器失效。

=== 调用方式

```cpp
std::unordered_map<long long, int, CustomHash> mp;
mp.max_load_factor(0.7f);
mp.reserve(100000);
mp[-1] = 7;
std::unordered_set<long long, CustomHash> st;
std::unordered_map<std::pair<int, int>, int, CustomHash> pair_map;
pair_map[{1, 2}] = 3;
```

=== 现场记忆

`splitmix64 + 固定运行时盐`，pair 也要加盐；盐不能每次调用都变。
防预先构造冲突，不能保证绝对防 Hack；扩容后别继续用旧迭代器。

=== 模板引用

#raw(read("../code/basic/custom_hash.hpp"), lang: "cpp", block: true)

== Debug 宏

*用途：* 本地输出变量、pair 和常用容器；提交模式删除调试调用。
*形式：* 宏配合重载函数，打印由类型决定；不需要保存状态，不使用 `struct`。

=== 接口、复杂度与约定

- 本地编译加 `-DLOCAL`，使用 `debug(x, y, ...)`，输出到 `cerr`，不污染标准答案输出。
- 支持可流输出的标量、pair、vector、set、map、unordered_set、unordered_map，支持这些类型的嵌套组合。
- 本地时间 `O(实际打印内容长度)`，递归空间 `O(嵌套深度)`；非 LOCAL 时调用时间、额外空间均为 `O(1)`，不计算参数。
- 容器遍历遵循原容器顺序，vector 从 `0` 开始；无序容器的打印顺序不固定。
- 无初始化状态，重复调用无缓存；空参数调用输出空标签与换行。
- 宏采用 `do ... while(false)`，可以安全放入无花括号的 `if/else`。
- 不要在参数中依赖副作用；`debug(++x)` 在提交模式不会递增。含逗号的模板表达式需先存变量或加括号。
- 本地不支持的自定义类型需自行补打印重载；打印 `i128` 时先包含 `int128_io.hpp`，再包含 `debug.hpp`，确保重载在模板定义时可见。

=== 调用方式

```cpp
std::vector<std::vector<int>> dp{{1, 2}, {3}};
std::map<int, std::vector<int>> groups{{1, {2, 3}}};
debug(dp, groups);
if (dp.empty()) debug("empty");
else debug(dp.size());
```

=== 现场记忆

本地 `-DLOCAL`，提交不加；只写 `cerr`。
嵌套容器的重载先声明再递归定义；调试参数不依赖副作用。

=== 模板引用

#raw(read("../code/basic/debug.hpp"), lang: "cpp", block: true)
