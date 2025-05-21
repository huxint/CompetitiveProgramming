#pragma once
#include <algorithm>
#include <cstdint>

class Barrett32 {
private:
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using u128 = unsigned __int128;

public:
    constexpr Barrett32(u32 _mod) : modular(_mod), inverse(static_cast<u64>(-1) / _mod + 1) {}

    constexpr void setmod(u32 _mod) {
        modular = _mod;
        inverse = static_cast<u64>(-1) / modular + 1;
    }

    constexpr u32 mod() const {
        return modular;
    }

    constexpr u32 multiply(u32 lhs, u32 rhs) const {
        u64 value = static_cast<u64>(lhs) * rhs;
        u32 res = static_cast<u32>(value - static_cast<u64>((static_cast<u128>(value) * inverse) >> 64) * mod());
        return res >= mod() ? res += mod() : res;
    }

private:
    u32 modular;
    u64 inverse;
};