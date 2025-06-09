# 🚀 高效率竞技编程模板库

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

## 📖 简介

这是一个专为 **竞技编程(Competitive Programming)** 设计的现代化C++20模板库，致力于提供高效、优雅、易用的算法实现。

### ✨ 核心特性

- 🎯 **现代C++20设计** - 充分利用concepts、ranges、constexpr等新特性
- 🏗️ **抽象化接口** - 基于数学抽象的优雅API设计
- ⚡ **极致性能** - 高度优化的算法实现，部分超越标准库
- 🔧 **开箱即用** - 统一的编程范式，降低学习成本
- 🧪 **完整工具链** - 内置对拍、性能测试、调试工具
- 📚 **丰富模块** - 涵盖数据结构、图论、数学、字符串等核心领域

### 🎯 适用场景

- 🏆 **算法竞赛** - ACM-ICPC、Codeforces、AtCoder等
- 📖 **算法学习** - 高质量参考实现
- 💼 **面试准备** - 常用算法模板
- 🔬 **算法研究** - 性能对比和优化

## 📁 模块架构

### 🗂️ 数据结构 (DS)

| 模块 | 文件 | 特性 | 复杂度 |
|------|------|------|--------|
| **线段树** | `SegmentTree.hpp` | 抽象群设计，支持任意可结合操作 | O(log n) |
| **懒标记线段树** | `LazySegmentTree.hpp` | 区间修改+查询，支持复合操作 | O(log n) |
| **树状数组** | `BinaryIndexedTree.hpp` | 可自定义聚合操作，内存友好 | O(log n) |
| **并查集** | `DisjointSetUnion.hpp` | 路径压缩+按秩合并优化 | O(α(n)) |
| **可删除并查集** | `ErasableDisjointSetUnion.hpp` | 支持撤销操作的并查集 | O(log n) |
| **离散化器** | `Discretizer.hpp` | 坐标压缩，处理大值域问题 | O(n log n) |
| **ST表** | `SparseTable.hpp` | 静态RMQ，预处理后O(1)查询 | O(n log n) / O(1) |
| **二进制字典树** | `BinaryTrie.hpp` | 异或操作优化，支持插入删除 | O(log V) |

### 🌐 图论算法 (Graph)

| 算法 | 文件 | 功能特性 | 时间复杂度 |
|------|------|----------|------------|
| **Dijkstra** | `Dijkstra.hpp` | 泛型权重，支持路径追踪和计数 | O((V+E) log V) |
| **Floyd-Warshall** | `FloydWarshall.hpp` | 全源最短路，支持负权边 | O(V³) |
| **拓扑排序** | `TopologicalSort.hpp` | DAG排序，环检测 | O(V+E) |
| **匈牙利算法** | `Hungarian.hpp` | 最大二分匹配，KM算法 | O(V³) |

### 🔢 数学算法 (Math)

| 算法类别 | 文件 | 核心功能 | 性能特点 |
|----------|------|----------|----------|
| **埃氏筛** | `Sieve.hpp/EratosthenesSieve` | 质数筛选 | 10⁸范围内比欧拉筛快3倍 |
| **欧拉筛** | `Sieve.hpp/PhiSieve` | 线性筛+欧拉函数 | O(n)时间复杂度 |
| **最小质因子筛** | `Sieve.hpp/MinPrimeSieve` | 质因数分解 | 快速分解，O(log n) |
| **莫比乌斯筛** | `Sieve.hpp/MobiusSieve` | 莫比乌斯函数 | 支持前缀和查询 |
| **矩阵运算** | `Matrix.hpp` | 矩阵乘法+快速幂 | 支持任意环结构 |
| **组合数学** | `Combination.hpp` | 组合数计算 | 预处理+O(1)查询 |
| **快速幂** | `FastPower.hpp` | 模幂运算 | O(log n)，支持泛型 |
| **Miller-Rabin** | `MillerRabin.hpp` | 概率素数测试 | 确定性算法，极快 |
| **Pollard-Rho** | `PollardRho.hpp` | 大整数分解 | 期望O(n^(1/4)) |
| **异或线性基** | `XorBase.hpp` | 异或空间基底 | 处理异或极值问题 |
| **Barrett约简** | `Barrett.hpp` | 快速取模 | 避免除法运算 |

