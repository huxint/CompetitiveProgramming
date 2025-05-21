#pragma once
#include <bits/stdc++.h>

namespace fs = std::filesystem;
using ofs = std::ofstream;
using ifs = std::ifstream;
using fsp = fs::path;
const fsp dir = "E:/huxint/Competition";
const fsp _data = dir / "data.txt";
const fsp out1 = dir / "out1.txt";
const fsp out2 = dir / "out2.txt";

std::mt19937_64 random(std::chrono::steady_clock::now().time_since_epoch().count());
auto range(std::integral auto left, std::integral auto right) {
    return random() % (right - left + 1) + left;
}

bool compare(auto &&generator, auto &&solve1, auto &&solve2, std::size_t test = 100) {
    bool ok = true;
    while (test--) {
        generator(ofs(_data));
        solve1(ifs(_data), ofs(out1));
        solve2(ifs(_data), ofs(out2));
        ifs f1(out1, ifs::binary);
        ifs f2(out2, ifs::binary);
        if (std::equal(std::istreambuf_iterator<char>(f1), {}, std::istreambuf_iterator<char>(f2)) == false) {
            ok = false;
            break;
        }
    }
    if (ok) {
        std::cout << "AC" << "\n";
    } else {
        std::cout << "WA" << "\n";
        std::cout << "data: \n" << ifs(_data).rdbuf() << "\n";
        std::cout << "out1: \n" << ifs(out1).rdbuf() << "\n";
        std::cout << "out2: \n" << ifs(out2).rdbuf() << "\n";
    }
    fs::remove(out1);
    fs::remove(out2);
    fs::remove(_data);
    return ok;
};