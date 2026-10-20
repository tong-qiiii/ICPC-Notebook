#pragma once
#include <cassert>
#include <chrono>
#include <cstdint>
#include <random>

inline std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

inline long long rnd(long long l, long long r)
{
    assert(l <= r);
    return std::uniform_int_distribution<long long>(l, r)(rng);
}
