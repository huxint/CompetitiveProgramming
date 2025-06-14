#pragma once
#include "iostream.hpp"

namespace debug {
    void dbg(const char *name, auto &&arg) {
        std::cerr << '{' << name << ": " << arg << "}\n";
    }

    void dbg(const char *names, auto &&arg, auto &&...args) {
        const char *comma = std::strchr(names + 1, ',');
        std::cerr << '{';
        std::cerr.write(names, comma - names) << ": " << arg << "}, ";
        dbg(comma + 2, args...);
    }
}
#define debug(...) debug::dbg(#__VA_ARGS__, __VA_ARGS__)