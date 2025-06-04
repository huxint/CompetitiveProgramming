#pragma once
#include <bits/stdc++.h>

namespace fs = std::filesystem;
using ofs = std::ofstream;
using ifs = std::ifstream;
using fsp = fs::path;
const fsp dir = "data.txt";
const fsp out1 = "out1.txt";
const fsp out2 = "out2.txt";

std::mt19937_64 random(std::chrono::steady_clock::now().time_since_epoch().count());
auto range(std::integral auto left, std::integral auto right) {
    return random() % (right - left + 1) + left;
}

void compare(auto generator, auto solve1, auto solve2, std::size_t test = 100, bool flush = false) {
    bool ok = true;
    for (std::size_t i = 0; i < test; ++i) {
        generator(ofs(dir));
        solve1(ifs(dir), ofs(out1));
        solve2(ifs(dir), ofs(out2));
        ifs f1(out1, ifs::binary);
        ifs f2(out2, ifs::binary);
        if (std::equal(std::istreambuf_iterator<char>(f1), {}, std::istreambuf_iterator<char>(f2)) == false) {
            std::cout << "WA" << "\n";
            std::cout << "data: \n" << ifs(dir).rdbuf() << "\n";
            std::cout << "out1: \n" << ifs(out1).rdbuf() << "\n";
            std::cout << "out2: \n" << ifs(out2).rdbuf() << "\n";
            ok = false;
            break;
        } else {
            if (flush) {
                std::cout << "test: " << i << " AC!" << std::endl;
            }
        }
    }
    if (ok) {
        std::cout << "All AC" << std::endl;
    }
    fs::remove(out1);
    fs::remove(out2);
    fs::remove(dir);
};