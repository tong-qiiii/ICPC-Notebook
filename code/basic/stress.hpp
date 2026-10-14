#pragma once
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

inline bool run_cmd(const std::string& command) {
    return std::system(command.c_str()) == 0;
}

inline bool same_output(const std::string& a, const std::string& b) {
    std::ifstream fa(a), fb(b);
    if (!fa || !fb) return false;
    std::string x, y;
    while (true) {
        bool pa = bool(fa >> x), pb = bool(fb >> y);
        if (fa.bad() || fb.bad() || pa != pb) return false;
        if (!pa) return true;
        if (x != y) return false;
    }
}

// Commands are shell strings. Use a separate working directory; files are overwritten.
inline bool stress(const std::string& generator = "./gen",
                   const std::string& solution = "./sol",
                   const std::string& brute = "./brute",
                   std::uint64_t rounds = 0) {
    for (std::uint64_t tc = 1;; ++tc) {
        if (!run_cmd(generator + " > data.in")) {
            std::cerr << "Generator RE on test " << tc << '\n';
            return false;
        }
        if (!run_cmd(solution + " < data.in > sol.out")) {
            std::cerr << "Solution RE on test " << tc << '\n';
            return false;
        }
        if (!run_cmd(brute + " < data.in > brute.out")) {
            std::cerr << "Brute RE on test " << tc << '\n';
            return false;
        }
        if (!same_output("sol.out", "brute.out")) {
            std::cerr << "WA on test " << tc << '\n';
            return false;
        }
        std::cerr << "AC " << tc << '\r' << std::flush;
        if (rounds && tc == rounds) {
            std::cerr << '\n';
            return true;
        }
    }
}
