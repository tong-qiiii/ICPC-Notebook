#pragma once
#include <cassert>
#include <cstdint>

template <class F>
void for_each_mask(int n, F visit) {
    assert(0 <= n && n < 64);
    const std::uint64_t end = std::uint64_t{1} << n;
    for (std::uint64_t mask = 0; mask < end; ++mask) visit(mask);
}

template <class F>
void for_each_set_bit(std::uint64_t mask, F visit) {
    for (auto s = mask; s; s &= s - 1) {
        // __builtin_ctzll(0) is undefined.
        visit(__builtin_ctzll(s));
    }
}

template <class F>
void for_each_submask(std::uint64_t mask, F visit,
                      bool include_empty = true) {
    for (auto sub = mask;; sub = (sub - 1) & mask) {
        if (sub || include_empty) visit(sub);
        if (sub == 0) break;
    }
}

template <class F>
void for_each_mask_submask(int n, F visit, bool include_empty = false) {
    for_each_mask(n, [&](std::uint64_t mask) {
        for_each_submask(mask, [&](std::uint64_t sub) {
            visit(mask, sub);
        }, include_empty);
    });
}
