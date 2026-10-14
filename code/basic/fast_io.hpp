#pragma once
#include <cstdio>
#include <limits>
#include <string>
#include <type_traits>

struct FastIO {
    static constexpr int BUFFER_SIZE = 1 << 20;
    unsigned char buf[BUFFER_SIZE];
    int p = 0, len = 0;
    std::FILE* input;

    explicit FastIO(std::FILE* input = stdin) : input(input) {}

    int gc() {
        if (p == len) {
            len = int(std::fread(buf, 1, BUFFER_SIZE, input));
            p = 0;
            if (!len) return EOF;
        }
        return buf[p++];
    }

    template <class T>
    bool read(T& x) {
        static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>
                      && sizeof(T) <= 8);
        using U = std::make_unsigned_t<T>;
        int c = gc();
        while (c != EOF && c <= ' ') c = gc();
        if (c == EOF) return false;

        bool neg = c == '-';
        if (c == '-' || c == '+') c = gc();
        U value = 0, limit = U(std::numeric_limits<T>::max());
        if constexpr (std::is_signed_v<T>) {
            if (neg) ++limit;
        }
        bool valid = c >= '0' && c <= '9';
        if constexpr (std::is_unsigned_v<T>) {
            if (neg) valid = false;
        }
        while (c != EOF && c > ' ') {
            if (c < '0' || c > '9') {
                valid = false;
            } else if (valid) {
                U digit = U(c - '0');
                if (value > (limit - digit) / 10) valid = false;
                else value = U(value * 10 + digit);
            }
            c = gc();
        }
        if (!valid) return false;
        // Never negate the signed minimum or accumulate in a signed type.
        if constexpr (std::is_signed_v<T>) {
            if (neg && value) x = T(-T(value - 1) - 1);
            else x = T(value);
        } else {
            x = T(value);
        }
        return true;
    }

    bool read(std::string& s) {
        int c = gc();
        while (c != EOF && c <= ' ') c = gc();
        if (c == EOF) return false;
        s.clear();
        while (c != EOF && c > ' ') {
            s += char(c);
            c = gc();
        }
        return true;
    }
};
