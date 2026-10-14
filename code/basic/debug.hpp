#pragma once

#ifdef LOCAL
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <class T> void debug_print(const T& x);
template <class A, class B> void debug_print(const std::pair<A, B>& x);
template <class T, class A> void debug_print(const std::vector<T, A>& x);
template <class T, class C, class A> void debug_print(const std::set<T, C, A>& x);
template <class K, class V, class C, class A>
void debug_print(const std::map<K, V, C, A>& x);
template <class T, class H, class E, class A>
void debug_print(const std::unordered_set<T, H, E, A>& x);
template <class K, class V, class H, class E, class A>
void debug_print(const std::unordered_map<K, V, H, E, A>& x);

// All overloads must be declared before recursion, for nested containers.
template <class T>
void debug_print(const T& x) { std::cerr << x; }

template <class A, class B>
void debug_print(const std::pair<A, B>& x) {
    std::cerr << '(';
    debug_print(x.first);
    std::cerr << ", ";
    debug_print(x.second);
    std::cerr << ')';
}

template <class Range>
void debug_print_range(const Range& x, char open, char close) {
    std::cerr << open;
    bool first = true;
    for (const auto& value : x) {
        if (!first) std::cerr << ", ";
        first = false;
        debug_print(value);
    }
    std::cerr << close;
}

template <class T, class A>
void debug_print(const std::vector<T, A>& x) { debug_print_range(x, '[', ']'); }

template <class T, class C, class A>
void debug_print(const std::set<T, C, A>& x) { debug_print_range(x, '{', '}'); }

template <class K, class V, class C, class A>
void debug_print(const std::map<K, V, C, A>& x) { debug_print_range(x, '{', '}'); }

template <class T, class H, class E, class A>
void debug_print(const std::unordered_set<T, H, E, A>& x) {
    debug_print_range(x, '{', '}');
}

template <class K, class V, class H, class E, class A>
void debug_print(const std::unordered_map<K, V, H, E, A>& x) {
    debug_print_range(x, '{', '}');
}

inline void debug_values() { std::cerr << '\n'; }

template <class T, class... Ts>
void debug_values(const T& x, const Ts&... xs) {
    debug_print(x);
    if constexpr (sizeof...(xs)) std::cerr << ", ";
    debug_values(xs...);
}

#define debug(...) do { \
    std::cerr << "[" << #__VA_ARGS__ << "] = "; \
    debug_values(__VA_ARGS__); \
} while (false)
#else
#define debug(...) ((void)0)
#endif
