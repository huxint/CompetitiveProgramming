# 高效率 CP 模板库

## 简介

这是一个为竞赛编程(Competitive Programming)设计的高效模板库，具有以下特点：

- 代码风格简洁易懂
- 接口设计优雅
- 性能高效
- 易于使用和修改

## 主要模块

### 数据结构 (DS)

- **线段树** (`SegmentTree.hpp`): 抽象设计，只需传入抽象群 `info, op, e` 即可
- **懒标记线段树** (`LazySegmentTree.hpp`): 需传入 `id, Composition, Mapping, Function`
- **树状数组** (`BinaryIndexedTree.hpp`): 可通过重载加法自定义聚合操作
- **并查集** (`DisjointSetUnion.hpp`): 支持路径压缩和按秩合并
- **可删除并查集** (`ErasableDisjointSetUnion.hpp`): 支持删除操作
- **离散化** (`Discretizer.hpp`): 将值域较大的数据映射到连续空间
- **ST表** (`SparseTable.hpp`): 用于处理静态RMQ问题
- **二进制字典树** (`BinaryTrie.hpp`): 用于处理异或极值等问题

### 图论 (Graph)

- **Dijkstra** (`Dijkstra.hpp`): 支持抽象类型 `Group`，类型需重载比较运算符
- **Floyd-Warshall** (`FloydWarshall.hpp`): 全源最短路算法
- **拓扑排序** (`TopologicalSort.hpp`): 处理有向无环图
- **匈牙利算法** (`Hungarian.hpp`): 最大二分图匹配

### 数学 (Math)

- **埃氏筛** (`Sieve.hpp/EratosthenesSieve`): 高度优化，在 10^8 范围内比欧拉筛快近3倍
- **欧拉筛** (`Sieve.hpp/PhiSieve`): 线性时间筛选质数
- **最小质因子筛** (`Sieve.hpp/MinPrimeSieve`): 快速分解质因数
- **莫比乌斯筛** (`Sieve.hpp/MobiusSieve`): 处理莫比乌斯函数
- **矩阵** (`Matrix.hpp`): 矩阵运算与快速幂
- **组合数学** (`Combination.hpp`): 计算组合数
- **快速幂** (`FastPower.hpp`): 整数快速幂
- **Miller-Rabin** (`MillerRabin.hpp`): 素数测试
- **Pollard-Rho** (`PollardRho.hpp`): 大整数分解
- **异或基** (`XorBase.hpp`): 处理异或线性基问题

### 字符串 (String)

- **Manacher** (`Manacher.hpp`): 处理回文串问题
- **字符串哈希** (`StringHash.hpp`): 快速字符串匹配

### 其他 (Misc)

- **对拍** (`对拍.hpp`): 对比两种算法的正确性，只需传入三个函数即可
- **大整数** (`BigInteger.hpp`): 处理超出内置类型范围的整数
- **取模整数** (`ModIntegral.hpp`): 自动取模的整数类型
- **动态取模整数** (`DyModIntegral.hpp`): 运行时确定模数
- **动态位集** (`DynamicBitset.hpp`): 动态大小的位集合
- **分数** (`Fraction.hpp`): 处理有理数
- **几何库** (`Geometry.hpp`): 计算几何算法
- **快速IO** (`FastIO.hpp`): 高效输入输出

## 用法示例

### 对拍

```cpp
auto generator = [&](auto cout) {
    // 生成测试数据
};

auto solve1 = [&](auto cin, auto cout) {
    // 第一种解法
};

auto solve2 = [&](auto cin, auto cout) {
    // 第二种解法
};

compare(generator, solve1, solve2, 1000); // 对拍1000组数据
```

### 线段树

```cpp
// 定义节点信息、操作和单位元
auto op = [](int a, int b) -> int {
    return std::max(a, b);
};
auto e = []() -> int {
    return 0;
};
// 第一种创建线段树
SegmentTree<int, op, e> st(n);

// 第二种创建线段树
std::vector<int> a(n);
SegmentTree<int, std::ranges::max, e> st(a.begin(), a.end());
```

### Dijkstra算法

```cpp
Dijkstra::Graph<int> g(n);  // 创建图，边权为int类型
g.add_edge(u, v, w);  // 添加边
auto result = g.solve<true, true>(s, INF);  // 计算从s出发的最短路，带路径和计数
```

### 埃氏筛

```cpp
EratosthenesSieve<10000000> sieve;
for (auto prime : sieve) {
    // 使用质数
}
```

- 所有代码均使用C++20标准
- 接口设计注重易用性和扩展性
- 使用前请先了解相关算法原理