#pragma once
#include <cassert>
#include <chrono>
#include <cstdint>
#include <random>

inline std::mt19937_64 rng(
    std::uint64_t(std::chrono::steady_clock::now().time_since_epoch().count())
    ^ std::uint64_t(reinterpret_cast<std::uintptr_t>(&rng)));

inline long long rnd(long long l, long long r) {
    assert(l <= r);
    return std::uniform_int_distribution<long long>(l, r)(rng);
}
