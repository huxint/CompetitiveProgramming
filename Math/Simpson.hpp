/**
 * @brief 辛普森积分算法
 * @details 使用自适应辛普森公式计算函数在指定区间的定积分
 *          支持误差控制和最大迭代次数限制，防止精度问题导致TLE
 *          函数f需要接受一个参数x，返回浮点数类型的值
 * @author huxint
 */
#pragma once
#include <concepts>
template <std::floating_point T>
constexpr auto integral(T l, T r, auto &&f, auto eps, std::size_t loop = 30) {
    auto simpson = [&f](auto l, auto r) -> auto {
        return (f(l) + 4 * f((l + r) / 2) + f(r)) * (r - l) / 6;
    };
    auto running = [&](auto &&self, auto l, auto r, auto s, auto eps, std::size_t loop) -> T {
        auto mid = (l + r) / 2;
        auto sl = simpson(l, mid);
        auto sr = simpson(mid, r);
        return loop == 0 or std::abs(sl + sr - s) < 15 * eps ? sl + sr + (sl + sr - s) / 15 : self(self, l, mid, sl, eps / 2, loop - 1) + self(self, mid, r, sr, eps / 2, loop - 1);
    };
    return running(running, l, r, simpson(l, r), eps, loop);
}