#pragma once
#include <cstdint>

/*
没什么用的东西
前缀异或
一般都能背下来了
*/
std::uint64_t prexor(std::uint64_t x) {
    return x % 4 % 2 == 1 ? x % 4 == 1 : x + (x % 4 != 0);
}