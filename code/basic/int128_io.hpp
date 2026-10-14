#pragma once
#include <algorithm>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

using i128 = __int128;
using u128 = unsigned __int128;

inline std::istream& operator>>(std::istream& in, i128& x) {
    std::string s;
    if (!(in >> s)) return in;
    std::size_t p = 0;
    bool neg = s[0] == '-';
    if (s[0] == '-' || s[0] == '+') ++p;
    const u128 limit = (u128{1} << 127) - (neg ? 0 : 1);
    u128 value = 0;
    bool valid = p < s.size();
    for (; p < s.size(); ++p) {
        if (s[p] < '0' || s[p] > '9') {
            valid = false;
            break;
        }
        unsigned digit = unsigned(s[p] - '0');
        if (value > (limit - digit) / 10) {
            valid = false;
            break;
        }
        value = value * 10 + digit;
    }
    if (!valid) {
        in.setstate(std::ios::failbit);
        return in;
    }
    // -(i128(1) << 127) and -MIN overflow; construct negatives via value - 1.
    x = neg && value ? -i128(value - 1) - 1 : i128(value);
    return in;
}

inline std::ostream& operator<<(std::ostream& out, i128 x) {
    if (x == 0) return out << '0';
    u128 value = x < 0 ? -u128(x) : u128(x);
    if (x < 0) out << '-';
    std::string s;
    while (value) {
        s.push_back(char('0' + value % 10));
        value /= 10;
    }
    std::reverse(s.begin(), s.end());
    return out << s;
}
