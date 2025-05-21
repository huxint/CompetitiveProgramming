#pragma once
#include <algorithm>

class Barrett32 {
private:
    using u32 = unsigned;
    using u64 = unsigned long long;

public:
    constexpr Barrett32(u32 _mod) {
        setmod(_mod);
    }

    constexpr auto setmod(u32 _mod) -> void {
        modular = _mod;
        inverse = static_cast<u64>(-1) / modular + 1;
    }

    constexpr auto mod() const -> u32 {
        return modular;
    }

    constexpr auto multiply(u32 lhs, u32 rhs) const -> u32 {
        u64 value = static_cast<u64>(lhs) * rhs;
        u32 res = static_cast<u32>(value - static_cast<u64>((static_cast<unsigned __int128>(value) * inverse) >> 64) * mod());
        return res >= mod() ? res += mod() : res;
    }

private:
    u32 modular;
    u64 inverse;
};