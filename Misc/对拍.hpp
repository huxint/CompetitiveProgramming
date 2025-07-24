/**
 * @brief 对拍工具
 * @details 用于算法正确性验证的对拍工具，支持随机数据生成和多解法比较
 *          提供随机数生成器和自动化测试框架
 * @author huxint
 */
#include <bits/stdc++.h>

namespace fs = std::filesystem;
using ofs = std::ofstream;
using ifs = std::ifstream;
fs::path out1 = "out1.txt";
fs::path out2 = "out2.txt";
fs::path data = "data.txt";

std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

auto range(std::integral auto left, std::integral auto right) {
    return std::uniform_int_distribution(left, right)(rng);
}

auto range(std::floating_point auto left, std::floating_point auto right) {
    return std::uniform_real_distribution(left, right)(rng);
}

void compare(auto &&generator, auto &&solve1, auto &&solve2, std::size_t test = 100, bool flush = false) {
    struct Cleaner {
        ~Cleaner() {
            fs::remove(out1);
            fs::remove(out2);
            fs::remove(data);
        };
    } cleaner;
    for (std::size_t i = 1; i <= test; ++i) {
        generator(ofs(data));
        solve1(ifs(data), ofs(out1));
        solve2(ifs(data), ofs(out2));
        ifs f1(out1, ifs::binary);
        ifs f2(out2, ifs::binary);
        if (std::equal(std::istreambuf_iterator<char>(f1), {}, std::istreambuf_iterator<char>(f2)) == false) {
            std::cout << "WA!" << "\n";
            std::cout << "data: \n" << ifs(data).rdbuf() << "\n";
            std::cout << "out1: \n" << ifs(out1).rdbuf() << "\n";
            std::cout << "out2: \n" << ifs(out2).rdbuf() << "\n";
            return;
        } else if (flush) {
            std::cout << "test: " << i << " AC!" << std::endl;
        }
    }
    std::cout << "All AC!" << std::endl;
};