#pragma once
#include <concepts>

/*
辛普森积分
求函数 f(x) 在 [l, r] 的积分
支持误差设置和最大循环次数，防止精度导致的 TLE

函数 f 需要接受一个参数 x，返回一个 T 类型的值，T 必须是浮点数
*/
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