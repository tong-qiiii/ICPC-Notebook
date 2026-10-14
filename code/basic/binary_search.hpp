#pragma once
#include <algorithm>
#include <vector>

// Ascending vector; size must fit in int. n means no successor, -1 no predecessor.
template <class T, class U>
int lower_index(const std::vector<T>& a, const U& x) {
    return int(std::lower_bound(a.begin(), a.end(), x) - a.begin());
}

template <class T, class U>
int upper_index(const std::vector<T>& a, const U& x) {
    return int(std::upper_bound(a.begin(), a.end(), x) - a.begin());
}

template <class T, class U>
int last_less_index(const std::vector<T>& a, const U& x) {
    return lower_index(a, x) - 1;
}

template <class T, class U>
int last_less_equal_index(const std::vector<T>& a, const U& x) {
    return upper_index(a, x) - 1;
}
