#include <cstdint>

std::uint64_t prexor(std::uint64_t x) {
    return x % 4 % 2 == 1 ? x % 4 == 1 : x + (x % 4 != 0);
}