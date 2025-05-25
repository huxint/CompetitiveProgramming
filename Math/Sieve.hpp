#pragma once
#include <cmath>
#include <vector>
#include <bitset>
#include <cstdint>
#include <numeric>

template <std::uint32_t range>
class EratosthenesSieve {
    using size_type = std::uint32_t;
public:
    EratosthenesSieve(size_type max = range) {
        visited.flip();
        primes.reserve(static_cast<size_type>(max / std::log(max)));
        for (size_type i = 3; i * i <= max; i = 2 * visited._Find_next((i - 1) >> 1) + 1) {
            for (size_type j = i * i; j <= max; j += 2 * i) {
                visited.reset((j - 1) >> 1);
            }
        }
        primes.emplace_back(2);
        for (size_type last = 1; 2 * last + 1 <= max; last = visited._Find_next(last)) {
            primes.emplace_back(2 * last + 1);
        }
    }

    auto begin() const {
        return primes.begin();
    }

    auto end() const {
        return primes.end();
    }

    std::size_t size() const {
        return primes.size();
    }

    size_type kth(std::size_t k) const {
        return primes.at(k);
    }

    bool contains(size_type value) const {
        return value % 2 == 0 ? value == 2 : visited.test((value - 1) >> 1);
    }

private:
    std::vector<size_type> primes;
    std::bitset<range / 2 + 1> visited;
};

class PhiSieve {
    using size_type = std::uint32_t;
public:
    PhiSieve(size_type max) {
        _phi.assign(max + 1, 1);
        primes.reserve(static_cast<size_type>(max / std::log(max)));
        for (size_type i = 2; i <= max; ++i) {
            if (_phi[i] == 1) {
                _phi[i] = i - 1;
                primes.push_back(i);
            }
            for (const auto &p : primes) {
                if (i * p > max) {
                    break;
                }
                if (i % p == 0) {
                    _phi[i * p] = _phi[i] * p;
                    break;
                }
                _phi[i * p] = _phi[i] * (p - 1);
            }
        }
    }

    auto begin() const {
        return primes.begin();
    }

    auto end() const {
        return primes.end();
    }

    std::size_t size() const {
        return primes.size();
    }

    size_type kth(std::size_t k) const {
        return primes.at(k);
    }

    bool contains(size_type value) const {
        return _phi.at(value) == value - 1;
    }

    size_type phi(size_type value) const {
        return _phi.at(value);
    }

private:
    std::vector<size_type> _phi;
    std::vector<size_type> primes;
};

class MinPrimeSieve {
    using size_type = std::uint32_t;
public:
    MinPrimeSieve(size_type max) {
        _minp.assign(max + 1, 0);
        primes.reserve(static_cast<size_type>(max / std::log(max)));
        for (size_type i = 2; i <= max; ++i) {
            if (_minp[i] == 0) {
                primes.push_back(_minp[i] = i);
            }
            for (const auto &p : primes) {
                if (i * p > max) {
                    break;
                }
                if (_minp[i * p] = p; _minp[i] == p) {
                    break;
                }
            }
        }
    }
    
    void decompose(size_type value, auto &&call) const {
        while (value > 1) {
            auto p = minp(value);
            while (minp(value) == p) {
                std::forward<decltype(call)>(call)(p);
                value /= p;
            }
        }
    }

    auto begin() const {
        return primes.begin();
    }

    auto end() const {
        return primes.end();
    }

    std::size_t size() const {
        return primes.size();
    }

    size_type kth(std::size_t k) const {
        return primes.at(k);
    }

    bool contains(size_type value) const {
        return _minp.at(value) == value;
    }

    size_type minp(size_type value) const {
        return _minp.at(value);
    }

private:
    std::vector<size_type> _minp;
    std::vector<size_type> primes;
};

class MobiusSieve {
    using size_type = std::uint32_t;
public:
    MobiusSieve(size_type max) {
        mobius.assign(max + 1, 0);
        isprime.assign(max + 1, true);
        primes.reserve(static_cast<size_type>(max / std::log(max > 1 ? max : 2)));
        mobius[1] = 1;
        isprime[1] = false;
        for (size_type i = 2; i <= max; ++i) {
            if (isprime[i]) {
                primes.push_back(i);
                mobius[i] = -1;
            }
            for (const auto &p : primes) {
                if (i * p > max) {
                    break;
                }
                isprime[i * p] = false;
                if (i % p == 0) {
                    mobius[i * p] = 0;
                    break;
                } else {
                    mobius[i * p] = -mobius[i];
                }
            }
        }
        std::partial_sum(mobius.begin(), mobius.end(), mobius.begin());
    }

    auto begin() const {
        return primes.begin();
    }

    auto end() const {
        return primes.end();
    }

    std::int32_t query(size_type value) const {
        return mobius.at(value) - mobius.at(value - 1);
    }

    std::int32_t query(size_type l, size_type r) const {
        return mobius.at(r) - mobius.at(l - 1);
    }

    bool contains(size_type value) const {
        return isprime.at(value);
    }

    std::size_t size() const {
        return primes.size();
    }

    size_type kth(std::size_t k) const {
        return primes.at(k);
    }

private:
    std::vector<bool> isprime;
    std::vector<std::int32_t> mobius;
    std::vector<size_type> primes;
};