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