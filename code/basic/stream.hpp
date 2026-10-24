#pragma once
#include <bits/stdc++.h>
using namespace std;
#ifndef ll
#define ll long long
#endif

inline vector<string> split(const string &s, char delim,
                            bool keep_tail = false)
{
    stringstream ss(s);
    vector<string> res;
    string t;
    while (getline(ss, t, delim)) res.push_back(t);
    if (keep_tail && (s.empty() || s.back() == delim)) res.push_back("");
    return res;
}

inline vector<string> split(const string &s, const string &delim)
{
    if (delim.empty()) return {s};
    vector<string> res;
    size_t st = 0;
    while (true)
    {
        size_t p = s.find(delim, st);
        if (p == string::npos)
        {
            res.push_back(s.substr(st));
            return res;
        }
        res.push_back(s.substr(st, p - st));
        st = p + delim.size();
    }
}

inline vector<string> split_words(const string &s)
{
    stringstream ss(s);
    vector<string> res;
    string t;
    while (ss >> t) res.push_back(t);
    return res;
}

inline vector<ll> read_numbers(string s, const string &delims = "")
{
    for (char &c : s)
    {
        if (delims.find(c) != string::npos) c = ' ';
    }
    stringstream ss(s);
    vector<ll> res;
    ll v;
    while (ss >> v) res.push_back(v);
    return res;
}

// 完整的十进制 ll，允许首尾空白；失败不修改 ans。
inline bool parse_int(const string &s, ll &ans)
{
    stringstream ss(s);
    ll v;
    char c;
    if (!(ss >> v) || (ss >> c)) return false;
    ans = v;
    return true;
}

inline bool is_integer(const string &s)
{
    ll v;
    return parse_int(s, v);
}

inline vector<ll> split_int(const string &s, char delim,
                            bool keep_tail = false)
{
    vector<ll> res;
    for (const string &t : split(s, delim, keep_tail))
    {
        ll v;
        if (!parse_int(t, v)) throw invalid_argument("split_int");
        res.push_back(v);
    }
    return res;
}

// 提取整数片段；符号仅在紧邻数字时生效，不解析小数或科学计数法。
inline vector<ll> parse_numbers(const string &s,
                                bool signed_value = true)
{
    auto digit = [](char c)
    {
        return c >= '0' && c <= '9';
    };
    vector<ll> res;
    for (size_t i = 0; i < s.size();)
    {
        bool sign = signed_value && (s[i] == '-' || s[i] == '+');
        if (!digit(s[i]) &&
            !(sign && i + 1 < s.size() && digit(s[i + 1])))
        {
            i++;
            continue;
        }
        size_t j = i + sign;
        while (j < s.size() && digit(s[j])) j++;
        ll v;
        if (!parse_int(s.substr(i, j - i), v))
            throw out_of_range("parse_numbers");
        res.push_back(v);
        i = j;
    }
    return res;
}
