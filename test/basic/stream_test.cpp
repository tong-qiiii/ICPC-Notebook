#include "code/basic/stream.hpp"

int main()
{
    assert(split("a,,b,", ',') == vector<string>({"a", "", "b"}));
    assert(split("a,,b,", ',', true) == vector<string>({"a", "", "b", ""}));
    assert(split("", ',').empty());
    assert(split("", ',', true) == vector<string>({""}));
    assert(split(",,", ',', true) == vector<string>({"", "", ""}));
    assert(split("a", ',') == vector<string>({"a"}));
    assert(split(string("a\0b\0", 4), '\0', true) == vector<string>({"a", "b", ""}));
    assert(split("XYZabcXYZXYZ", "XYZ") == vector<string>({"", "abc", "", ""}));
    assert(split("aaaaa", "aa") == vector<string>({"", "", "a"}));
    assert(split("abc", "") == vector<string>({"abc"}));
    assert(split("", "") == vector<string>({""}));
    assert(split("", "x") == vector<string>({""}));
    assert(split_words(" \ta  b\nc\r\n") == vector<string>({"a", "b", "c"}));
    assert(split_words(" \t\r\n").empty());
    assert(read_numbers("1 -2 +3\r\n") == vector<ll>({1, -2, 3}));
    assert(read_numbers("12,34;56|78", ",;|") == vector<ll>({12, 34, 56, 78}));
    assert(read_numbers("1 x 2") == vector<ll>({1}));
    assert(read_numbers("1 9223372036854775808 2") == vector<ll>({1}));
    assert(read_numbers("").empty());
    ll ans = 77;
    for (const string &s : vector<string>{"", " ", "+", "-", "1 2", "12a",
         "0x10", "1.5", "9223372036854775808", "-9223372036854775809",
         string("1\0", 2)})
    {
        assert(!parse_int(s, ans) && ans == 77 && !is_integer(s));
    }
    assert(parse_int(" -9223372036854775808 \t", ans) && ans == LLONG_MIN);
    assert(parse_int("+9223372036854775807", ans) && ans == LLONG_MAX);
    assert(is_integer(" -0 "));
    assert(split_int("1, -2,+3", ',') == vector<ll>({1, -2, 3}));
    assert(split_int("1,2,", ',') == vector<ll>({1, 2}));
    assert(split_int("", ',').empty());
    for (const char *s : {"1,,2", "12a,2", "9223372036854775808"})
    {
        bool caught = false;
        try { split_int(s, ','); }
        catch (const invalid_argument &) { caught = true; }
        assert(caught);
    }
    bool caught = false;
    try { split_int("1,2,", ',', true); }
    catch (const invalid_argument &) { caught = true; }
    assert(caught);
    assert(parse_numbers("x=12,y=-34,z=+56") == vector<ll>({12, -34, 56}));
    assert(parse_numbers("- + ; --2 a- b=7") == vector<ll>({-2, 7}));
    assert(parse_numbers("a=-12;b=+34", false) == vector<ll>({12, 34}));
    assert(parse_numbers("3.14 1e3") == vector<ll>({3, 14, 1, 3}));
    assert(parse_numbers("-9223372036854775808 +9223372036854775807") == vector<ll>({LLONG_MIN, LLONG_MAX}));
    assert(parse_numbers(string(1, char(255)) + "12") == vector<ll>({12}));
    caught = false;
    try { parse_numbers("n=9223372036854775808"); }
    catch (const out_of_range &) { caught = true; }
    assert(caught);

    stringstream ss("2\n  abc\n\nlast");
    int n;
    string line;
    ss >> n;
    ss.ignore(numeric_limits<streamsize>::max(), '\n');
    assert(getline(ss, line) && line == "  abc");
    assert(getline(ss, line) && line.empty());
    assert(getline(ss, line) && line == "last");
    assert(!getline(ss, line));
    ss.clear(); ss.str("2\n  abc\n");
    ss >> n;
    assert(getline(ss >> ws, line) && line == "abc");
    ss.clear(); ss.str("1 2");
    vector<int> a;
    while (ss >> n) a.push_back(n);
    assert(a == vector<int>({1, 2}));
    ss.str("3 4");
    assert(!(ss >> n));
    ss.clear();
    assert(ss >> n && n == 3);

    mt19937_64 rg(20261023);
    for (int t = 0; t < 1200; t++)
    {
        vector<string> words;
        int k = 1 + rg() % 20;
        for (int i = 0; i < k; i++)
        {
            string s(rg() % 10, 'a');
            for (char &c : s) c += rg() % 20;
            words.push_back(s);
        }
        string joined;
        for (int i = 0; i < k; i++) joined += (i ? "|" : "") + words[i];
        assert(split(joined, '|', true) == words);
        joined.clear();
        for (int i = 0; i < k; i++) joined += (i ? "XYZ" : "") + words[i];
        assert(split(joined, "XYZ") == words);
        vector<ll> nums;
        joined.clear();
        for (int i = 0; i < k; i++)
        {
            ll v = (ll)(rg() % 2000000000001ULL) - 1000000000000LL;
            nums.push_back(v);
            joined += (i ? "\t " : "") + to_string(v);
            assert(parse_int(to_string(v), ans) && ans == v);
        }
        assert(read_numbers(joined) == nums);
        assert(parse_numbers(joined) == nums);
    }
    cout << "PASS stream: delimiters, empty fields, strict integers, extraction, EOF/reuse, 1200 random cases\n";
}
