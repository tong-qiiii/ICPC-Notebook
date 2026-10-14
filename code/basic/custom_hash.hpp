#pragma once
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>

struct CustomHash {
    static std::uint64_t splitmix64(std::uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    static std::uint64_t seed() {
        // Fixed for the process: changing the seed invalidates existing tables.
        static const std::uint64_t FIXED_RANDOM = std::uint64_t(
            std::chrono::steady_clock::now().time_since_epoch().count());
        return FIXED_RANDOM;
    }

    std::size_t operator()(std::uint64_t x) const {
        return std::size_t(splitmix64(x + seed()));
    }

    template <class T, class U>
    std::size_t operator()(const std::pair<T, U>& p) const {
        auto a = splitmix64(std::uint64_t(std::hash<T>{}(p.first)) + seed());
        auto b = splitmix64(std::uint64_t(std::hash<U>{}(p.second)) + seed());
        return std::size_t(splitmix64(a ^ (b + 0x9e3779b97f4a7c15ULL
                                          + (a << 6) + (a >> 2))));
    }
};
