#pragma once
#include <array>
#include <ranges>
#include <iosfwd>
#include <ostream>
#include <cstddef>
#include <concepts>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

template <typename T, std::size_t row, std::size_t col>
class Matrix {
public:
    constexpr Matrix() : matrix{} {}

    constexpr Matrix(const T &value) {
        set(value);
    }

    template <typename Mapping>
        requires std::invocable<Mapping, std::size_t, std::size_t>
    constexpr Matrix(Mapping &&mapping) {
        for (std::size_t i = 0; i < row; ++i) {
            for (std::size_t j = 0; j < col; ++j) {
                (*this)(i, j) = std::forward<Mapping>(mapping)(i, j);
            }
        }
    }

    constexpr Matrix(const std::ranges::range auto &container) : Matrix(std::forward<decltype(container)>(container).begin(), std::forward<decltype(container)>(container).end()) {}

    constexpr Matrix(std::initializer_list<std::initializer_list<T>> container) : Matrix(container.begin(), container.end()) {}

    template <typename Iterator>
    constexpr Matrix(Iterator begin, Iterator end) {
        for (auto data = matrix.data(); begin != end; ++begin, data += col) {
            std::copy(begin->begin(), begin->end(), data);
        }
    }

    constexpr auto set(const T &value) -> void {
        matrix.fill(value);
    }

    constexpr auto rows() const -> std::size_t {
        return row;
    }

    constexpr auto cols() const -> std::size_t {
        return col;
    }

    static constexpr auto unit() -> Matrix {
        static_assert(row == col);
        Matrix res;
        for (std::size_t i = 0; i < row; ++i) {
            res(i, i) = T{1};
        }
        return res;
    }

    constexpr auto power(std::size_t exp) const -> Matrix {
        static_assert(row == col);
        Matrix res(unit());
        for (Matrix base(*this); exp != 0; exp >>= 1, base = base * base) {
            if (exp & 1) {
                res = res * base;
            }
        }
        return res;
    }

    constexpr auto transpose() const -> Matrix<T, col, row> {
        Matrix<T, col, row> res;
        for (std::size_t i = 0; i < row; ++i) {
            for (std::size_t j = 0; j < col; ++j) {
                res(j, i) = (*this)(i, j);
            }
        }
        return res;
    }

    constexpr auto begin() -> decltype(auto) {
        return matrix.begin();
    }

    constexpr auto end() -> decltype(auto) {
        return matrix.end();
    }

    constexpr auto operator()(std::size_t i, std::size_t j) -> T & {
        return matrix.at(index(i, j));
    }

    constexpr auto operator()(std::size_t i, std::size_t j) const -> const T & {
        return matrix.at(index(i, j));
    }

    constexpr auto operator+=(const Matrix &other) -> Matrix & {
        for (std::size_t i = 0; i < row; ++i) {
            for (std::size_t j = 0; j < col; ++j) {
                (*this)(i, j) += other(i, j);
            }
        }
        return *this;
    }

    constexpr auto operator-=(const Matrix &other) -> Matrix & {
        for (std::size_t i = 0; i < row; ++i) {
            for (std::size_t j = 0; j < col; ++j) {
                (*this)(i, j) -= other(i, j);
            }
        }
        return *this;
    }

    constexpr auto operator*=(const T &other) -> Matrix & {
        std::transform(matrix.begin(), matrix.end(), matrix.begin(), [&](const auto &value) {
            return value * other;
        });
        return *this;
    }

    constexpr auto operator/=(const T &other) -> Matrix & {
        std::transform(matrix.begin(), matrix.end(), matrix.begin(), [&](const auto &value) {
            return value / other;
        });
        return *this;
    }

    friend constexpr auto operator+(const Matrix &lhs, const Matrix &rhs) -> Matrix {
        return Matrix(lhs) += rhs;
    }

    friend constexpr auto operator-(const Matrix &lhs, const Matrix &rhs) -> Matrix {
        return Matrix(lhs) -= rhs;
    }

    friend constexpr auto operator*(const Matrix &lhs, const T &rhs) -> Matrix {
        return Matrix(lhs) *= rhs;
    }

    friend constexpr auto operator*(const T &lhs, const Matrix &rhs) -> Matrix {
        return Matrix(rhs) *= lhs;
    }

    friend constexpr auto operator/(const Matrix &lhs, const T &rhs) -> Matrix {
        return Matrix(lhs) /= rhs;
    }

    template <std::size_t size>
    friend constexpr auto operator*(const Matrix &lhs, const Matrix<T, col, size> &rhs) -> Matrix<T, row, size> {
        Matrix<T, row, size> res;
        for (std::size_t i = 0; i < row; ++i) {
            for (std::size_t j = 0; j < col; ++j) {
                T value = lhs(i, j);
                if (value == T{}) {
                    continue;
                }
                for (std::size_t k = 0; k < size; ++k) {
                    res(i, k) += value * rhs(j, k);
                }
            }
        }
        return res;
    }

    friend constexpr auto operator==(const Matrix &lhs, const Matrix &rhs) -> bool {
        return lhs.matrix == rhs.matrix;
    }

    template <typename Ostream>
    friend constexpr auto operator<<(Ostream &ostream, const Matrix &value) -> Ostream & {
        for (std::size_t i = 0; i < row; ++i) {
            for (std::size_t j = 0; j < col; ++j) {
                ostream << value(i, j) << " \n"[j + 1 == col];
            }
        }
        return ostream;
    }

private:
    std::array<T, row * col> matrix;
    constexpr auto index(std::size_t i, std::size_t j) const -> std::size_t {
        return i * col + j;
    }
};