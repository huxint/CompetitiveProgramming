/**
 * @brief 输入输出的重载
 */
#pragma once
#include <string_view>
#include <ranges>

namespace iostream {
    template <typename T>
    concept range_like = std::ranges::range<T> && !std::is_convertible_v<T, std::string_view>;
    template <typename T>
    concept range_tuple = range_like<T> || std::__is_tuple_like<T>::value && !std::ranges::range<T>;

    template <typename Istream>
    Istream &operator>>(Istream &istream, range_tuple auto &value) {
        if constexpr (range_like<decltype(value)>) {
            for (auto &e : value) {
                istream >> e;
            }
        } else {
            // clang-format off
            // 因为我觉得 这样写更清晰一些 clang-format 感觉差点意思
            std::apply([&istream](auto &&...args) {
                ((istream >> args), ...);
            }, value);
            // clang-format on
        }
        return istream;
    }

    template <typename Ostream>
    Ostream &operator<<(Ostream &ostream, const range_tuple auto &value) {
        if constexpr (range_like<decltype(value)>) {
            auto it = value.begin();
            for (ostream << *it++; it != value.end(); ++it) {
                ostream << ' ' << *it;
            }
        } else {
            // clang-format off
            ostream << '(' << (std::apply([&ostream](auto &&...args) {
                auto size = sizeof...(args);
                ((ostream << args << (--size ? ", " : "")), ...);
            }, value), ')');
            // clang-format on
        }
        return ostream;
    }
} // namespace iostream
using iostream::operator<<, iostream::operator>>;