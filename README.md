# 竞技编程模板库

现代化 C++20 竞技编程算法模板库

## 核心特性

- **C++20 现代语法** - concepts、ranges、constexpr等
- **零成本抽象** - 编译期优化，无运行时开销  
- **模板化设计** - 支持任意类型和操作
- **高性能实现** - 部分算法超越标准库性能
- **完整工具链** - 内置对拍、调试、性能测试

## 项目结构

```
template/
├── all.hpp              # 一键包含所有模块
├── DS/                  # 数据结构
├── Graph/              # 图论算法  
├── Math/               # 数学算法
├── String/             # 字符串算法
├── Misc/               # 实用工具
├── debug.hpp           # 调试工具
├── iostream.hpp        # IO优化
└── types.hpp           # 基础类型定义
```

## 算法模块

<details>
<summary><b>🏗️ 数据结构 (DS/)</b></summary>

| 文件 | 算法 | 复杂度 | 用途 |
|------|------|--------|------|
| `SegmentTree.hpp` | 线段树 | $\mathcal{O}(\log n)$ | 区间查询/单点修改 |
| `LazySegmentTree.hpp` | 懒标记线段树 | $\mathcal{O}(\log n)$ | 区间修改/查询 |
| `BinaryIndexedTree.hpp` | 树状数组 | $\mathcal{O}(\log n)$ | 前缀和查询 |
| `DisjointSetUnion.hpp` | 并查集 | $\mathcal{O}(\alpha(n))$ | 连通性查询 |
| `SparseTable.hpp` | ST表 | $\mathcal{O}(1)$ | 静态RMQ |
| `BinaryTrie.hpp` | 01字典树 | $\mathcal{O}(\log V)$ | 异或最值 |
| `Discretizer.hpp` | 离散化 | $\mathcal{O}(n \log n)$ | 坐标压缩 |
| `ErasableDisjointSetUnion.hpp` | 可撤销并查集 | $\mathcal{O}(\log n)$ | 支持撤销 |

</details>

<details>
<summary><b>🌐 图论算法 (Graph/)</b></summary>

| 文件 | 算法 | 复杂度 | 用途 |
|------|------|--------|------|
| `Dijkstra.hpp` | Dijkstra | $\mathcal{O}((V+E) \log V)$ | 单源最短路 |
| `FloydWarshall.hpp` | Floyd | $\mathcal{O}(V^3)$ | 全源最短路 |
| `TopologicalSort.hpp` | 拓扑排序 | $\mathcal{O}(V+E)$ | DAG排序 |
| `Hungarian.hpp` | 匈牙利算法 | $\mathcal{O}(V^3)$ | 二分图匹配 |

</details>

<details>
<summary><b>🔢 数学算法 (Math/)</b></summary>

| 文件 | 算法 | 复杂度 | 用途 |
|------|------|--------|------|
| `Sieve.hpp` | 各种筛法 | $\mathcal{O}(n)$ | 质数/欧拉函数 |
| `FastPower.hpp` | 快速幂 | $\mathcal{O}(\log n)$ | 模幂运算 |
| `MillerRabin.hpp` | 素数测试 | $\mathcal{O}(k \log^3 n)$ | 判断素数 |
| `PollardRho.hpp` | 大数分解 | $\mathcal{O}(n^{\frac{1}{4}})$ | 质因数分解 |
| `Matrix.hpp` | 矩阵快速幂 | $\mathcal{O}(n^3 \log k)$ | 矩阵运算 |
| `XorBase.hpp` | 异或线性基 | $\mathcal{O}(\log V)$ | 异或极值 |
| `ModIntegral.hpp` | 模运算 | $\mathcal{O}(1)$ | 自动取模 |
| `Combination.hpp` | 组合数 | $\mathcal{O}(1)$ | 组合计数 |

</details>

<details>
<summary><b>🔤 字符串算法 (String/)</b></summary>

| 文件 | 算法 | 复杂度 | 用途 |
|------|------|--------|------|
| `StringHash.hpp` | 字符串哈希 | $\mathcal{O}(n)$ | 快速字符串比较 |
| `Manacher.hpp` | Manacher | $\mathcal{O}(n)$ | 回文串检测 |

</details>

<details>
<summary><b>🛠️ 实用工具 (Misc/)</b></summary>

| 文件 | 功能 | 说明 |
|------|------|------|
| `对拍.hpp` | 自动对拍 | 验证算法正确性 |
| `Timer.hpp` | 性能测试 | 代码耗时统计 |
| `FastIO.hpp` | 快速IO | 加速输入输出 |
| `BigInteger.hpp` | 大整数 | 任意精度整数 |
| `Geometry.hpp` | 计算几何 | 2D几何算法 |
| `search.hpp` | 搜索算法 | 二分搜索等 |

</details>

## 使用示例

### 线段树

```cpp
#include "DS/SegmentTree.hpp"

// 区间求和
std::vector<int> a{1, 2, 3, 4, 5};
auto e = []() { return 0; };
SegmentTree<int, std::plus<>(), e> seg(a);

std::cout << seg.query(1, 4) << '\n';  // 输出: 9 (2+3+4)
seg.update(2, 10);                      // 将a[2]修改为10
std::cout << seg.query(1, 4) << '\n';  // 输出: 16 (2+10+4)
```

### Dijkstra最短路

```cpp
#include "Graph/Dijkstra.hpp"

Dijkstra::Graph<int> g(n);
g.add_edge(u, v, w);  // 添加边u->v权重w

// 计算单源最短路
auto res = g.solve(start, INF);
std::cout << res.distances[target] << '\n';

// 获取最短路径
auto res = g.solve<true>(start, INF);
auto path = res.path(target);
for (int v : path) std::cout << v << " ";
```

### 快速素数筛

```cpp
#include "Math/Sieve.hpp"

// 10^7范围内筛素数
EratosthenesSieve<10000000> primes;
std::cout << primes.size() << '\n';          // 素数个数
std::cout << primes.kth(99) << '\n';         // 第100个素数
std::cout << primes.contains(1009) << '\n';  // 是否为素数

// 欧拉筛计算φ函数
PhiSieve phi_sieve(1000000);
std::cout << phi_sieve.phi(100) << '\n';     // φ(100) = 40
```

### 自动对拍验证

```cpp
#include "Misc/对拍.hpp"

auto gen = [](auto cout) {
    int n = range(1, 100);
    cout << n << '\n';
    for (int i = 0; i < n; i++) {
        cout << range(1, 1000) << " ";
    }
};

auto brute = [](auto cin, auto cout) {
    // O(n²) 暴力解法
};

auto fast = [](auto cin, auto cout) {
    // O(n log n) 优化解法  
};

// 对拍1000组随机数据
compare(gen, brute, fast, 1000);
```

### 调试输出

```cpp
#include "debug.hpp"

std::vector<int> vec{1, 2, 3};
std::map<int, std::string> mp{{1, "one"}, {2, "two"}};

debug(), vec, mp;  // 自动输出容器内容
```

## 编译说明

使用 C++20 标准编译：

```bash
g++ -std=c++20 -O2 solution.cpp -o solution
```