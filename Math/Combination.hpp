#pragma once
#include <vector>
#include <cstdint>
#include <numeric>
#include <algorithm>

/*
组合数，需要配合 取模类

Combination<ModIntegral<998244353U, true>> comb;

直接用 comb.combination(n, m) 即可
即可完成不需要你手动初始化

支持求阶乘、逆阶乘、组合数、排列数、卡特兰数、多重排列、圆排列、圆排列（带环）、错排、非相邻组合、卢卡斯定理
*/
template <typename Base>
    requires requires (Base object) {
    object.mod();
    object.power(0);
    object.inverse();
}
class Combination {
public:
    Combination() : _max{}, _inverse{1}, _factorial{1}, _ifactorial{1} {}

    Combination(std::size_t max) : Combination() {
        init(max);
    }

    void init(std::size_t max) {
        if (max > Base::mod() - 1) {
            max = Base::mod() - 1;
        }
        if (max <= _max) {
            return;
        }
        _inverse.resize(max + 1);
        _factorial.resize(max + 1);
        _ifactorial.resize(max + 1);
        for (std::size_t i = _max + 1; i <= max; ++i) {
            _factorial[i] = i * _factorial[i - 1];
        }
        _ifactorial[max] = _factorial[max].inverse();
        for (std::size_t i = max; i > _max; --i) {
            _ifactorial[i - 1] = i * _ifactorial[i];
            _inverse[i] = _ifactorial[i] * _factorial[i - 1];
        }
        _max = max;
    }

    Base factorial(std::uint32_t value) {
        if (value > _max) {
            init(value << 1);
        }
        return _factorial[value];
    }

    Base inverse(std::uint32_t value) {
        if (value > _max) {
            init(value << 1);
        }
        return _inverse[value];
    }

    Base ifactorial(std::uint32_t value) {
        if (value > _max) {
            init(value << 1);
        }
        return _ifactorial[value];
    }

    Base combination(std::uint32_t n, std::uint32_t m) {
        if (n <= m) {
            return n == m;
        }
        return factorial(n) * ifactorial(m) * ifactorial(n - m);
    }

    Base permutation(std::uint32_t n, std::uint32_t m) {
        if (n < m) {
            return 0;
        }
        return factorial(n) * ifactorial(n - m);
    }

    /**
     * 卡特兰数
     * 1 1 2 5 14 42 132 ...
     */
    Base catalan(std::uint32_t n) {
        return combination(n << 1, n) * inverse(n + 1);
    }

    /**
     * 多重排列
     * 例: 输入 `[2, 3, 5]` 这个数组 ，查询两个红球，三个蓝球，五个黄球，共多少种排列方式
     */
    template <typename Iterator>
    Base permutation_multi(Iterator begin, Iterator end) {
        Base res = factorial(std::accumulate(begin, end, 0));
        for (auto it = begin; it != end; ++it) {
            res *= ifactorial(*it);
        }
        return res;
    }

    /**
     * 圆排列
     * 对 `n` 个圆上的不同元素进行排列的方案数
     */
    Base permutation_circular(std::uint32_t n) {
        return factorial(n - 1);
    }

    /**
     * 圆排列 (带环)
     * 从 `n` 个圆上的不同元素中选取 `m` 个进行排列的方案数
     */
    Base permutation_circular(std::uint32_t n, std::uint32_t m) {
        if (m == 0) {
            return 1;
        }
        return combination(n, m) * factorial(m - 1);
    }

    /**
     * 错排
     * `n` 个元素进行排列后，每个元素都和自己之前的旧位置不同的方案数
     */
    Base permutation_staggered(std::uint32_t n) {
        static std::vector<Base> res{1, 0};
        while (res.size() < n + 1) {
            res.push_back((res[res.size() - 2] + res.back()) * Base(res.size() - 1));
        }
        return res[n];
    }

    /**
     * `n` 个元素中选取出 `m` 个互不相邻的元素的方案数
     */
    Base combination_nonadjacent(std::uint32_t n, std::uint32_t m) {
        if (n < m) {
            return 0;
        }
        return combination(n - m + 1, m);
    }

    Base lucas(std::size_t n, std::size_t m) {
        return m == 0 ? 1 : lucas(n / Base::mod(), m / Base::mod()) * combination(n % Base::mod(), m % Base::mod());
    }

private:
    std::size_t _max;
    std::vector<Base> _inverse;
    std::vector<Base> _factorial;
    std::vector<Base> _ifactorial;
};