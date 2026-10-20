#pragma once
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

inline bool stress(const std::string& gen = "./gen", const std::string& sol = "./sol",
                   const std::string& brute = "./brute",
                   std::uint64_t rounds = 0)
{
    for (std::uint64_t t = 1;; t++)
    {
        if (std::system((gen + " " + std::to_string(t) + " > data.in").c_str()) ||
            std::system((sol + " < data.in > sol.out").c_str()) ||
            std::system((brute + " < data.in > brute.out").c_str()) ||
            std::system("diff -bB sol.out brute.out"))
        {
            std::cerr << "FAIL seed=" << t << '\n';
            return false;
        }
        std::cerr << "AC " << t << '\n';
        if (rounds && t == rounds) return true;
    }
}