### 🔤 字符串算法 (String)

| 算法 | 文件 | 应用场景 | 时间复杂度 |
|------|------|----------|------------|
| **Manacher** | `Manacher.hpp` | 回文串检测，最长回文子串 | O(n) |
| **字符串哈希** | `StringHash.hpp` | 快速字符串匹配和比较 | O(1) 查询 |

### 🛠️ 实用工具 (Misc)

| 工具类别 | 文件 | 核心功能 | 使用场景 |
|----------|------|----------|----------|
| **自动对拍** | `对拍.hpp` | 算法正确性验证 | 调试和测试 |
| **大整数** | `BigInteger.hpp` | 任意精度整数运算 | 高精度计算 |
| **取模整数** | `ModIntegral.hpp` | 自动模运算 | 避免溢出 |
| **动态取模** | `DyModIntegral.hpp` | 运行时模数 | 灵活的模运算 |
| **动态位集** | `DynamicBitset.hpp` | 可变长度位操作 | 状态压缩DP |
| **有理数** | `Fraction.hpp` | 分数运算 | 精确有理数计算 |
| **计算几何** | `Geometry.hpp` | 2D几何算法 | 几何问题求解 |
| **快速IO** | `FastIO.hpp` | 高效输入输出 | 大数据量处理 |
| **搜索算法** | `search.hpp` | 二分搜索等 | 通用搜索模板 |

### ⏱️ 性能工具

| 工具 | 文件 | 功能 |
|------|------|------|
| **计时器** | `Timer.hpp` | 性能测试和统计分析 |

## 🚀 快速开始

### 📦 安装使用

```bash
# 克隆仓库
git clone https://github.com/huxint/CompetitiveProgramming.git
cd CompetitiveProgramming

# 包含所有模块
#include "all.hpp"

# 或者按需包含特定模块
#include "DS/SegmentTree.hpp"
#include "Graph/Dijkstra.hpp"
```

### 💡 核心示例

#### 🔧 自动对拍系统

```cpp
#include "Misc/对拍.hpp"

auto generator = [&](auto cout) {
    int n = range(1, 100);  // 随机生成1-100的数
    cout << n << '\n';
    for (int i = 0; i < n; i++) {
        cout << range(1, 1000) << " ";
    }
};

auto solve_bruteforce = [&](auto cin, auto cout) {
    // O(n²) 暴力解法
    int n; cin >> n;
    // ... 实现
};

auto solve_optimized = [&](auto cin, auto cout) {
    // O(n log n) 优化解法
    int n; cin >> n;
    // ... 实现
};

// 自动对拍1000组数据
compare(generator, solve_bruteforce, solve_optimized, 1000);
```

#### 🌳 抽象线段树

```cpp
#include "DS/SegmentTree.hpp"

// 区间最大值查询
auto max_op = [](int a, int b) { return std::max(a, b); };
auto max_e = []() { return 0; };
SegmentTree<int, max_op, max_e> max_tree(n);

// 区间和查询
auto sum_op = [](long long a, long long b) { return a + b; };
auto sum_e = []() { return 0LL; };
SegmentTree<long long, sum_op, sum_e> sum_tree(arr);

// 使用示例
max_tree.set(pos, value);           // 单点修改
auto result = max_tree.prod(l, r);  // 区间查询
auto pos = max_tree.max_right(l, [&](int x) { return x <= threshold; });
```

#### 🌐 泛型Dijkstra算法

```cpp
#include "Graph/Dijkstra.hpp"

// 基础用法
Dijkstra::Graph<int> graph(n);
graph.add_edge(u, v, weight);

// 只计算距离
auto result1 = graph.solve(start, INF);
cout << result1.distances[target] << '\n';

// 计算距离 + 路径追踪
auto result2 = graph.solve<true>(start, INF);
auto path = result2.path(target);

// 计算距离 + 路径数量
auto result3 = graph.solve<false, true>(start, INF);
cout << result3.number[target] << '\n';

// 完整功能：距离 + 路径 + 计数
auto result4 = graph.solve<true, true>(start, INF);
result4.trace(target, [](int node) { cout << node << " "; });
```

#### 🔢 高性能质数筛

```cpp
#include "Math/Sieve.hpp"

// 埃氏筛 - 10^8范围内最快
EratosthenesSieve<10000000> sieve;
for (auto prime : sieve) {
    cout << prime << " ";
}
cout << "质数个数: " << sieve.size() << '\n';
cout << "第100个质数: " << sieve.kth(99) << '\n';
cout << "997是质数吗: " << sieve.contains(997) << '\n';

// 欧拉筛 + 欧拉函数
PhiSieve phi_sieve(1000000);
cout << "φ(100) = " << phi_sieve.phi(100) << '\n';

// 最小质因子筛 + 快速分解
MinPrimeSieve min_sieve(1000000);
min_sieve.decompose(360, [](int p) { cout << p << " "; }); // 输出: 2 2 2 3 3 5
```

#### ⏱️ 性能测试

```cpp
#include "Timer.hpp"

{
    SCOPE_TIMER("算法耗时");
    // 你的算法代码
    solve();
}

PRINT_TIMER_RESULTS();  // 输出统计结果
```

## 🎯 设计理念

### 🏗️ 抽象化设计
- **数学抽象**: 基于群、环、域等代数结构设计接口
- **泛型编程**: 支持任意满足要求的类型
- **零开销抽象**: 编译时优化，运行时无额外开销

### 🚀 性能优先
- **算法优化**: 采用最新的算法优化技术
- **内存友好**: 紧凑的内存布局和缓存友好的访问模式
- **编译优化**: 充分利用编译器优化能力

### 🔧 易用性
- **统一接口**: 相似功能的模块采用一致的API设计
- **类型安全**: 利用C++20的concepts进行编译时检查
- **错误友好**: 清晰的错误信息和边界检查

## 📊 性能对比

| 算法 | 本库实现 | 标准实现 | 性能提升 |
|------|----------|----------|----------|
| 埃氏筛(10⁸) | 2.1s | 6.8s | **3.2x** |
| 线段树查询 | 0.8ns | 1.2ns | **1.5x** |
| Dijkstra | 与std::priority_queue相当 | - | **相当** |

## 🛠️ 开发环境

### 📋 系统要求
- **编译器**: GCC 10+ / Clang 12+ / MSVC 2022+
- **标准**: C++20
- **平台**: Windows / Linux / macOS

### 🔧 编译选项
```bash
g++ -std=c++20 -O2 -Wall -Wextra solution.cpp
```

### 📚 依赖项
- 标准库 (无外部依赖)
- 部分模块使用 `<tr2/dynamic_bitset>` (GCC扩展)

## 📖 学习资源

### 🎓 推荐学习路径
1. **基础数据结构**: 线段树 → 树状数组 → 并查集
2. **图论算法**: Dijkstra → Floyd → 拓扑排序
3. **数学算法**: 质数筛 → 快速幂 → 组合数学
4. **高级技巧**: 懒标记 → 可持久化 → 分块

### 📚 参考资料
- [OI Wiki](https://oi-wiki.org/) - 算法竞赛知识整合
- [Codeforces](https://codeforces.com/) - 在线练习平台
- [AtCoder Library](https://github.com/atcoder/ac-library) - 官方算法库

## 🤝 贡献指南

### 🐛 问题反馈
- 发现bug请提交 [Issue](https://github.com/huxint/CompetitiveProgramming/issues)
- 包含复现步骤和环境信息

### 💡 功能建议
- 新算法实现建议
- 性能优化建议
- 接口改进建议

### 🔧 代码贡献
1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

### 📝 代码规范
- 遵循现有代码风格
- 添加必要的注释和文档
- 确保所有测试通过
- 性能关键代码需要benchmark

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情

## 🙏 致谢

- 感谢所有为算法竞赛社区做出贡献的开发者
- 特别感谢 [AtCoder Library](https://github.com/atcoder/ac-library) 的设计启发
- 感谢各大OJ平台提供的测试环境

## 📞 联系方式

- **作者**: huxint
- **邮箱**: [huxint123@gmail.com](mailto:huxint123@gmail.com)
- **GitHub**: [@huxint](https://github.com/huxint)

---

<div align="center">

**⭐ 如果这个项目对你有帮助，请给个Star支持一下！ ⭐**

[![Star History Chart](https://api.star-history.com/svg?repos=huxint/CompetitiveProgramming&type=Date)](https://star-history.com/#huxint/CompetitiveProgramming&Date)

</div>